#include "BSP.hpp"

//4 mini
#define MEM_SIZE 4

bool UT_buffer_tools_Linear()
{
	bool res = true;
	
	uint8_t memory[MEM_SIZE];
	memset(memory, 1, MEM_SIZE);
	LinearBuffer buffer;
	linear_init(&buffer, memory, MEM_SIZE);
	
	//Check initial condition
	for(unsigned int i = 0; i < MEM_SIZE ; i++)
	{
	    res &= memory[i] == 0;
	}
	res &= linear_getOccupiedRoom(&buffer) == 0;
	res &= linear_getFreeRoom(&buffer) == MEM_SIZE;

	//Append bytes when room available
	for(unsigned int i = 0 ; i < MEM_SIZE ; i++)
	{
	    res &= linear_appendByte(&buffer, i+1);
	    res &= memory[i] == i+1;
	    res &= linear_getOccupiedRoom(&buffer) == i+1;
	    res &= linear_getFreeRoom(&buffer) == MEM_SIZE-i-1;
	}

	//Append a byte in a full buffer
	res &= !linear_appendByte(&buffer, MEM_SIZE);
    res &= linear_getOccupiedRoom(&buffer) == MEM_SIZE;
    res &= linear_getFreeRoom(&buffer) == 0;
    for(unsigned int i = 0 ; i < MEM_SIZE ; i++)
    {
        res &= memory[i] == i+1;
    }

    //Pop bytes when bytes present
    for(int i = MEM_SIZE-1 ; 0 <= i ; i--)
    {
        uint8_t byte = 123;
        res &= linear_getLastByte(&buffer, &byte);
        res &= byte == memory[i];
        res &= byte == i+1;
        res &= linear_getOccupiedRoom(&buffer) == (unsigned int) i;
        res &= linear_getFreeRoom(&buffer) == (unsigned int)(MEM_SIZE-i);
    }

    //Pop a byte in an empty buffer
    {
        uint8_t byte = 123;
        res &= !linear_getLastByte(&buffer, &byte);
        res &= byte == 123;
        res &= linear_getOccupiedRoom(&buffer) == 0;
        res &= linear_getFreeRoom(&buffer) == MEM_SIZE;
        for(unsigned int i = 0 ; i < MEM_SIZE ; i++)
        {
            res &= memory[i] == i+1;
        }
    }

	return res;
}

bool UT_buffer_tools_CircularTooLittle()
{
    bool res = true;

    uint8_t memory[2];
    memset(memory, 1, MEM_SIZE);
    CircularBuffer buffer;
    circular_init(&buffer, memory, 0);
    res &= circular_getOccupiedRoom(&buffer) == 0;
    res &= circular_getFreeRoom(&buffer) == 1;

    circular_init(&buffer, memory, 1);
    res &= circular_getOccupiedRoom(&buffer) == 0;
    res &= circular_getFreeRoom(&buffer) == 1;

    return res;
}

