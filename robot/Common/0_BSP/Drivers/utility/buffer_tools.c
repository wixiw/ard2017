/*
 * buffer_buffer.c
 *
 *  Created on: Feb 20, 2015
 *      Author: willy
 */

#include "buffer_tools.h"
#include <string.h>

//no optimization because it prevent a correct index increment for unknown reason

//--------------------------------------------------------------------
//             LINEAR BUFFER
//--------------------------------------------------------------------

void linear_init(LinearBuffer * const object, uint8_t * const buffer, uint16_t const size)
{
    object->data = buffer;
    object->size = size;
    linear_reset(object);
}

void linear_reset(LinearBuffer * const object)
{
    object->end = 0;
    memset(object->data, 0, object->size);
}

uint16_t linear_getOccupiedRoom(LinearBuffer const * const object)
{
    return object->end;
}

uint16_t linear_getFreeRoom(LinearBuffer const * const object)
{
    return object->size - object->end;
}

bool linear_appendByte(LinearBuffer * const object, uint8_t const byte)
{
    if(linear_getFreeRoom(object))
    {
        object->data[object->end++] = byte;
        return true;
    }
    else
    {
        return false;
    }
}

bool linear_getLastByte(LinearBuffer * const object, uint8_t * const byte)
{
    if(linear_getOccupiedRoom(object))
    {
        *byte = object->data[--object->end];
        return true;
    }
    else
    {
        return false;
    }
}

//--------------------------------------------------------------------
//             CIRCULAR BUFFER
//--------------------------------------------------------------------

void circular_init(CircularBuffer * const object, uint8_t * const buffer, uint16_t const size)
{
    object->data = buffer;
    if(size < 2 )
         object->size = 2;
    else
        object->size = size;
        
    circular_reset(object);
}

void circular_reset(CircularBuffer * const object)
{
    object->start = 0;
    object->end = 0;
    memset(object->data, 0, object->size);
}

uint16_t circular_getOccupiedRoom(CircularBuffer const * const object)
{
    return ((int32_t)(object->end) - (int32_t)(object->start)) % object->size;
}

uint16_t  circular_getFreeRoom(CircularBuffer const * const object)
{
    //-1 because there is always 1 byte you cannot use to separate head from tail
    return object->size - circular_getOccupiedRoom(object) - 1;
}

bool circular_appendByte(CircularBuffer * const object, uint8_t const byte)
{
    if(circular_getFreeRoom(object))
    {
        object->data[object->end] = byte;
        object->end = (object->end + 1) % object->size;
        return true;
    }
    else
        return false;
}

bool circular_popByte(CircularBuffer * const object, uint8_t * const byte)
{
    if(circular_getOccupiedRoom(object))
    {
        *byte = object->data[object->start];
        object->start = (object->start + 1) % object->size;
        return true;
    }
    else
        return false;
}

//return true if you can read sizeToRead until the end of the buffer
bool private_canIReadContigousData(CircularBuffer const * const object, uint16_t sizeToRead)
{
	uint16_t endOfRead = object->start + sizeToRead;

	//detection d'overflow dans le int :
	if( endOfRead < object->start || endOfRead < sizeToRead )
	{
		return false;
	}


	return endOfRead < object->size ? true : false;
}

//return true if you can write sizeToRead until the end of the buffer
bool private_canIWriteContigousData(CircularBuffer const * const object, uint16_t sizeToWrite)
{
	uint16_t endOfWrite = object->end + sizeToWrite;

	//detection d'overflow dans le int :
	if( endOfWrite < object->end || endOfWrite < sizeToWrite )
	{
		return false;
	}


	return endOfWrite < object->size ? true : false;
}


//--------------------------------------------------------------------
//             BUFFER OPERATIONS
//--------------------------------------------------------------------


bool buffer_move_cir2lin(LinearBuffer* const dest, CircularBuffer* const source, uint16_t const sizeToMove)
{
    //If sizeToMove is null move the complete buffer
    uint16_t bytesToMove = sizeToMove;
    if(bytesToMove == 0)
    {
        bytesToMove = circular_getOccupiedRoom(source);
    }

    //if target buffer is not big enougth to hold the data, just move what fits
    if( linear_getFreeRoom(dest) < bytesToMove)
        bytesToMove = linear_getFreeRoom(dest);

    //move data
    if( !buffer_copy_cir2raw(dest->data + dest->end, source, bytesToMove))
        return false;

    //update destination counters
    dest->end += bytesToMove;

    return true;
}

bool buffer_move_lin2cir(CircularBuffer* const  dest,     LinearBuffer* const   source,    uint16_t const sizeToCopy)
{
    //If sizeToMove is null move the complete buffer
    uint16_t bytesToMove = sizeToCopy;
    if(bytesToMove == 0)
    {
        bytesToMove = linear_getOccupiedRoom(source);
    }

    //if target buffer is not big enougth to hold the data, just move what fits
    if( circular_getFreeRoom(dest) < bytesToMove)
        bytesToMove = circular_getFreeRoom(dest);

    //move data
    if( !buffer_copy_raw2cir(dest, source->data, bytesToMove))
        return false;

    //update destination coutners
    dest->end -= bytesToMove;

    return true;
}

bool buffer_copy_cir2raw(uint8_t* const dest, CircularBuffer * const source, uint16_t const sizeToMove)
{
    if(!sizeToMove)
        return false;

    if( circular_getOccupiedRoom(source) < sizeToMove )
        return false;

    if( private_canIReadContigousData(source, sizeToMove) )
    {
        // message deja contigu en memoire
        memcpy(dest, source->data + source->start, sizeToMove);
    }
    else
    {
        uint16_t roomToEnd = source->size - source->start;
        //une partie est mise a la fin
        memcpy(dest, source->data + source->start, roomToEnd);
        //la suite au debut
        memcpy(dest + roomToEnd, source->data, sizeToMove - roomToEnd);
    }

    source->start = (source->start + sizeToMove) % source-> size;
    return true;
}

bool buffer_copy_raw2cir(CircularBuffer* const  dest,  uint8_t const* const  source,    uint16_t const sizeToCopy)
{
    if(!sizeToCopy)
        return false;

	if( circular_getFreeRoom(dest) < sizeToCopy )
		return false;

	if( private_canIWriteContigousData(dest, sizeToCopy) )
	{
		// message deja contigu en memoire
		memcpy(dest->data + dest->end, source, sizeToCopy);
	}
	else
	{
		uint16_t roomToEnd = dest->size - dest->end;
		//une partie est mise a la fin
		memcpy(dest->data + dest->end, source, roomToEnd);
		//la suite au debut
		memcpy(dest->data, source + roomToEnd, sizeToCopy - roomToEnd);
	}

	dest->end = (dest->end + sizeToCopy) % dest-> size;
	return true;
}





