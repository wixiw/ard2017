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

#ifndef _ARD_UART_
#define _ARD_UART_

#include <sam.h>
#include "ComInterfaces.h"
#include "utility/buffer_tools.h"
#include "ArdOs.h"

namespace ard
{
    
/**
 * The ArdUART class is a driver to send/receive chars on a serial line of UART type
 */
class ArdUART: public ISerialDriver
{
  public:

    /**
     * Parity configuration
     */
    enum UARTModes {
      Mode_8N1 = UART_MR_PAR_NO,
      Mode_8E1 = UART_MR_PAR_EVEN,
      Mode_8O1 = UART_MR_PAR_ODD,
      Mode_8M1 = UART_MR_PAR_MARK,
      Mode_8S1 = UART_MR_PAR_SPACE,
    };

    /**
     * Constructor. Memory is allocated on the heap
     * @param pUArt : the base address of the associated UART device
     * @param dwId  : the device identifier (see robot/Common/0_BSP/CMSIS/Device/ATMEL/sam3xa/include/sam3x8e.h)
     * @param rxSize: the size of the rxBuf
     * @param txSize: the size of the txBuf
     *
     */
    ArdUART(Uart* pUart,
            uint32_t dwId,
            size_t const rxSize,
            size_t const txSize);


    /**
     * Start operating the serial line : device is configured and interrupt activated
     * After the call, the interrupt will begin to feed the rxBuf
     * @param dwBaudRate : line baudrate in bits/s
     * @param config     : the parity configuration/loopback configuration
     */
    void start(const uint32_t dwBaudRate, const uint32_t config);
    void start(const uint32_t dwBaudRate){
        start(dwBaudRate, Mode_8N1 | UART_MR_CHMODE_NORMAL);
    }

    /**
     * Stop operating the serial line : interrupt is deactivated
     */
    void stop(void);

    /**
     * Wait until the content of the tx buffer is sent the serial line. It's a blocking call
     */
    void flush(void);

    /**
     * Implements ISerialDriver
     * Read a byte from the RX circular buffer (which has been populated by the interrupt)
     * blocks until a data is available
     */
    void read(uint8_t * const byte) override;

    /**
     * Implements ISerialDriver
     * Append a byte to the TX circular buffer (which will be sent by interrupt later on)
     * blocks until a data is available
     */
    void write(uint8_t byte) override;

    /**
     * Function to be called in the interrupt handler
     */
    void IrqHandler(void);

    /**
     * Implements ISerialDriver: flag is reset after read
     */
    eError getError() override
    {
        ISerialDriver::eError res = lastError;
        lastError = ISerialDriver::ERR_NO_ERROR;
        return res;
    }

    /**
     * Implements ISerialDriver
     */
    int getNbBytesReceived(void)
    {
        return circular_getOccupiedRoom(&rxBuf);
    }

    /**
     * Implements ISerialDriver
     */
    int getNbBytesAvailableForWrite(void)
    {
        return circular_getFreeRoom(&txBuf);
    }

    void setInterruptPriority(uint32_t priority)
    {
        NVIC_SetPriority(irqId, priority & 0x0F);
    }

    uint32_t getInterruptPriority()
    {
        return NVIC_GetPriority(irqId);
    }

  protected:
    uint8_t* _memory_rxBuf;
    uint8_t* _memory_txBuf;
    CircularBuffer rxBuf;
    CircularBuffer txBuf;
    ard::Semaphore rxSem; //counts the number of received bytes in the rxBuf
    ard::Semaphore txSem; //counts the number of free space in the txBuf
    bool firstByteReceived; //keeps in mind if a first byte has been received to mask some frame error at connection

    Uart* baseAddr;
    uint32_t deviceId;
    IRQn_Type irqId;

    //Keep track of errors detected when received
    uint32_t nbRxBytesLost;
    uint32_t nbTxBytesLost;
    uint32_t nbFrameError;
    uint32_t nbParityError;
    uint32_t nbMissedIrq;
    eError lastError;

};

};//end namespace

#endif // _ARD_UART_
