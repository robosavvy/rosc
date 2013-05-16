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
 *  exmsg.h created by Christian Holl
 */

/*
 * Message File
 * bool   r_or_w
 * uint8  Bytes[]
 * uint16 Words[]
 */

#include <rosc/system/types.h>
#include <rosc/system/iface.h>

#ifndef EXMSG_H_
#define EXMSG_H_


//This is used to send out a message for every configuration
#pragma pack(push,1)

typedef struct
{
	bool r_or_w;

	struct
	{
	uint32_t size;
	uint8_t *data;
	}Bytes;

	struct
	{
	uint32_t size;
	uint16_t *data;
	}Words;

}examplepack_exmsg_t;

#pragma pack(pop)


//NOTE: iface.h definition
//typedef struct iface_t
//{
//	iface_type_t type;
//	char* name;	//!< This is the topic / server name of the interface
//	struct iface_t* next;
//	iface_definition_t* const def;
//	iface_state_t state;
//}iface_t;
#ifndef __SYSTEM_HAS_MALLOC__
	#define ROSC_PUBLISHER()\
			{IFACE_TYPE_TOPIC_PUBLISHER,0,}
#else
	//TODO iface creation function
#endif

#define FIXED_SIZE_EXAMPLEPACK_EXMSG_CMD_ARRAY(__STRUCTNAME,__BYTES_SIZE, __WORDS_SIZE)\
	const uint32_t examplepack_exmsg_cmd_##__STRUCTNAME##_fixed[]  = {IFACE_DEFINITION_BOOL,IFACE_DEFINITION_UNDEF_ARRAY(IFACE_DEFINITION_UINT8), __BYTES_SIZE, IFACE_DEFINITION_UNDEF_ARRAY(IFACE_DEFINITION_UINT16),__WORDS_SIZE}

#define FIXED_SIZE_EXAMPLEPACK_EXMSG_STRUCT(__STRUCTNAME,__BYTES_SIZE, __WORDS_SIZE)\
		typedef struct\
		{\
			bool r_or_w;\
			\
			struct\
			{\
			uint32_t size;\
			uint8_t data[__BYTES_SIZE];\
			}Bytes;\
			\
			struct\
			{\
			uint32_t size;\
			uint16_t data[__WORDS_SIZE];\
			}Words;\
		} examplepack_exmsg_structfixed_##__STRUCTNAME;\
		FIXED_SIZE_EXAMPLEPACK_EXMSG_CMD_ARRAY(__STRUCTNAME)

/*
 *
 * NOTE
 *
typedef struct
{
	char a;
	uint32_t b;
	uint8_t c;
	char d[5];
	int e[9];
}acme_t;

const union
{
acme_t __struct;
char __bytes[sizeof(acme_t)];
}acme_padding_identifier_array={{0xFF,0xFFFFFFFF,0xFF,{0xFF},{0xFFFFFFFF}}};
 *
 *
 * Initialize iface.
 * On arrays only the first element will be initialized
 *
 */


#endif /* EXMSG_H_ */
