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
 *  endian.h created by Christian Holl
 */

#ifndef ENDIAN_H_
#define ENDIAN_H_

#include <rosc/system/types.h>

/**
 * This type of the struct stores the byte order for each
 * common type of the system, when the values are initialized
 * with like for example 0x0807060504030201 for 8 bytes.
 */
typedef struct
{
	union
	{
		uint16_t SIZE_2;
		int8_t SIZE_2_B[sizeof(uint16_t)];
	};
	union
	{
		uint32_t SIZE_4;
		int8_t SIZE_4_B[sizeof(uint32_t)];
	};
	union
	{
		uint64_t SIZE_8;
		int8_t SIZE_8_B[sizeof(uint64_t)];
	};
} endian_t;

/**
 * Contains byte order corrections for the communication
 * byte order (little endian) to the endian format of the
 * system. To convert the adress from system to communication
 * byte order do for each byte chrptr+SIZE_X_B[byte_number]
 * and for the other direction chrptr-SIZE_X_B[byte_number]
 */
extern const endian_t* const g_system_byte_order;

/**
 * This function initializes the variable
 * which is linked to the system_byte_order.
 *
 * @TODO Make rosc_init_endian replaceable for really weired compilers...
 */
void rosc_init_endian(void);

#endif /* ENDIAN_H_ */
