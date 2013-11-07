/*
 * msggen.h
 *
 *  Created on: 17.10.2013
 *      Author: choll
 */
#include <rosc/com/msg_gen.h>
#include <rosc/sebs_parse_fw/sebs_parser_frame.h>

#ifndef MSGGEN_H_
#define MSGGEN_H_



#define SEBS_PARSE_SOCKET_CONNECT(PARSER_DATA, DATA_STORAGE, BUFFER, MAX_LEN, TYPE, DATA)\
		PARSER_DATA->next_parser.parser_function=(sebs_parse_function_t) &sebs_msggen;\
		PARSER_DATA->next_parser.parser_data=(void *)(&DATA_STORAGE);\
		DATA_STORAGE.buffer_size=MAX_LEN;\
		DATA_STORAGE.buffer=BUFFER;\
		DATA_STORAGE.type=TYPE;\
		DATA_STORAGE.data_ptr=DATA;\
		return (SEBS_PARSE_RETURN_INIT)

typedef enum
{
	MSGGEN_TYPE_XMLRPC_REQ_REGISTER_PUBLISHER_TOPIC,
	MSGGEN_TYPE_XMLRPC_REQUEST_TOPIC,
	MSGGEN_TYPE_XMLRPC_ERROR,
}msggen_message_type_t;


typedef struct
{
	bool first_run;  //TODO improve mem, can this be replaced by init ?...
	uint8_t *buffer;
	size_t buffer_size;
	msggen_message_type_t type;
	void **data_ptr;
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