bool UT_buffer_tools_Circular()
{
	bool res = true;

    uint8_t memory[MEM_SIZE];
    memset(memory, 1, MEM_SIZE);
    CircularBuffer buffer;
    circular_init(&buffer, memory, MEM_SIZE);

    //Check initial condition
    for(unsigned int i = 0; i < MEM_SIZE ; i++)
    {
        res &= memory[i] == 0;
    }
    res &= circular_getOccupiedRoom(&buffer) == 0;
    res &= circular_getFreeRoom(&buffer) == MEM_SIZE - 1;

    //Append bytes when room available
    for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
    {
        res &= circular_appendByte(&buffer, i+1);
        res &= memory[i] == i+1;
        res &= circular_getOccupiedRoom(&buffer) == i+1;
        res &= circular_getFreeRoom(&buffer) == MEM_SIZE-i-2;
    }

    //Append a byte in a full buffer
    res &= !circular_appendByte(&buffer, MEM_SIZE);
    res &= circular_getOccupiedRoom(&buffer) == MEM_SIZE-1;
    res &= circular_getFreeRoom(&buffer) == 0;
    for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
    {
        res &= memory[i] == i+1;
    }
    //the last byte is untouched to keep the head/tail separated
    res &= memory[MEM_SIZE-1] == 0;

    //Pop bytes when bytes present
    for(unsigned int i = 0 ; i < MEM_SIZE - 1 ; i++)
    {
        uint8_t byte = 123;
        res &=circular_popByte(&buffer, &byte);
        res &= byte == memory[i];
        res &= byte == i+1;
        res &= circular_getOccupiedRoom(&buffer) == MEM_SIZE-i-2;
        res &= circular_getFreeRoom(&buffer) == i+1;
    }

    //Pop a byte in an empty buffer
    {
        uint8_t byte = 123;
        res &= !circular_popByte(&buffer, &byte);
        res &= byte == 123;
        res &= circular_getOccupiedRoom(&buffer) == 0;
        res &= circular_getFreeRoom(&buffer) == MEM_SIZE-1;
        //data is not cleaned for performance reasons
        for(int i = 0 ; i < MEM_SIZE-1 ; i++)
        {
            res &= memory[i] == i+1;
        }
        res &= memory[MEM_SIZE-1] == 0;
    }

    //Append/Pop bytes in loop
    circular_reset(&buffer);
    for(unsigned int i = 0 ; i < 2*MEM_SIZE ; i++)
    {
        uint8_t byte = 123;

        res &= circular_appendByte(&buffer, i+1);
        res &= circular_getOccupiedRoom(&buffer) == 1;
        res &= circular_getFreeRoom(&buffer) == MEM_SIZE-2;

        res &= circular_popByte(&buffer, &byte);
        res &= memory[i%MEM_SIZE] == i+1;
        res &= circular_getOccupiedRoom(&buffer) == 0;
        res &= circular_getFreeRoom(&buffer) == MEM_SIZE-1;
    }

	return res;
}

