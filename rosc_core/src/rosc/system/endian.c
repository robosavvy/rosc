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

static endian_t local_byte_order_correction_to_system;
static endian_t local_byte_order_correction_to_network;


const endian_t* const g_byte_order_correction_to_system = &local_byte_order_correction_to_system;
const endian_t* const g_byte_order_correction_to_network = &local_byte_order_correction_to_network;

static endian_t pattern_local =
{
{ 0x0201 },
{ 0x04030201 },
{ 0x0807060504030201 }, }; //Bytes are numbered 1-X to find out if there is something strange on the current platform



void rosc_init_endian(void)
{
	if (sizeof(uint8_t) != 1 && sizeof(uint16_t) != 2 && sizeof(uint32_t) != 4
			&& sizeof(uint64_t) != 8)
	{
		ROSC_FATAL("One or more data types do not have the expected byte size!");
	}



	uint8_t size;
	int8_t *system_byte_order;
	int8_t *byte_order_2_system_array;
	int8_t *byte_order_2_network_array;
	for (size = 2; size <= 8; size <<= 1)
	{
		switch(size)
		{
			case 2:
				system_byte_order = pattern_local.SIZE_2_B;
				byte_order_2_system_array = local_byte_order_correction_to_system.SIZE_2_B;
				byte_order_2_network_array = local_byte_order_correction_to_network.SIZE_2_B;
				break;
			case 4:
				system_byte_order = pattern_local.SIZE_4_B;
				byte_order_2_system_array = local_byte_order_correction_to_system.SIZE_4_B;
				byte_order_2_network_array = local_byte_order_correction_to_network.SIZE_4_B;
				break;
			case 8:
				system_byte_order = pattern_local.SIZE_8_B;
				byte_order_2_system_array = local_byte_order_correction_to_system.SIZE_8_B;
				byte_order_2_network_array = local_byte_order_correction_to_network.SIZE_8_B;
				break;
			default:
				ROSC_FATAL("ENDIAN: Unexpected state in rosc_init_endian size switch!");
				break;
		}

		int i, j;
		DEBUG_PRINT(INT,"Size",size);
		for (i = 0; i < size; ++i)
		{
			//Seeking the current byte position
			bool found = false;
			for (j = 0; j < size; ++j)
			{
				if (system_byte_order[j] == i+1) //we search for the current byte
				{
					found = true;
					break;
				}
			}
			if (!found)
			{
				ROSC_FATAL("ENDIAN: Byte number not found!");
			}
			else
			{
				byte_order_2_system_array[i]=j-i;
				byte_order_2_network_array[i]=system_byte_order[i]-(i+1);
			}

		}

	}

}
