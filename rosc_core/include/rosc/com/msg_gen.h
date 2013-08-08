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
#include <rosc/system/ports.h>

typedef enum
{
	MSG_GEN_TYPE_WHOLE_LEN_BIN,
	MSG_GEN_TYPE_WHOLE_LEN_STRING,
	MSG_GEN_TYPE_CHAR,
	MSG_GEN_TYPE_STRING,
	MSG_GEN_TYPE_TAG,
	MSG_GEN_TYPE_CLOSE_TAG,
	MSG_GEN_TYPE_ROSRPC_FIELD,
	MSG_GEN_TYPE_BOOL,
	MSG_GEN_TYPE_URL,
	MSG_GEN_TYPE_FLOAT32,
	MSG_GEN_TYPE_FLOAT64,
	MSG_GEN_TYPE_INT8,
	MSG_GEN_TYPE_UINT8,
	MSG_GEN_TYPE_INT16,
	MSG_GEN_TYPE_UINT16,
	MSG_GEN_TYPE_INT32,
	MSG_GEN_TYPE_UINT32,
	MSG_GEN_TYPE_INT64,
	MSG_GEN_TYPE_UINT64,
	MSG_GEN_TYPE_END
}msg_gen_type_t;

typedef struct msg_gen_command_t
{
	msg_gen_type_t type;
	void * data;
	bool belongs_to_previous;

	uint32_t size;
}msg_gen_command_t;

uint32_t gen_msg(port_t *port, msg_gen_command_t* message_def);

#endif /* MSG_GEN_H_ */