bool UT_buffer_tools_Copy_raw2cir()
{
    bool res = true;

    uint8_t memory[MEM_SIZE];
    uint8_t raw[MEM_SIZE-1];
    CircularBuffer buffer;
    circular_init(&buffer, memory, MEM_SIZE);
    for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
    {
        raw[i] = i+1;
    }

    //Move 0 elements : no operation is done on buffer
    res &= !buffer_copy_raw2cir(&buffer, raw, 0);
    res &= circular_getOccupiedRoom(&buffer) == 0;
    res &= circular_getFreeRoom(&buffer) == MEM_SIZE - 1;
    for(unsigned int i = 0; i < MEM_SIZE ; i++)
    {
        res &= memory[i] == 0;
    }
    for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
    {
        res &= raw[i] == i+1;
    }
    circular_reset(&buffer);

    //Move one element
    res &= buffer_copy_raw2cir(&buffer, raw, 1);
    res &= circular_getOccupiedRoom(&buffer) == 1;
    res &= circular_getFreeRoom(&buffer) == MEM_SIZE - 2;
    res &= memory[0] == 1;
    for(unsigned int i = 1; i < MEM_SIZE ; i++)
    {
        res &= memory[i] == 0;
    }
    for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
    {
        res &= raw[i] == i+1;
    }
    circular_reset(&buffer);

    //Move max - 1 contiguous elements  (note that max - 1 is (MEM_SIZE-1) - 1)
    res &= buffer_copy_raw2cir(&buffer, raw, MEM_SIZE-2);
    res &= circular_getOccupiedRoom(&buffer) == MEM_SIZE-2;
    res &= circular_getFreeRoom(&buffer) == 1;
    for(unsigned int i = 0; i < MEM_SIZE-2 ; i++)
    {
        res &= memory[i] == i+1;
    }
    for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
    {
        res &= raw[i] == i+1;
    }
    res &= memory[MEM_SIZE-2] == 0; //index we didn't fill on purpose
    res &= memory[MEM_SIZE-1] == 0; //index we can't fill to allow a separation between head/tail of circular buffer
    circular_reset(&buffer);

    //Move max contiguous elements (note that max = MEM_SIZE-1)
    res &= buffer_copy_raw2cir(&buffer, raw, MEM_SIZE-1);
    res &= circular_getOccupiedRoom(&buffer) == MEM_SIZE-1;
    res &= circular_getFreeRoom(&buffer) == 0;
    for(unsigned int i = 0; i < MEM_SIZE-1 ; i++)
    {
        res &= memory[i] == i+1;
    }
    for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
    {
        res &= raw[i] == i+1;
    }
    res &= memory[MEM_SIZE-1] == 0; //index we can't fill to allow a separation between head/tail of circular buffer
    circular_reset(&buffer);

    //Move max + 1 contiguous elements : no operation is done on buffer (note that max +1 = (MEM_SIZE-1)+1)
    res &= !buffer_copy_raw2cir(&buffer, raw, MEM_SIZE);
    res &= circular_getOccupiedRoom(&buffer) == 0;
    res &= circular_getFreeRoom(&buffer) == MEM_SIZE-1;
    for(unsigned int i = 0; i < MEM_SIZE ; i++)
    {
        res &= memory[i] == 0;
    }
    for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
    {
        res &= raw[i] == i+1;
    }
    circular_reset(&buffer);

    //Move max - 1 discontiguous elements  (note that max - 1 is (MEM_SIZE-1) - 1)
    {
        uint8_t byte = 0;
        
        //move buffer head/tail ahead
        res &= circular_appendByte(&buffer, byte);
        res &= circular_popByte(&buffer, &byte);
        res &= circular_appendByte(&buffer, byte);
        res &= circular_popByte(&buffer, &byte);
        
        res &= buffer_copy_raw2cir(&buffer, raw, MEM_SIZE-2);
        res &= circular_getOccupiedRoom(&buffer) == MEM_SIZE-2;
        res &= circular_getFreeRoom(&buffer) == 1;
        for(unsigned int i = 2; i < MEM_SIZE ; i++)
        {
            res &= memory[i] == i-1;
        }
        for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
        {
            res &= raw[i] == i+1;
        }
        res &= memory[0] == 0; //index we didn't fill on purpose
        res &= memory[1] == 0; //index we can't fill to allow a separation between head/tail of circular buffer
        circular_reset(&buffer);
    }        

    //Move max discontiguous elements (note that max = MEM_SIZE-1)
    {
        uint8_t byte = 0;
        
        //move buffer head/tail ahead
        res &= circular_appendByte(&buffer, byte);
        res &= circular_popByte(&buffer, &byte);
        res &= circular_appendByte(&buffer, byte);
        res &= circular_popByte(&buffer, &byte);
        
        res &= buffer_copy_raw2cir(&buffer, raw, MEM_SIZE-1);
        res &= circular_getOccupiedRoom(&buffer) == MEM_SIZE-1;
        res &= circular_getFreeRoom(&buffer) == 0;
        for(unsigned int i = 2; i < MEM_SIZE ; i++)
        {
            res &= memory[i] == i-1;
        }
        for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
        {
            res &= raw[i] == i+1;
        }
        res &= memory[0] == MEM_SIZE-1; 
        res &= memory[1] == 0; //index we can't fill to allow a separation between head/tail of circular buffer
        circular_reset(&buffer); 
    }

    //Move max + 1 discontiguous elements : no operation is done on buffer (note that max +1 = (MEM_SIZE-1)+1)
        //move buffer head/tail ahead
    for(unsigned int i = 0; i < 2 ; i++)
    {
        uint8_t byte = 0;
        res &=circular_appendByte(&buffer, byte);
        res &=circular_popByte(&buffer, &byte);
    }

    res &= !buffer_copy_raw2cir(&buffer, raw, MEM_SIZE);
    res &= circular_getOccupiedRoom(&buffer) == 0;
    res &= circular_getFreeRoom(&buffer) == MEM_SIZE-1;
    for(unsigned int i = 0; i < MEM_SIZE ; i++)
    {
        res &= memory[i] == 0;
    }
    for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
    {
        res &= raw[i] == i+1;
    }
    circular_reset(&buffer);
    
    return res;
}

