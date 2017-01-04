#ifndef hdlc_h
#define hdlc_h

#include <Arduino.h>
#include <stdint.h>
#include <stdbool.h>
#include "lib_crc.h"

typedef void (* sendchar_type) (uint8_t);
typedef void (* frame_handler_type)(const uint8_t *framebuffer, uint16_t framelength);

#define MAX_HDLC_FRAME_LENGTH 32

//ARD :
//Taken from : https://github.com/jarkko-hautakorpi/Arduhdlc

class Hdlc
{
  public:
    Hdlc();
    virtual ~Hdlc() = default;
    void receiveNextByte(uint8_t data);
    void encodeFrame(const char *framebuffer, uint8_t frame_length);

  private:
    //Child class is expected to implement this callback to send a byte on the serial link
    virtual void sendByte(uint8_t data) = 0;

    //Child class is expected to implement this callback in order to parse the frame payload
    virtual void handleFrame(const uint8_t *framebuffer, uint16_t framelength) = 0;

    bool escape_character;
    uint8_t receive_frame_buffer[MAX_HDLC_FRAME_LENGTH];
    uint8_t frame_position;
    // 16bit CRC sum for _crc_ccitt_update
    uint16_t frame_checksum;
	uint16_t max_frame_length;
};

#endif
