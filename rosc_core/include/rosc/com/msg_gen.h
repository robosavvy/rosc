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
 *  msg_gen.h created by Christian Holl
 */

#ifndef MSG_GEN_H_
#define MSG_GEN_H_

#include <rosc/system/types.h>

typedef enum
{
	MSG_GEN_TYPE_CHAR,  // char 2
	MSG_GEN_TYPE_STRING_LIST, //list / string 3
	MSG_GEN_TYPE_STRING_POINTER,
	MSG_GEN_TYPE_TAG,    //list / string 3
	MSG_GEN_TYPE_CLOSE_TAG, //list /string 3
	MSG_GEN_TYPE_ROSRPC_FIELD, //list / string / string 4
	MSG_GEN_TYPE_URL,// ipaddr / ipaddr / ipaddr/ ipaddr / port(2) 7
	MSG_GEN_TYPE_FLOAT, // size
	MSG_GEN_TYPE_INT,  // size
	MSG_GEN_TYPE_UINT, //size
}msg_gen_type_t;

typedef struct
{
	 const char **cmd_spec_str;

}msg_gen_command;


typedef struct
{
	msg_gen_type_t type;
	union
	{
		char chr;

		char *string;

		struct
		{
			uint8_t list;
			uint8_t entry;
		}string_list;

		struct
		{
			bool string_list;
			union
			{
				char *string;
				struct
				{
					uint8_t list;
					uint8_t entry;
				}string_list;
			};
		}tag;

		struct
		{
			bool string_list;
			union
			{
				char *string[2];
				struct
				{
					uint8_t list;
					uint8_t entry;
				}string_list[2];
			};
			uint16_t port;
		}rpc_field;
	};
}msg_gen_command_t;

uint32_t msg_gen(const char *** const char_arrays);





#endif /* MSG_GEN_H_ */
