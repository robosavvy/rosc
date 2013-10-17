/*
 * msggen.h
 *
 *  Created on: 17.10.2013
 *      Author: choll
 */

#ifndef MSGGEN_H_
#define MSGGEN_H_

#include <rosc/com/msg_gen.h>
#include <rosc/sebs_parse_fw/sebs_parser_frame.h>


typedef enum
{
	MSGGEN_MESSAGE_TYPE_REGISTER_PUBLISHER_TOPIC,
	MSGGEN_MESSAGE_TYPE_REQUEST_TOPIC,
	MSGGEN_MESSAGE_TYPE_ERROR,
}msggen_message_type_t;


typedef struct
{
	msggen_message_type_t type;
	void *data_ptr[4];
	msg_gen_command_t cmds;
}sebs_msggen_t;



sebs_parse_return_t sebs_msggen(sebs_parser_data_t* pdata);



#endif /* MSGGEN_H_ */
