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
 *  msg_gen_handler.h created by Christian Holl
 */

#ifndef MSG_GEN_HANDLER_H_
#define MSG_GEN_HANDLER_H_

#include <rosc/sebs_parse_fw/sebs_parser_frame.h>
#include <rosc/sebs_parse_fw/gen_modules/msg_gen_common.h>
#include <rosc/sebs_parse_fw/gen_modules/size_deter.h>
#include <rosc/sebs_parse_fw/gen_modules/buffer_fill.h>
#include <rosc/debug/debug_out.h>

typedef enum
{
	MSG_GEN_HANDLER_STATE_SIZE,
	MSG_GEN_HANDLER_STATE_OUTPUT,
}msg_gen_handler_state_t;


typedef struct
{
	msg_gen_command_t *message_definition;

}msg_gen_handler_init_t;


typedef enum
{
	MSG_GEN_HANDLER_EVENT_NONE=SEBS_PARSE_EVENT_NONE,
	MSG_GEN_HANDLER_EVENT_BUFFER_FULL,
	MSG_GEN_HANDLER_EVENT_END_MESSAGE,
}msg_gen_handler_event_t;

typedef struct
{
	uint32_t buffer_size;
	msg_gen_command_t *message_definition;
	msg_gen_handler_state_t handler_state;
	union
	{
		uint32_t message_size;
		uint32_t msg_def;

		buffer_fill_data_t buffer_fill;
		size_deter_data_t size_deter;
	};
}msg_gen_handler_data_t;

sebs_parse_return_t msg_gen_handler(sebs_parser_data_t* pdata);

#endif /* MSG_GEN_HANDLER_H_ */
