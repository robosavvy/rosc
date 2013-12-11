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
 *  msggen.h created by Christian Holl
 */

#include <rosc/com/msg_gen.h>
#include <rosc/sebs_parse_fw/sebs_parser_frame.h>

#ifndef MSGGEN_H_
#define MSGGEN_H_



#define SEBS_PARSE_MSG_GEN(PARSER_DATA, DATA_STORAGE, BUFFER, MAX_LEN, TYPE, HEADER_DATA, PAYLOAD_DATA)\
		PARSER_DATA->next_parser.parser_function=(sebs_parse_function_t) &sebs_msggen;\
		PARSER_DATA->next_parser.parser_data=(void *)(&DATA_STORAGE);\
		DATA_STORAGE.buffer_size=MAX_LEN;\
		DATA_STORAGE.buffer=BUFFER;\
		DATA_STORAGE.type=TYPE;\
		DATA_STORAGE.payload_data_ptr = PAYLOAD_DATA;\
		DATA_STORAGE.header_data_ptr = HEADER_DATA;\
		return (SEBS_PARSE_RETURN_INIT)

typedef enum
{
	MSGGEN_TYPE_XMLRPC_REQ_REGISTER_PUBLISHER,
	MSGGEN_TYPE_XMLRPC_REQ_REGISTER_SUBSCRIBER,
	MSGGEN_TYPE_XMLRPC_REQ_UNREGISTER_PUBLISHER,
	MSGGEN_TYPE_XMLRPC_REQ_UNREGISTER_SUBSCRIBER,
	MSGGEN_TYPE_XMLRPC_REQUEST_TOPIC,
	MSGGEN_TYPE_XMLRPC_RESPOND_REQUEST_TOPIC,
	MSGGEN_TYPE_XMLRPC_ACK,
	MSGGEN_TYPE_XMLRPC_ERROR,
	MSGGEN_TYPE_HTTP_ERROR,
	MSGGEN_TYPE_ROSMSG_HEADER,
}msggen_message_type_t;


typedef struct
{
	bool first_run;  //TODO improve mem, can this be replaced by init ?...
	uint8_t *buffer;
	size_t buffer_size;
	msggen_message_type_t type;
	const void **header_data_ptr;
	const void **payload_data_ptr;
	msg_gen_command_t cmds;
	bool finished;
}sebs_msggen_t;


typedef enum
{
	SEBS_MSGGEN_EVENT_NONE=SEBS_PARSE_EVENT_NONE,
	SEBS_MSGGEN_EVENT_SOCKET_SIGNAL,
}sebs_msggen_event_t;


sebs_parse_return_t sebs_msggen(sebs_parser_data_t* pdata);



#endif /* MSGGEN_H_ */
