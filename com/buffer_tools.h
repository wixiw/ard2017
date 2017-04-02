/*
 * buffer_buffer.h
 *
 *  Created on: Feb 20, 2015
 *      Author: willy
 */

#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C"
{
#endif

// The following structs and functions are tools to operate buffers with variable
// content, allowing an easy and secure access to occupied/free content
// For better use in C drivers, the following code is design is an object oriented
// manner. In order to fit in static initialized applications, all the memmory has
// to be pre-allocated and linked to opaque pointer representing the object.
// WARNING : the library is not protected against IT/Thread or Thread/Thread competition

//--------------------------------------------------------------------
//             LINEAR BUFFER
//--------------------------------------------------------------------

// A circular buffer is a buffer which valid data is not necessarily at the beginning
// It is often used to create a FILO stack
// data are not zeroed

//--------------------------------------------------------------------

//opaque structure, should not be used directly
typedef struct
{
    uint8_t* data;      //pointer on buffer start
    uint16_t size;  //total size of the buffer
    uint16_t end;   //pointer on data end. data[end] is the next place to write

} LinearBuffer;

//--------------------------------------------------------------------
//             CIRCULAR BUFFER
//--------------------------------------------------------------------

// A circular buffer is a buffer which valid data is not necessarily at the beginning
// It is often used to create a FIFO queue*
// data are not zeroed

// Ex :
// #define BUF_SIZE 20
// CircularBuffer myCircularBuffer;
// uint8_t _privateMemory[BUF_SIZE];
// buffer_init(&myCircularBuffer, _privateMemory, BUF_SIZE); //it represents a C++ call to a constructor like : "CircularBuffer myCircularBuffer = CircularBuffer(LIN_BUF_SIZE)"
// uint16_t freeSpace = buffer_getFreeRoom(&myCircularBuffer); //it represents a C++ call to a method call like : "myCircularBuffer.getFreeRoom()"

//--------------------------------------------------------------------

//opaque structure, should not be used directly
typedef struct
{
    uint8_t* data;      //pointer on buffer start
    uint16_t size;      //total size of the buffer
    uint16_t start;     //pointer on data begin. data[start] is the next place to read
    uint16_t end;       //pointer on data end. data[end] is the next place to write

} CircularBuffer;

//--------------------------------------------------------------------
//             BUFFER OPERATIONS
//--------------------------------------------------------------------

//initialize the buffer with a pre-allocated memory
//shall be called before any other call
//it will zero the pre-allocated buffer
//Take care in case of a circular buffer one byte is always free so you can only have size-1 data
//@param object : an opaque pointer
//@param buffer : the pre-allocated buffer
//@param size   : the size of the preallocated buffer (shall be at least 2 for a circular buffer, else, forced to 2)
void linear_init(LinearBuffer     * const object, uint8_t * const buffer, uint16_t const size);
void circular_init(CircularBuffer * const object, uint8_t * const buffer, uint16_t const size);

//reset the buffer and empty the memory
void linear_reset(LinearBuffer     * const object);
void circular_reset(CircularBuffer * const object);

//returns the occupied room in the buffer
uint16_t linear_getOccupiedRoom(LinearBuffer     const * const object);
uint16_t circular_getOccupiedRoom(CircularBuffer const * const object);

//returns the free room in the buffer
uint16_t linear_getFreeRoom(LinearBuffer     const * const object);
uint16_t circular_getFreeRoom(CircularBuffer const * const object);

//add a char at the end of the buffer
//@param byte : the byte to write in the buffer
//@return true in case of success, false in any error case
bool linear_appendByte(LinearBuffer     * const object, uint8_t const byte);
bool circular_appendByte(CircularBuffer * const object, uint8_t const byte);

//remove a char at the begining of the buffer
//@param byte : the memory to set with the buffer popped data
//@return true in case of success, false in any error case
bool linear_getLastByte(LinearBuffer * const object, uint8_t * const byte);
bool circular_popByte(CircularBuffer * const object, uint8_t * const byte);

//extract data from one buffer and append it to another buffer
//parameter order is equivalent to the one used by memcpy
//@param source : the source buffer from which the data is extracted
//@param target : the destination buffer where the data is sent
//@param sizeToMove : the number of bytes to move,
//                  for non raw inputs, if set to 0 the whole buffer if taken, in this case,
//                  if the target buffer can't hold the data, data is copyed until it's full
//@return : true in case of success false in any error condition
bool buffer_move_cir2lin(LinearBuffer* const    dest,     CircularBuffer* const source,    uint16_t const sizeToMove);
bool buffer_move_lin2cir(CircularBuffer* const  dest,     LinearBuffer* const   source,    uint16_t const sizeToMove);

//copy data from or to a raw buffer. Data a appended to linear/circular buffers
//parameter order is equivalent to the one used by memcpy
//note that there is no protection on raw buffer overflow, it's your job to check (or use a linear buffer instead)
//@param source : the source buffer from which the data is read
//@param target : the destination buffer where the data is writtent
//@param sizeToCopy : the number of bytes to copy,
//                  for non raw inputs, if set to 0 the whole buffer if taken, in this case,
//                  if the target buffer can't hold the data, data is copyed until it's full
//@return : true in case of success false in any error condition
bool buffer_copy_cir2raw(uint8_t* const         dest,     CircularBuffer* const source,    uint16_t const sizeToCopy);
bool buffer_copy_raw2cir(CircularBuffer* const  dest,     uint8_t const* const  source,    uint16_t const sizeToCopy);


#ifdef __cplusplus
}
#endif

#endif /* CIRCULAR_BUFFER_H_ */