bool UT_buffer_tools_Copy_cir2raw()
{
    bool res = true;

    uint8_t memory[MEM_SIZE];
    uint8_t raw[MEM_SIZE-1];
    CircularBuffer buffer;
    circular_init(&buffer, memory, MEM_SIZE);
    memset(raw, 0, MEM_SIZE-1);

    //Move 0 elements : no operation is done on an empty circular buffer
    res &= !buffer_copy_cir2raw(raw, &buffer, 0);
    res &= circular_getOccupiedRoom(&buffer) == 0;
    res &= circular_getFreeRoom(&buffer) == MEM_SIZE - 1;
    for(unsigned int i = 0; i < MEM_SIZE ; i++)
    {
        res &= memory[i] == 0;
    }
    for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
    {
        res &= raw[i] == 0;
    }

    //Move 0 elements : no operation is done on a non-empty circular buffer
    res &=circular_appendByte(&buffer, 1);
    res &=circular_appendByte(&buffer, 2);
    res &= !buffer_copy_cir2raw(raw, &buffer, 0);
    res &=circular_getOccupiedRoom(&buffer) == 2;
    res &=circular_getFreeRoom(&buffer) == MEM_SIZE - 3;
    res &= memory[0] == 1;
    res &= memory[1] == 2;
    for(unsigned int i = 2; i < MEM_SIZE ; i++)
    {
        res &= memory[i] == 0;
    }
    for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
    {
        res &= raw[i] == 0;
    }

    //Move one element
    res &= buffer_copy_cir2raw(raw, &buffer, 1);
    res &= circular_getOccupiedRoom(&buffer) == 1;
    res &= circular_getFreeRoom(&buffer) == MEM_SIZE - 2;
    res &= memory[0] == 1;
    res &= memory[1] == 2;
    for(unsigned int i = 2; i < MEM_SIZE ; i++)
    {
        res &= memory[i] == 0;
    }
    res &= raw[0] == 1;
    for(unsigned int i = 1 ; i < MEM_SIZE-1 ; i++)
    {
        res &= raw[i] == 0;
    }
    //Move a second element
    res &= buffer_copy_cir2raw(raw, &buffer, 1);
    res &=circular_getOccupiedRoom(&buffer) == 0;
    res &=circular_getFreeRoom(&buffer) == MEM_SIZE - 1;
    res &= memory[0] == 1;
    res &= memory[1] == 2;
    for(unsigned int i = 2; i < MEM_SIZE ; i++)
    {
        res &= memory[i] == 0;
    }
    res &= raw[0] == 2;
    for(unsigned int i = 1 ; i < MEM_SIZE-1 ; i++)
    {
        res &= raw[i] == 0;
    }
    //Try to move one element, since there is no data in circular buffer
    memset(raw, 0, MEM_SIZE-1);
    res &= !buffer_copy_cir2raw(raw, &buffer, 1);
    res &=circular_getOccupiedRoom(&buffer) == 0;
    res &=circular_getFreeRoom(&buffer) == MEM_SIZE - 1;
    res &= memory[0] == 1;
    res &= memory[1] == 2;
    for(unsigned int i = 2; i < MEM_SIZE ; i++)
    {
        res &= memory[i] == 0;
    }
    for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
    {
        res &= raw[i] == 0;
    }
    circular_reset(&buffer);

    //Move a filled circular buffer with no split data in circular buffer
    for(unsigned int i = 0; i < MEM_SIZE-1 ; i++)
    {
        res &=circular_appendByte(&buffer, i+1);
    }
    res &= buffer_copy_cir2raw(raw, &buffer, MEM_SIZE-1);
    res &= circular_getOccupiedRoom(&buffer) == 0;
    res &= circular_getFreeRoom(&buffer) == MEM_SIZE - 1;
    for(unsigned int i = 0; i < MEM_SIZE-1 ; i++)
    {
        res &= memory[i] == i+1;
    }
    res &= memory[MEM_SIZE-1] == 0;
    for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
    {
        res &= raw[i] == i+1;
    }
    circular_reset(&buffer);

    //Move a filled circular buffer with data split in circular buffer
    for(unsigned int i = 0; i < 2 ; i++)
    {
        uint8_t byte = 0;
        res &=circular_appendByte(&buffer, byte);
        res &=circular_popByte(&buffer, &byte);
    }
    for(unsigned int i = 0; i < MEM_SIZE-1 ; i++)
    {
        res &=circular_appendByte(&buffer, i+1);
    }
    res &= buffer_copy_cir2raw(raw, &buffer, MEM_SIZE-1);
    res &= circular_getOccupiedRoom(&buffer) == 0;
    res &= circular_getFreeRoom(&buffer) == MEM_SIZE - 1;
    res &= memory[0] == MEM_SIZE-1;
    res &= memory[1] == 0;
    for(unsigned int i = 0; i < MEM_SIZE-1 ; i++)
    {
        res &= memory[(i+2)%MEM_SIZE] == i+1;
    }
    for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
    {
        res &= raw[i] == i+1;
    }
    circular_reset(&buffer);

    //Move one byte in a filled circular buffer with no split data in circular buffer
    for(unsigned int i = 0; i < MEM_SIZE-1 ; i++)
    {
        res &=circular_appendByte(&buffer, i+1);
    }
    res &= buffer_copy_cir2raw(raw, &buffer,1);
    res &= circular_getOccupiedRoom(&buffer) == MEM_SIZE-2;
    res &= circular_getFreeRoom(&buffer) == 1;
    for(unsigned int i = 0; i < MEM_SIZE-1 ; i++)
    {
        res &= memory[i] == i+1;
    }
    res &= memory[MEM_SIZE-1] == 0;
    for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
    {
        res &= raw[i] == i+1;
    }
    circular_reset(&buffer);

    //Move one byte in a filled circular buffer with data split in circular buffer
    for(unsigned int i = 0; i < 2 ; i++)
    {
        uint8_t byte = 0;
        res &=circular_appendByte(&buffer, byte);
        res &=circular_popByte(&buffer, &byte);
    }
    for(unsigned int i = 0; i < MEM_SIZE-1 ; i++)
    {
        res &=circular_appendByte(&buffer, i+1);
    }
    res &= buffer_copy_cir2raw(raw, &buffer,1);
    res &= circular_getOccupiedRoom(&buffer) == MEM_SIZE-2;
    res &= circular_getFreeRoom(&buffer) == 1;
    res &= memory[0] == MEM_SIZE-1;
    res &= memory[1] == 0;
    for(unsigned int i = 0; i < MEM_SIZE-1 ; i++)
    {
        res &= memory[(i+2)%MEM_SIZE] == i+1;
    }
    for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
    {
        res &= raw[i] == i+1;
    }
    circular_reset(&buffer);

    return res;
}

