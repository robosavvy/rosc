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
#include <rosc/system/status.h>
#include <rosc/debug/debug_out.h>
#include <rosc/system/types.h>


static endian_t local_system_byte_order =
{
{ 0x0201 },
{ 0x04030201 },
{ 0x0807060504030201 },
};

const endian_t* const g_system_byte_order = &local_system_byte_order;

void rosc_init_endian(void)
{
	if (sizeof(uint8_t) != 1 &&
		sizeof(uint16_t) != 2 &&
		sizeof(uint32_t) != 4 &&
		sizeof(uint64_t) != 8)
	{
		ROSC_FATAL("One or more data types do not have the expected byte size!");
	}

	uint8_t size;
	int8_t *currentByteAccess;
	for(size=2;size<=8;size<<=1)
	{
		switch(size)
		{
		case 2:
			currentByteAccess=local_system_byte_order.SIZE_2_B;
			break;
		case 4:
			currentByteAccess=local_system_byte_order.SIZE_4_B;
			break;
		case 8:
			currentByteAccess=local_system_byte_order.SIZE_8_B;
			break;
		default:
			ROSC_FATAL("Unexpected state in rosc_init_endian size switch!")
			break;
		}

		int i;
		DEBUG_PRINT(INT,"Size",size);
		for(i=0;i<size;++i)
		{
			if(currentByteAccess>0 &&
			   currentByteAccess[i]<=8)
			{
				currentByteAccess[i]-=i+1;
			}
			else
			{
				ROSC_FATAL("Byte Endian information is not in the specified range");
			}
		}

	}




}
