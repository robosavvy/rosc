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

typedef struct
{
	union
	{
		int8_t INT8;
		char int8_t_B[sizeof(int8_t)];
	};
	union
	{
		int16_t INT16;
		char int16_t_B[sizeof(int16_t)];
	};
	union
	{
		int32_t INT32;
		char int32_t_B[sizeof(int32_t)];
	};
	union
	{
		int64_t INT64;
		char int64_t_B[sizeof(int64_t)];
	};

	union
	{
		uint8_t UINT8;
		char uint8_t_B[sizeof(uint8_t)];
	};
	union
	{
		uint16_t UINT16;
		char uint16_t_B[sizeof(uint16_t)];
	};
	union
	{
		uint32_t UINT32;
		char uint32_t_B[sizeof(uint32_t)];
	};
	union
	{
		uint64_t UINT64;
		char uint64_t_B[sizeof(uint64_t)];
	};

	union
	{
		float32_t FLOAT32;
		char float32_t_B[sizeof(float32_t)];
	};
	union
	{
		float64_t FLOAT64;
		char float64_t_B[sizeof(float64_t)];
	};

	union
	{
		bool BOOL;
		char bool_B[sizeof(bool)];
	};
} endian_t;

/**
 * Contains byte order corrections
 */
extern const endian_t* const system_byte_order;

void rosc_init_endian();

#endif /* ENDIAN_H_ */
