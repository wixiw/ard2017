/*
 * ComInterfaces.h
 *
 *  Created on: 10 janv. 2017
 *      Author: wix
 */

#ifndef ROBOT_COMMON_1_RSP_COM_COMINTERFACES_H_
#define ROBOT_COMMON_1_RSP_COM_COMINTERFACES_H_

#include <stddef.h>
#include <stdint.h>

namespace ard
{
    //ARD
    class ISerialDriver
    {
        public:
            typedef enum
            {
                ERR_NO_ERROR,
                ERR_FRAME,
                ERR_PARITY,
                ERR_OVERSHOOT_RX,
                ERR_OVERSHOOT_TX,
                ERR_MISSED_RX_IRQ,
            } eError;


            virtual ~ISerialDriver(){};

            /**
             * Read a byte from the serial line (byte have been received previsouly and buffered in RAM)
             */
            virtual void read(uint8_t * const byte) = 0;

            /**
             * Write the byte on the serial line (byte may be buffered in RAM before being send later)
             */
            virtual void write(uint8_t byte) = 0;

            /**
             * Wait until the content of the tx buffer is sent the serial line. It's a blocking call
             */
            virtual void flush(void) = 0;

            /**
             * Print an ascii text finished by a null character on serial line
             */
            void print(char const * text);
            void println(char const * text);


            /**
             * Get the number of received bytes in the rxBuf
             * It's safer to use write with several chars to ensure thread safety
             */
            virtual int getNbBytesReceived(void) = 0;

            /**
             * Get the number of free bytes in the txBuf
             * It's safer to use write with several chars to ensure thread safety
             */
            virtual int getNbBytesAvailableForWrite(void) = 0;

            /**
             * @return true in any case where a byte has been lost during reception
             */
            virtual eError getError(){return ERR_NO_ERROR;};
    };
    
    /**
     * Decoupling listener interface for ICom
     */
    class ICom;
    class IComListener
    {
    public:
        virtual ~IComListener();
        virtual void com_connected(ICom const* origin) = 0;
        virtual void com_disconnected(ICom const* origin) = 0;

        //A message has been received from the communication media
        //@param origin : the ICom interface that is calling the method
        //@param msg : the data to decode
        //@param msgLength : the length of the data buffer
        virtual void handleMsg(ICom const* origin, char const * msg, size_t msgLength) = 0;
    };

    /**
     * This class is the interface to use for any communication stack.
     */
    class ICom
    {
    public:
        virtual ~ICom();
        virtual void setListener(IComListener* newListener) = 0;

        //Open the communication stack.
        //You shoudld check that isStarted() is true before sending messages
        virtual bool connect() = 0;

        //Close the communication stack
        virtual void disconnect() = 0;

        //@returns true if the Communication is up and ready to communicate
        virtual bool isConnected() const = 0;

        //Write a message on the communication media
        virtual bool sendMsg(char const * msg, size_t msgLength) = 0;

        //Read all available data and parse it to find messages
        virtual void readAll() = 0;
    };
    
    
    //--------------------------------------------------------------------------
    // FAKES
    //--------------------------------------------------------------------------
    class FakeSerialDriver: public ISerialDriver
    {
        public:
            virtual ~FakeSerialDriver(){};
                
            /**
             * Implements ISerialDriver
             */            
            virtual void read(uint8_t * const byte){*byte = 0;};
                
            /**
             * Implements ISerialDriver
             */
            virtual void write(uint8_t byte){};

            /**
             * Implements ISerialDriver
             */
            virtual int getNbBytesReceived(void){return 0;};

            /**
             * Implements ISerialDriver
             */
            virtual int getNbBytesAvailableForWrite(void){return 0;};
    };

}

#endif /* ROBOT_COMMON_1_RSP_COM_COMINTERFACES_H_ */
