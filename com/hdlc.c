//from https://github.com/bang-olufsen/ardHdlc.git

#include "hdlc.h"

/* ------------------------------------------------------------------------------------------
 *  PRIVATE API
 ---------------------------------------------------------------------------------------------*/

// HDLC Control field bit positions
#define ARDHDLC_CONTROL_S_OR_U_FRAME_BIT 0
#define ARDHDLC_CONTROL_SEND_SEQ_NO_BIT 1
#define ARDHDLC_CONTROL_S_FRAME_TYPE_BIT 2
#define ARDHDLC_CONTROL_POLL_BIT 4
#define ARDHDLC_CONTROL_RECV_SEQ_NO_BIT 5

// HDLC Control type definitions
#define ARDHDLC_CONTROL_TYPE_RECEIVE_READY 0
#define ARDHDLC_CONTROL_TYPE_RECEIVE_NOT_READY 1
#define ARDHDLC_CONTROL_TYPE_REJECT 2
#define ARDHDLC_CONTROL_TYPE_SELECTIVE_REJECT 3

/** HDLC start/end flag sequence */
#define ARDHDLC_FLAG_SEQUENCE 0x7E

/** HDLC control escape value */
#define ARDHDLC_CONTROL_ESCAPE 0x7D

/** HDLC all station address */
#define ARDHDLC_ALL_STATION_ADDR 0xFF

/** FCS initialization value. */
#define FCS16_INIT_VALUE 0xFFFF

/** FCS value for valid frames. */
#define FCS16_GOOD_VALUE 0xF0B8

/** Minimal message size (empty payload). */
#define ARDHDLC_EMPTY_MSG_SIZE 6

//from https://github.com/bang-olufsen/ardHdlc.git
//which is certainly inspired by https://tools.ietf.org/html/rfc1549
static const unsigned short fcstab[256] =
{ 0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf, 0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7, 0x1081, 0x0108, 0x3393,
        0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e, 0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876, 0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af,
        0x4434, 0x55bd, 0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5, 0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c, 0xbdcb,
        0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974, 0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb, 0xce4c, 0xdfc5, 0xed5e, 0xfcd7,
        0x8868, 0x99e1, 0xab7a, 0xbaf3, 0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a, 0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb,
        0xaa72, 0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9, 0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1, 0x7387, 0x620e,
        0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738, 0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70, 0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c,
        0xd3a5, 0xe13e, 0xf0b7, 0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff, 0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
        0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e, 0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5, 0x2942, 0x38cb, 0x0a50,
        0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd, 0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134, 0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e,
        0x5cf5, 0x4d7c, 0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3, 0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb, 0xd68d,
        0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232, 0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a, 0xe70e, 0xf687, 0xc41c, 0xd595,
        0xa12a, 0xb0a3, 0x8238, 0x93b1, 0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9, 0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9,
        0x8330, 0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78 };

/**
 * Calculates a new checksum based on the current value and value of data.
 * FCS stand for Frame Check Sequence
 *
 * @param fcs Current FCS value
 * @param value The value to be added
 * @returns Calculated FCS value
 */
unsigned short fcs16(unsigned short fcs, unsigned char value)
{
    return (fcs >> 8) ^ fcstab[(fcs ^ value) & 0xff];
}

static ardHdlc_state_t ardHdlc_state =
{ .control_escape = 0, .fcs = FCS16_INIT_VALUE, .start_index = -1, .end_index = -1, .src_index = 0, .dest_index = 0, };

int ardHdlc_setState(ardHdlc_state_t *state)
{
    if (!state)
    {
        return -1;
    }

    ardHdlc_state = *state;
    return 0;
}

int ardHdlc_getState(ardHdlc_state_t *state)
{
    if (!state)
    {
        return -ARDHDLC_EINVAL;
    }

    *state = ardHdlc_state;
    return 0;
}

//If the destination size is overshoot, return 0 else 1
int ardHdlc_escape_value(char value, char *dest, unsigned int *dest_index, unsigned int max_dest_length)
{
    if (max_dest_length <= *dest_index)
        return 0;

    // Check and escape the value if needed
    if ((value == ARDHDLC_FLAG_SEQUENCE) || (value == ARDHDLC_CONTROL_ESCAPE))
    {
        dest[(*dest_index)++] = ARDHDLC_CONTROL_ESCAPE;
        value ^= 0x20;
    }

    if (max_dest_length <= *dest_index)
        return 0;

    // Add the value to the destination buffer and increment destination index
    dest[(*dest_index)++] = value;
    return 1;
}