bool UT_buffer_tools_Move()
{
    bool res = true;

    uint8_t memoryCir[MEM_SIZE];
    uint8_t memoryLin[MEM_SIZE-1];
    CircularBuffer bufferCir;
    LinearBuffer bufferLin;
    circular_init(&bufferCir, memoryCir, MEM_SIZE);
    linear_init(&bufferLin, memoryLin, MEM_SIZE-1);

    //copy the full buffer from circular to linear
    for(unsigned int i = 0; i < MEM_SIZE-1 ; i++)
    {
        res &=circular_appendByte(&bufferCir, i+1);
    }
    res &= buffer_move_cir2lin(&bufferLin, &bufferCir, 0);
    res &= circular_getOccupiedRoom(&bufferCir) == 0;
    res &= circular_getFreeRoom(&bufferCir) == MEM_SIZE-1;
    res &= linear_getOccupiedRoom(&bufferLin) == MEM_SIZE-1;
    res &= linear_getFreeRoom(&bufferLin) == 0;
    for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
    {
        res &= memoryLin[i] == i+1;
    }
    circular_reset(&bufferCir);

    //copy back the full buffer from circular to linear
    res &= buffer_move_lin2cir(&bufferCir, &bufferLin, 0);
    res &= circular_getOccupiedRoom(&bufferCir) == MEM_SIZE-1;
    res &= circular_getFreeRoom(&bufferCir) == 0;
    res &= linear_getOccupiedRoom(&bufferLin) == 0;
    res &= linear_getFreeRoom(&bufferLin) == MEM_SIZE-1;


    //copy the full buffer from circular to linear in 2 pass
    res &= buffer_move_cir2lin(&bufferLin, &bufferCir, MEM_SIZE/2);
    res &= circular_getFreeRoom(&bufferCir) == MEM_SIZE/2;
    res &= linear_getOccupiedRoom(&bufferLin) == MEM_SIZE/2 ;
    for(unsigned int i = 0 ; i < MEM_SIZE/2 ; i++)
    {
        res &= memoryLin[i] == i+1;
    }
    res &= buffer_move_cir2lin(&bufferLin, &bufferCir, MEM_SIZE - 1 - MEM_SIZE/2);
    res &= circular_getOccupiedRoom(&bufferCir) == 0;
    res &= linear_getFreeRoom(&bufferLin) == 0 ;
    for(unsigned int i = 0 ; i < MEM_SIZE - 1 ; i++)
    {
        res &= memoryLin[i] == i+1;
    }
    circular_reset(&bufferCir);

    //copy back the full buffer from circular to linear in 2 pass
    res &= buffer_move_lin2cir(&bufferCir, &bufferLin, MEM_SIZE/2);
    res &= linear_getFreeRoom(&bufferLin) == MEM_SIZE/2;
    res &= circular_getOccupiedRoom(&bufferCir) == MEM_SIZE/2;
    for(unsigned int i = 0 ; i < MEM_SIZE/2 ; i++)
    {
        res &= memoryCir[i] == i+1;
    }
    res &= buffer_move_lin2cir(&bufferCir, &bufferLin, MEM_SIZE - 1 - MEM_SIZE/2);
    res &= linear_getOccupiedRoom(&bufferLin) == 0;
    res &= circular_getFreeRoom(&bufferCir) == 0;
    for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
    {
        res &= memoryCir[i] == i+1;
    }

    //Copy a bigger circular buffer in a little linear
    circular_reset(&bufferCir);
    linear_reset(&bufferLin);
    uint8_t memoryCir2[2*MEM_SIZE];
    uint8_t memoryLin2[2*MEM_SIZE];
    CircularBuffer bufferCir2;
    LinearBuffer bufferLin2;
    circular_init(&bufferCir2, memoryCir2, 2*MEM_SIZE);
    linear_init(&bufferLin2, memoryLin2, 2*MEM_SIZE);

    for(unsigned int i = 0; i < 2*MEM_SIZE-1 ; i++)
    {
        res &=circular_appendByte(&bufferCir2, i+1);
    }
        //first pass
    res &= buffer_move_cir2lin(&bufferLin, &bufferCir2, MEM_SIZE+2);
    res &= linear_getOccupiedRoom(&bufferLin) == MEM_SIZE-1;
    res &= circular_getFreeRoom(&bufferCir2) == MEM_SIZE-1;
    for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
    {
        res &= memoryLin[i] == i+1;
    }
        //second pass
    linear_reset(&bufferLin);
    res &= buffer_move_cir2lin(&bufferLin, &bufferCir2, MEM_SIZE+2);
    res &= linear_getOccupiedRoom(&bufferLin) == 3;
    res &= circular_getFreeRoom(&bufferCir2) == MEM_SIZE+2;
    for(unsigned int i = 0 ; i < MEM_SIZE - 1 ; i++)
    {
        if( i < 3 )
            res &= memoryLin[i] == MEM_SIZE + i;
        else
            res &= memoryLin[i] == 0;
    }

    //Copy a bigger linear buffer in a little circular
    circular_reset(&bufferCir);
    for(unsigned int i = 0; i < 2*MEM_SIZE ; i++)
    {
        res &=linear_appendByte(&bufferLin2, i+1);
    }
        //first pass
    res &= buffer_move_lin2cir(&bufferCir, &bufferLin2, MEM_SIZE+2);
    res &= circular_getOccupiedRoom(&bufferCir) == MEM_SIZE-1;
    res &= linear_getFreeRoom(&bufferLin2) == MEM_SIZE-1;
    for(unsigned int i = 0 ; i < MEM_SIZE-1 ; i++)
    {
     res &= memoryLin[i] == i+1;
    }
        //second pass
    circular_reset(&bufferCir);
    res &= buffer_move_lin2cir(&bufferCir, &bufferLin2, MEM_SIZE+2);
    res &= circular_getOccupiedRoom(&bufferCir) == 3;
    res &= linear_getFreeRoom(&bufferLin2) == MEM_SIZE+2;

    return res;
}

bool UT_buffer_tools()
{
	bool res = true;

	res &= UT_buffer_tools_Linear();
	res &= UT_buffer_tools_CircularTooLittle();
	res &= UT_buffer_tools_Circular();
	res &= UT_buffer_tools_Copy_raw2cir();
	res &= UT_buffer_tools_Copy_cir2raw();
	res &= UT_buffer_tools_Move();

	return res;
}

