/**
 * from https://github.com/bang-olufsen/ardHdlc.git
 * @file ardHdlc.h
 */

#ifndef ARDHDLC_H
#define ARDHDLC_H

//take care when you change this to also change your serial buffer size
#define HDLC_FRAME_LENGTH       512

#define ARDHDLC_ESUCCESS    0
#define ARDHDLC_EINVAL      1
#define ARDHDLC_ENOMSG      2
#define ARDHDLC_ETOOSHORT   3
#define ARDHDLC_EFCS        4

/** Supported HDLC frame types */
typedef enum {
  ARDHDLC_FRAME_DATA,
  ARDHDLC_FRAME_ACK,
  ARDHDLC_FRAME_NACK,
} ardHdlc_frame_t;

/** Control field information */
typedef struct {
  ardHdlc_frame_t frame;
  unsigned char seq_no :3;
} ardHdlc_control_t;

/** Variables used in ardHdlc_get_data and ardHdlc_get_data_with_state
 * to keep track of received buffers
 */
typedef struct {
  char control_escape;
  unsigned short fcs;
  int start_index;
  int end_index;
  int src_index;
  int dest_index;
} ardHdlc_state_t;

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Reset the state variable
 */
void ardHdlc_resetState(ardHdlc_state_t *state);

/**
 * Set the library built-in state. The use of this variable is not possible for re-entrant calls
 *
 * @param[in] state The new state to be used
 * @retval 0 Success
 * @retval -EINVAL Invalid parameter
 */
int ardHdlc_setState(ardHdlc_state_t *state);

/**
 * Get current library  built-instate. The use of this variable is not possible for re-entrant calls
 * Typically used by python because it's not easy to pass the ardHdlc_state_t struct from python to C
 *
 * @param[out] state Current state
 * @retval 0 Success
 * @retval -EINVAL Invalid parameter
 */
int ardHdlc_getState(ardHdlc_state_t *state);

/**
 * Retrieves data from specified buffer containing the HDLC frame. Frames can be
 * parsed from multiple buffers e.g. when received via UART.
 *
 * @param[out] control Control field structure with frame type and sequence number
 * @param[in] state : the state tracking variable (so that the function is reentrant)
 * @param[in] src Source buffer with frame
 * @param[in] src_len Source buffer length
 * @param[out] dest Destination buffer (should be able to contain max frame size)
 * @param[out] dest_len Destination buffer length
 * @retval >=0 Success (size of returned value should be discarded from source buffer)
 * @retval -EINVAL Invalid parameter
 * @retval -ENOMSG No message has been found, the input buffer is still valid, but bytes are not received yet
 * @retval -ETOOSHORT The decoded message is too short for being a valid message, the input buf should be cleaned of the first "dest_len" bytes
 * @retval -EFCS The frame check sequence is invalid, message is corrupted,  the input buf should be cleaned of the first "dest_len" bytes
 */

int ardHdlc_decodeFrame(ardHdlc_state_t *state, ardHdlc_control_t *control, const char *src,
                               unsigned int src_len, char *dest, unsigned int *dest_len);

/**
 * Creates HDLC frame with specified data buffer.
 *
 * @param[in] control Control field structure with frame type and sequence number
 * @param[in] src Source buffer with data
 * @param[in] src_len Source buffer length
 * @param[out] dest Destination buffer (should be bigger than source buffer)
 * @param[out] dest_len Destination buffer length
 * @retval 0 Success
 * @retval -EINVAL Invalid parameter
 */
int ardHdlc_createDataFrame(ardHdlc_control_t *control, const char *src,
                      unsigned int src_len, char *dest, unsigned int *dest_len);

#ifdef __cplusplus
}
#endif

#endif