ardHdlc_control_t ardHdlc_parseControlByte(unsigned char control)
{
    ardHdlc_control_t value;

    // Check if the frame is a S-frame (or U-frame)
    if (control & (1 << ARDHDLC_CONTROL_S_OR_U_FRAME_BIT))
    {
        // Check if S-frame type is a Receive Ready (ACK)
        if (((control >> ARDHDLC_CONTROL_S_FRAME_TYPE_BIT) & 0x3) == ARDHDLC_CONTROL_TYPE_RECEIVE_READY)
        {
            value.frame = ARDHDLC_FRAME_ACK;
        }
        else
        {
            // Assume it is an NACK since Receive Not Ready, Selective Reject and U-frames are not supported
            value.frame = ARDHDLC_FRAME_NACK;
        }

        // Add the receive sequence number from the S-frame (or U-frame)
        value.seq_no = (control >> ARDHDLC_CONTROL_RECV_SEQ_NO_BIT);
    }
    else
    {
        // It must be an I-frame so add the send sequence number (receive sequence number is not used)
        value.frame = ARDHDLC_FRAME_DATA;
        value.seq_no = (control >> ARDHDLC_CONTROL_SEND_SEQ_NO_BIT);
    }

    return value;
}

unsigned char ardHdlc_buildControlByte(ardHdlc_control_t *control)
{
    unsigned char value = 0;

    // For details see: https://en.wikipedia.org/wiki/High-Level_Data_Link_Control
    switch (control->frame)
    {
    case ARDHDLC_FRAME_DATA:
        // Create the HDLC I-frame control byte with Poll bit set
        value |= (control->seq_no << ARDHDLC_CONTROL_SEND_SEQ_NO_BIT);
        value |= (1 << ARDHDLC_CONTROL_POLL_BIT);
        break;
    case ARDHDLC_FRAME_ACK:
        // Create the HDLC Receive Ready S-frame control byte with Poll bit cleared
        value |= (control->seq_no << ARDHDLC_CONTROL_RECV_SEQ_NO_BIT);
        value |= (1 << ARDHDLC_CONTROL_S_OR_U_FRAME_BIT);
        break;
    case ARDHDLC_FRAME_NACK:
        // Create the HDLC Receive Ready S-frame control byte with Poll bit cleared
        value |= (control->seq_no << ARDHDLC_CONTROL_RECV_SEQ_NO_BIT);
        value |= (ARDHDLC_CONTROL_TYPE_REJECT << ARDHDLC_CONTROL_S_FRAME_TYPE_BIT);
        value |= (1 << ARDHDLC_CONTROL_S_OR_U_FRAME_BIT);
        break;
    }

    return value;
}

/* ------------------------------------------------------------------------------------------
 *  PUBLIC API
 ---------------------------------------------------------------------------------------------*/

void ardHdlc_resetState(ardHdlc_state_t *state)
{
    state->fcs = FCS16_INIT_VALUE;
    state->start_index = state->end_index = -1;
    state->src_index = state->dest_index = 0;
    state->control_escape = 0;
}

