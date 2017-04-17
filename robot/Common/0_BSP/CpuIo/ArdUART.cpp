/*
 Copyright (c) 2011 Arduino.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "ArdUART.h"
#include "BSP.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <chip.h>

using namespace ard;

// Constructors ////////////////////////////////////////////////////////////////

ArdUART::ArdUART(Uart* pUart, uint32_t dwId, size_t const rxSize, size_t const txSize):
    rxBuf(),
    txBuf(),
    rxSem(rxSize, 0),
    txSem(txSize, txSize),
    firstByteReceived(false),
    prioritySet(false),
    baseAddr(pUart),
    deviceId(dwId),
    irqId(static_cast<IRQn_Type>(dwId)),
    nbRxBytes(0),
    nbTxBytes(0),
    nbRxBytesLost(0),
    nbTxBytesLost(0),
    nbFrameError(0),
    nbParityError(0),
    nbMissedIrq(0),
    lastError(ISerialDriver::ERR_NO_ERROR)
{
    //Add +1 to sizes as a circular buffer always keep 1 byte free to separate head and tail
    _memory_rxBuf = static_cast<uint8_t*>(malloc(rxSize+1));
    _memory_txBuf = static_cast<uint8_t*>(malloc(txSize+1));
    circular_init(&rxBuf, _memory_rxBuf, rxSize+1);
    circular_init(&txBuf, _memory_txBuf, txSize+1);
}

// Public Methods //////////////////////////////////////////////////////////////

void ArdUART::start(const uint32_t dwBaudRate, uint32_t config)
{
    //Ensure that a priority has been defined (see setInterruptPriority())
    ASSERT(prioritySet);

    // Make sure both ring buffers are initialized back to empty.
    circular_reset(&rxBuf);
    circular_reset(&txBuf);

    // Configure PMC
    pmc_enable_periph_clk(deviceId);

    // Disable PDC channel
    baseAddr->UART_PTCR = UART_PTCR_RXTDIS | UART_PTCR_TXTDIS;

    // Reset and disable receiver and transmitter
    baseAddr->UART_CR = UART_CR_RSTRX | UART_CR_RSTTX | UART_CR_RXDIS | UART_CR_TXDIS;

    // Configure mode
    baseAddr->UART_MR = config;

    // Configure baudrate (asynchronous, no oversampling)
    baseAddr->UART_BRGR = (SystemCoreClock / dwBaudRate) >> 4; // >>4 == /16 because baudrate is main clock / (16*BRGR)
    ASSERT(baseAddr->UART_BRGR != 0); // With a system clock of 4Mhz it means 250kbit/s max

    // Configure interrupts
    baseAddr->UART_IDR = 0xFFFFFFFF;
    baseAddr->UART_IER = UART_IER_RXRDY | UART_IER_OVRE | UART_IER_FRAME;

    // Enable UART interrupt in NVIC
    NVIC_EnableIRQ(irqId);

    // Enable receiver and transmitter
    baseAddr->UART_CR = UART_CR_RXEN | UART_CR_TXEN;
}

void ArdUART::stop(void)
{
    // Wait for any outstanding data to be sent
    flush();

    // Disable UART interrupt in NVIC
    NVIC_DisableIRQ (irqId);
    pmc_disable_periph_clk(deviceId);

    // Make sure both ring buffers are initialized back to empty.
    circular_reset(&rxBuf);
    circular_reset(&txBuf);
}


void ArdUART::flush(void)
{
    //wait for tx buffer to be emptied
    while (circular_getOccupiedRoom(&txBuf))
    {
        vTaskDelay(10);
    }

    // Wait for transmission to complete
    while ((baseAddr->UART_SR & UART_SR_TXEMPTY) != UART_SR_TXEMPTY);
}

void ArdUART::read(uint8_t * const byte)
{
//    dh_publish_event("read:take", *byte, 0);
    rxSem.take();

    Thread::enterCriticalSection();
    bool byteReceived = circular_popByte(&rxBuf, byte);
    Thread::exitCriticalSection();
    nbTxBytes++;

    ASSERT(byteReceived);
//    dh_publish_event("read:done", *byte, 0);
}

void ArdUART::write(uint8_t byte)
{
//    dh_publish_event("write:take", byte, 0);
    txSem.take();

    Thread::enterCriticalSection();
    bool roomAvailable = circular_appendByte(&txBuf, byte);
    Thread::exitCriticalSection();
    nbRxBytes++;

    ASSERT(roomAvailable);
//    dh_publish_event("write:done", byte, 0);

    //inform HW that new data are available
    baseAddr->UART_IER = UART_IER_TXRDY;
}

void ArdUART::IrqHandler(void)
{
    uint32_t status = baseAddr->UART_SR;
    uint8_t txByte = 0;
    uint8_t rxByte = 0;
    
    // Acknowledge and report errors (OVRE means that a new IT was generated before the char is read by previous one)
    if (status & UART_SR_OVRE)
    {
        nbMissedIrq++;
        nbRxBytesLost++;
        lastError = ERR_MISSED_RX_IRQ;
        //drop erroneous byte
        rxByte = baseAddr->UART_RHR;
        //acknowledge error
        baseAddr->UART_CR |= UART_CR_RSTSTA;

//        dh_publish_event_fromISR("ErrMisIt", nbMissedIrq, 0);
        return;
    }

    // Acknowledge and report errors (FRAME means that an electrical perturbation occured on the line, especially on normal condition at connection )
    if( status & UART_SR_FRAME )
    {
        // this error is normally generated at connection time until emmitter/receptor are in sync (one or 2 bytes)
        if( firstByteReceived)
        {
            nbFrameError++;
            lastError = ERR_FRAME;
            //drop erroneous byte
            rxByte = baseAddr->UART_RHR;
            //acknowledge error
            baseAddr->UART_CR |= UART_CR_RSTSTA;

//            dh_publish_event_fromISR("ErrFrame", nbFrameError, 0);
        }
        else
        {
            //drop erroneous byte
            rxByte = baseAddr->UART_RHR;
            //acknowledge error
            baseAddr->UART_CR |= UART_CR_RSTSTA;
        
//            dh_publish_event_fromISR("ErrCon", nbFrameError, 0);
        }

        return;
    }

    // Acknowledge and report errors (PARE=parity means that an electrical perturbation occured on the line and the data if broken )
    if (status & UART_SR_PARE)
    {
        nbParityError++;
        nbRxBytesLost++;
        lastError = ERR_PARITY;
        //drop erroneous byte
        rxByte = baseAddr->UART_RHR;
        //acknowledge error
        baseAddr->UART_CR |= UART_CR_RSTSTA;

//        dh_publish_event_fromISR("ErrParity", nbParityError, 0);
        return;
    }
    
    // Did we receive data?
    if ((status & UART_SR_RXRDY) == UART_SR_RXRDY)
    {
        firstByteReceived = true;

        //if the store failed, the buffer is overflown, save the error
        rxByte = baseAddr->UART_RHR;
        if ( circular_appendByte(&rxBuf, rxByte) )
        {
            ASSERT(rxSem.giveFromISR());
//            dh_publish_event_fromISR("it::RX", rxByte, 0);
        }
        else
        {
            nbRxBytesLost++;
            lastError = ERR_OVERSHOOT_RX;
//            dh_publish_event_fromISR("ErrOvRx", nbRxBytesLost, 0);
        }
    }

    // Do we need to keep sending data?
    if ((status & UART_SR_TXRDY) == UART_SR_TXRDY)
    {
        if (circular_popByte(&txBuf, &txByte))
        {
            ASSERT(txSem.giveFromISR());

//            dh_publish_event_fromISR("UART::ItTx", txByte, 0);

            //write the popped byte into the TX device register
            baseAddr->UART_THR = txByte;
        }
        else
        {
            // If pop failed, there is no data to send
            // Mask off transmit interrupt so we don't get it anymore
            baseAddr->UART_IDR = UART_IDR_TXRDY;
        }
    }


}


