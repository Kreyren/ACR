/*

    ********** DO NOT REMOVE THIS INFORMATION ************

    ACR - A set of C functions in a git Repository
    Copyright (C) 2018 - 2020 Adam C. Rosenberg

    Please read LICENSE before using this code

    Please read README.md for instructions on including
    this code in your project and where to find examples

    If you contribute to this code, Please add your name
    and email address to the AUTHORS list below so that
    we know who to thank!

    AUTHORS

    Adam Rosenberg <adam@sirspot.com>

    LICENSE

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.


    ******************************************************

*/
/** \file test_buffer.c

    application to test buffer struct, macros, and functions

*/
#include "ACR/buffer.h"

//
// PROTOTYPES
//

/** simple example to use a buffer on the stack
*/
int StackTest(void);

/** simple example to use a buffer on the heap
*/
int HeapTest(void);

/** simple example to use a buffer at a low level
*/
int LowLevelTest(void);

/** a more complex test that takes into account malloc
    option and prints debug messages as the test is run
*/
int VerboseTest(void);

//
// MAIN
//

int main(int argc, char** argv)
{
	int result = ACR_SUCCESS;

	ACR_UNUSED(argc);
	ACR_UNUSED(argv);

	result |= StackTest();
	result |= HeapTest();
	result |= LowLevelTest();

	result |= VerboseTest();

	return result;
}

//
// IMPLEMENTATION
//

/**********************************************************/
int StackTest(void)
{
	int result = ACR_SUCCESS;
	ACR_Buffer_t buffer;

	ACR_BufferInit(&buffer);
	if(ACR_BufferAllocate(&buffer, 5000) == ACR_INFO_OK)
	{
		//
		// OK - 5000 byte buffer ready for use
		//

		// optional step to clear the buffer
		ACR_BufferClear(&buffer);
	}
	else
	{
		// failed to allocate 5000 bytes
		result = ACR_FAILURE;
	}
	ACR_BufferDeInit(&buffer);

	return result;
}

/**********************************************************/
int HeapTest(void)
{
	int result = ACR_SUCCESS;
	ACR_Buffer_t* bufferPtr;

	ACR_BufferNew(&bufferPtr);
	if(ACR_BufferAllocate(bufferPtr, 5000) == ACR_INFO_OK)
	{
		//
		// OK - 5000 byte buffer ready for use
		//

		// optional step to clear the buffer
		ACR_BufferClear(bufferPtr);
	}
	else
	{
		// failed to allocate 5000 bytes
		result = ACR_FAILURE;
	}
	ACR_BufferDelete(&bufferPtr);

	return result;
}

/**********************************************************/
int LowLevelTest(void)
{
	ACR_BUFFER(buffer);

	ACR_BUFFER_ALLOC(buffer, 5000);
	if(ACR_BUFFER_IS_VALID(buffer))
	{
		//
		// OK - 5000 byte buffer ready for use
		//

		// optional step to clear the buffer
		ACR_BUFFER_CLEAR(buffer);

		// done with the buffer
		ACR_BUFFER_FREE(buffer);
	}
	else
	{
		// cannot allocate 5000 bytes
		return ACR_FAILURE;
	}

	return ACR_SUCCESS;
}

/**********************************************************/
int VerboseTest(void)
{
	ACR_BUFFER(buffer);
	ACR_Length_t testAllocateBytes = 5000;

	ACR_DEBUG_PRINT(1, "TEST allocate %d bytes", (int)testAllocateBytes);
	ACR_BUFFER_ALLOC(buffer, testAllocateBytes);
	#if ACR_HAS_MALLOC == ACR_BOOL_TRUE
		if(ACR_BUFFER_GET_LENGTH(buffer) != testAllocateBytes)
		{
			ACR_DEBUG_PRINT(2, "FAIL buffer length is %d", (int)buffer.m_Length);
			return ACR_FAILURE;
		}
		else
		{
			ACR_DEBUG_PRINT(3, "PASS allocated %d bytes", (int)buffer.m_Length);
		}
	#else
		if(ACR_BUFFER_GET_LENGTH(buffer) != 0)
		{
			ACR_DEBUG_PRINT(4, "FAIL buffer length is %d", (int)buffer.m_Length);
			return ACR_FAILURE;
		}
		else
		{
			ACR_DEBUG_PRINT(5, "PASS malloc is not available");
		}
	#endif
	ACR_DEBUG_PRINT(6, "TEST free %d bytes", (int)buffer.m_Length);
	ACR_BUFFER_FREE(buffer);

	if(ACR_BUFFER_GET_LENGTH(buffer) != 0)
	{
		ACR_DEBUG_PRINT(7, "FAIL buffer length is %d", (int)buffer.m_Length);
		return ACR_FAILURE;
	}
	else
	{
		ACR_DEBUG_PRINT(8, "PASS buffer length is zero");
	}

	return ACR_SUCCESS;
}