int ardHdlc_decodeFrame(ardHdlc_state_t *state, ardHdlc_control_t *control, const char *src, unsigned int src_len, char *dest, unsigned int *dest_len)
{
    int ret;
    char value;
    unsigned int i;

    // Make sure that all parameters are valid
    if (!state || !control || !src || !dest || !dest_len)
    {
        return -ARDHDLC_EINVAL;
    }

    // Run through the data bytes
    for (i = state->src_index; i < src_len; i++)
    {
        // First find the start flag sequence
        if (state->start_index < 0)
        {
            if (src[i] == ARDHDLC_FLAG_SEQUENCE)
            {
                // Check if an additional flag sequence byte is present
                if ((i < (src_len - 1)) && (src[i + 1] == ARDHDLC_FLAG_SEQUENCE))
                {
                    // Just loop again to silently discard it (accordingly to HDLC)
                    continue;
                }

                state->start_index = state->src_index;
            }
        }
        else
        {
            // Check for end flag sequence
            if (src[i] == ARDHDLC_FLAG_SEQUENCE)
            {
                state->end_index = state->src_index;
                break;
            }
            else if (src[i] == ARDHDLC_CONTROL_ESCAPE)
            {
                state->control_escape = 1;
            }
            else
            {
                // Update the value based on any control escape received
                if (state->control_escape)
                {
                    state->control_escape = 0;
                    value = src[i] ^ 0x20;
                }
                else
                {
                    value = src[i];
                }

                // Now update the FCS value
                state->fcs = fcs16(state->fcs, value);

                if (state->src_index == state->start_index + 2)
                {
                    // Control field is the second byte after the start flag sequence
                    *control = ardHdlc_parseControlByte(value);
                }
                else if (state->src_index > (state->start_index + 2))
                {
                    // Start adding the data values after the Control field to the buffer
                    dest[state->dest_index++] = value;
                }
            }
        }
        state->src_index++;
    }

    // Check for invalid frame (no start or end flag sequence)
    if ((state->start_index < 0) || (state->end_index < 0))
    {
        // Return no message and make sure destination length is 0
        *dest_len = 0;
        ret = -ARDHDLC_ENOMSG;
    }
    // A frame is at least ARDHDLC_EMPTY_MSG_SIZE bytes in size
    else if (state->end_index < state->start_index + ARDHDLC_EMPTY_MSG_SIZE)
    {
        // Return error and indicate that data up to end flag sequence in buffer should be discarded
        *dest_len = state->end_index + 1;
        ret = -ARDHDLC_ETOOSHORT;
    }
    //A frame shall have a correct checksum value (FCS)
    else if (state->fcs != FCS16_GOOD_VALUE)
    {
        // Return FCS error and indicate that data up to end flag sequence in buffer should be discarded
        *dest_len = state->end_index + 1;
        ret = -ARDHDLC_EFCS;
    }
    //A clean message has been processed
    else
    {
        // Return success and indicate that data up to end flag sequence in buffer should be discarded
        *dest_len = state->dest_index - sizeof(state->fcs);
        ret = state->src_index;
    }

    if (ret != -ARDHDLC_ENOMSG)
    {
        // Reset values for next frame
        ardHdlc_resetState(state);
    }

    return ret;
}

int ardHdlc_createDataFrame(ardHdlc_control_t *control, const char *src, unsigned int src_len, char *dest, unsigned int *dest_len)
{
    unsigned int i;
    unsigned int dest_index = 0;
    unsigned char value = 0;
    unsigned short fcs = FCS16_INIT_VALUE;

    // Make sure that all parameters are valid
    if (!control || (!src && (src_len > 0)) || !dest || !dest_len)
    {
        return -ARDHDLC_EINVAL;
    }

    //ensure destination is at least minimal size
    unsigned int max_dest_length = *dest_len;
    if (max_dest_length <= ARDHDLC_EMPTY_MSG_SIZE)
    {
        return -ARDHDLC_ETOOSHORT;
    }

    // Start by adding the start flag sequence
    dest[dest_index++] = ARDHDLC_FLAG_SEQUENCE;

    // Add the all-station address from HDLC (broadcast)
    fcs = fcs16(fcs, ARDHDLC_ALL_STATION_ADDR);
    if( !ardHdlc_escape_value(ARDHDLC_ALL_STATION_ADDR, dest, &dest_index, max_dest_length) )
    {
        return -ARDHDLC_ETOOSHORT;
    }

    // Add the framed control field value
    value = ardHdlc_buildControlByte(control);
    fcs = fcs16(fcs, value);
    if(!ardHdlc_escape_value(value, dest, &dest_index, max_dest_length))
    {
        return -ARDHDLC_ETOOSHORT;
    }

    // Only DATA frames should contain data
    if (control->frame == ARDHDLC_FRAME_DATA)
    {
        // Calculate FCS and escape data
        for (i = 0; i < src_len; i++)
        {
            fcs = fcs16(fcs, src[i]);
            if(!ardHdlc_escape_value(src[i], dest, &dest_index, max_dest_length))
            {
                return -ARDHDLC_ETOOSHORT;
            }
        }
    }

    // Invert the FCS value accordingly to the specification
    fcs ^= 0xFFFF;

    // Run through the FCS bytes and escape the values
    for (i = 0; i < sizeof(fcs); i++)
    {
        value = ((fcs >> (8 * i)) & 0xFF);
        if(!ardHdlc_escape_value(value, dest, &dest_index, max_dest_length))
        {
            return -ARDHDLC_ETOOSHORT;
        }
    }

    // Add end flag sequence and update length of frame
    if(max_dest_length <= dest_index)
    {
        return -ARDHDLC_ETOOSHORT;
    }
    dest[dest_index++] = ARDHDLC_FLAG_SEQUENCE;
    *dest_len = dest_index;

    return ARDHDLC_ESUCCESS;
}
