/*
 *	Copyright (c) 2013, Synapticon GmbH
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions are met: 
 *
 *	1. Redistributions of source code must retain the above copyright notice, this
 *	   list of conditions and the following disclaimer. 
 *	2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution. 
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *	ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *	The views and conclusions contained in the software and documentation are those
 *	of the authors and should not be interpreted as representing official policies, 
 *	either expressed or implied, of the FreeBSD Project.
 *
 *  endian.c created by Christian Holl
 */

#include <rosc/system/endian.h>

typedef enum
{
	ENDIAN_INT8,
	ENDIAN_INT16,
	ENDIAN_INT32,
	ENDIAN_INT64,
	ENDIAN_UINT8,
	ENDIAN_UINT16,
	ENDIAN_UINT32,
	ENDIAN_UINT64,
	ENDIAN_FLOAT32,
	ENDIAN_FLOAT64,
	ENDIAN_BOOL,
	ENDIAN_END,
} endian_init_state_t;

static endian_t __system_byte_order =
{
{ 0x01 },
{ 0x0201 },
{ 0x04030201 },
{ 0x0807060504030201 },

{ 0x01 },
{ 0x0201 },
{ 0x04030201 },
{ 0x0807060504030201 },

{ 0x7ffffffb },
{ 0x0807060504030201 },

{ 0x01 }, };

const endian_t* const system_byte_order = &__system_byte_order;

void rosc_init_endian()
{
	if (sizeof(char) != 1)
	{

		while (1)
			;
	}

	int i;
	signed char *currentByteAccess;
	unsigned int currentByteSize;
	endian_init_state_t state;

	for (state = ENDIAN_INT8; state < ENDIAN_END; ++state)
	{
		switch (state)
		{
		case ENDIAN_INT8:
			currentByteAccess=__system_byte_order.int8_t_B;
			currentByteSize=sizeof(int8_t);
			break;
		case ENDIAN_INT16:
			currentByteAccess=__system_byte_order.int16_t_B;
			currentByteSize=sizeof(int16_t);
			break;
		case ENDIAN_INT32:
			currentByteAccess=__system_byte_order.int32_t_B;
			currentByteSize=sizeof(int32_t);
			break;
		case ENDIAN_INT64:
			currentByteAccess=__system_byte_order.int64_t_B;
			currentByteSize=sizeof(int64_t);
			break;
		case ENDIAN_UINT8:
			currentByteAccess=__system_byte_order.uint8_t_B;
			currentByteSize=sizeof(uint8_t);
			break;
		case ENDIAN_UINT16:
			currentByteAccess=__system_byte_order.uint16_t_B;
			currentByteSize=sizeof(uint16_t);
			break;
		case ENDIAN_UINT32:
			currentByteAccess=__system_byte_order.uint32_t_B;
			currentByteSize=sizeof(uint32_t);
			break;
		case ENDIAN_UINT64:
			currentByteAccess=__system_byte_order.uint64_t_B;
			currentByteSize=sizeof(uint64_t);
			break;

		case ENDIAN_FLOAT32:
			currentByteAccess=__system_byte_order.float32_t_B;
			currentByteSize=sizeof(float32_t);
			break;
		case ENDIAN_FLOAT64:
			currentByteAccess=__system_byte_order.float64_t_B;
			currentByteSize=sizeof(float64_t);
			break;

		case ENDIAN_BOOL:
			currentByteAccess=__system_byte_order.bool_B;
			currentByteSize=sizeof(bool);
			break;

		case ENDIAN_END:
		default:
			currentByteSize=0;

			break;
		}

		for(i=0;i<currentByteSize;++i)
		{
			printf("%i ",(signed int)currentByteAccess[i]);
		}
		printf("\n");
	}
}
