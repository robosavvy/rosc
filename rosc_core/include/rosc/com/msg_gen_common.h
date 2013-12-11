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

#ifndef MSG_GEN_COMMON_H_
#define MSG_GEN_COMMON_H_


#include <rosc/string_res/msg_strings.h>
#include <rosc/system/spec.h>

#define FLOATTYPES(ENUM,TYPE)\
	__##ENUM##_FLOAT_##TYPE##_SEP,\
	ENUM##_FLOAT_32_##TYPE,\
	ENUM##_FLOAT_64_##TYPE

#define INTTYPES(ENUM,TYPE)\
	__##ENUM##_INT_##TYPE##_SEP,\
	ENUM##_INT8_##TYPE,\
	ENUM##_INT16_##TYPE,\
	ENUM##_INT32_##TYPE,\
	ENUM##_INT64_##TYPE,\
	__##ENUM##_UINT_##TYPE##_SEP,\
	ENUM##_UINT8_##TYPE,\
	ENUM##_UINT16_##TYPE,\
	ENUM##_UINT32_##TYPE,\
	ENUM##_UINT64_##TYPE\


typedef enum
{
	MSG_GEN_MODE_TYPE,
	MSG_GEN_MODE_BUFFER_FILL,
	MSG_GEN_MODE_NUMBER_TO_STRING,
	MSG_GEN_MODE_STRING_SIZE,
} msg_gen_mode_t;

typedef enum
{
	MSG_GEN_SIZE_MODE_NONE = 0,
	MSG_GEN_SIZE_MODE_PAYLOAD_START,
	MSG_GEN_SIZE_MODE_ROSFIELD_START,
	MSG_GEN_SIZE_MODE_PAYLOAD,
	MSG_GEN_SIZE_MODE_ROSFIELD,
} msg_gen_size_mode;

typedef enum
{
	MSG_GEN_NUMBERTYPE_FLOAT, MSG_GEN_NUMBERTYPE_INT, MSG_GEN_NUMBERTYPE_UINT,
} msg_gen_state_numbertype_t;

typedef enum
{

	MSG_TYPE_NONE,
	MSG_TYPE_SKIP_ENTRIES,
	MSG_TYPE_SKIP_END,

	//Single Values
	MSG_TYPE_PAYLOAD_SIZE_BINARY,
	MSG_TYPE_PAYLOAD_SIZE_STRING,

	MSG_TYPE_DOT,
	MSG_TYPE_COLON,
	MSG_TYPE_SLASH,
	MSG_TYPE_SPACE,

	MSG_TYPE_NODENAME,
	MSG_TYPE_HOSTNAME_OR_IP,

	MSG_TYPE_NODEAPI_PORT,

	MSG_TYPE_BOOL_STRING,
	MSG_TYPE_STRING,

	MSG_TYPE_MESSAGE_END,
	MSG_TYPE_HEADER_END,
	MSG_TYPE_ROSRPC_FIELD_END,

	__MSG_TYPE_BINARY_OUT,
	MSG_TYPE_BINARY_8BIT,
	MSG_TYPE_CHAR=MSG_TYPE_BINARY_8BIT,
	MSG_TYPE_BINARY_16BIT,
	MSG_TYPE_BINARY_32BIT,
	MSG_TYPE_BINARY_64BIT,

	__MSG_TYPE_FLOAT_STRING,
	MSG_TYPE_FLOAT_32_STRING,
	MSG_TYPE_FLOAT_64_STRING,

	__MSG_TYPE_INT_STRING,
	MSG_TYPE_INT8_STRING,
	MSG_TYPE_INT16_STRING,
	MSG_TYPE_INT32_STRING,
	MSG_TYPE_INT64_STRING,

	__MSG_TYPE_UINT_STRING,
	MSG_TYPE_UINT8_STRING,
	MSG_TYPE_UINT16_STRING,
	MSG_TYPE_UINT32_STRING,
	MSG_TYPE_UINT64_STRING,



	__MSG_TYPE_DESCRIPTORS,
	XMLRPC_HTTP_DESCRIPTORS(MSG_TYPE),
	MSG_TYPE_DESCRIPTOR_END,
	MSG_TYPE_DESCRIPTOR_HTTP_HEADER_END,

	__MSG_TYPE_XMLRPC_OPEN_TAGS,
	XMLRPC_TAG_STRINGS(MSG_TYPE_OPEN),

	__MSG_TYPE_XMLRPC_CLOSE_TAGS,
	XMLRPC_TAG_STRINGS(MSG_TYPE_CLOSE),

	__MSG_TYPE_ROSRPC_FIELD_STRINGS,
	ROS_FIELD_STRINGS(MSG_TYPE),

	__MSG_TYPE_STRINGS,
	__MSG_TYPE_HTTP_ACTIONS=__MSG_TYPE_STRINGS,
	XMLRPC_HTTP_ACTIONS(MSG_TYPE),

	__MSG_TYPE_XMLRPC_SLAVE_METHODNAMES,
	XMLRPC_SLAVE_METHODNAMES(MSG_TYPE),

	__MSG_TYPE_XMLRPC_STDTEXT,
	XMLRPC_STDTEXT(MSG_TYPE),

	__MSG_TYPE_HTTP_HEADER_STDTEXT,
	HTTP_HEADER_STDTEXT(MSG_TYPE),

}msg_gen_type_t;

typedef struct msg_gen_command_t
{
	msg_gen_type_t * header;
	msg_gen_type_t * payload;
	const void ** header_data;
	const void ** payload_data;

	msg_gen_mode_t submode;
		uint8_t def_state;
		const msg_gen_type_t *type;
		const void** data;

		struct
		{
			uint32_t curPos;
			const char* data;
			union
			{

				struct
				{
					uint32_t size;
					uint8_t correct;
					char single_chr;
				};

				struct
				{
					uint64_t int_number;
					uint8_t digits;
				};
				msg_gen_state_numbertype_t ntype;
			};
		} out;

		struct
		{
			msg_gen_size_mode mode;
			bool payload_size_available;
			const msg_gen_type_t *type;
			const void **data;
			uint32_t payload_size;
			uint32_t rosrpc_size;
			uint32_t *selectedSize;

		} size;

}msg_gen_command_t;


extern msg_gen_type_t rosrpc_hd[];
extern msg_gen_type_t rosrpc_topic_init[];
extern msg_gen_type_t xmlrpc_hd_request[];
extern msg_gen_type_t xmlrpc_hd_response[];
extern msg_gen_type_t xmlrpc_msg_request[];
extern msg_gen_type_t xmlrpc_msg_response[];
extern msg_gen_type_t xmlrpc_msg_error[];
extern msg_gen_type_t xmlrpc_msg_http_error[];


#define XMLRPC_REQ_CALLERID_ONLY(COMMAND)\
		xmlrpc_msg_request[2]=COMMAND;\

#define XMLRPC_REQ_ONE_STRING(COMMAND)\
		xmlrpc_msg_request[2]=COMMAND;\
		xmlrpc_msg_request[11]=MSG_TYPE_NONE;\
		//xmlrpc_msg_request_data[0]=STRING;\

#define XMLRPC_REQ_ONE_STRING_RPCURI(COMMAND)\
		xmlrpc_msg_request[2]=COMMAND;\
		xmlrpc_msg_request[11]=MSG_TYPE_NONE;\
		xmlrpc_msg_request[42]=MSG_TYPE_NONE;\
		//xmlrpc_msg_request_data[0]=STRING;\

#define XMLRPC_REQ_ONE_STRING_SERVICEURI(COMMAND)\
		xmlrpc_msg_request[2]=COMMAND;\
		xmlrpc_msg_request[11]=MSG_TYPE_NONE;\
		xmlrpc_msg_request[52]=MSG_TYPE_NONE;\
		//xmlrpc_msg_request_data[0]=STRING;\
		//xmlrpc_msg_request_data[0]=&SERVICEPORT;\

#define XMLRPC_REQ_ONE_STRING_SERVICEURI_RPCURI(COMMAND)\
		xmlrpc_msg_request[2]=COMMAND;\
		xmlrpc_msg_request[11]=MSG_TYPE_NONE;\
		xmlrpc_msg_request[42]=MSG_TYPE_NONE;\
		xmlrpc_msg_request[52]=MSG_TYPE_NONE;\
		//xmlrpc_msg_request_data[0]=STRING;\
		//xmlrpc_msg_request_data[1]=&SERVICEPORT;\

#define XMLRPC_REQ_TWO_STRING(COMMAND)\
		xmlrpc_msg_request[2]=COMMAND;\
		xmlrpc_msg_request[11]=MSG_TYPE_NONE;\
		xmlrpc_msg_request[18]=MSG_TYPE_NONE;\
		//xmlrpc_msg_request_data[0]=STRING1;\
		//xmlrpc_msg_request_data[1]=STRING2;\

#define XMLRPC_REQ_TWO_STRING_RPCURI(COMMAND)\
		xmlrpc_msg_request[2]=COMMAND;\
		xmlrpc_msg_request[11]=MSG_TYPE_NONE;\
		xmlrpc_msg_request[18]=MSG_TYPE_NONE;\
		xmlrpc_msg_request[42]=MSG_TYPE_NONE;\
		//xmlrpc_msg_request_data[0]=STRING1;\
		//xmlrpc_msg_request_data[1]=STRING2;\



#define XMLRPC_RMESSAGE_GETMASTERURI
#define XMLRPC_RMESSAGE_SHUTDOWN
#define XMLRPC_RMESSAGE_REQUESTTOPIC
#define XMLRPC_RMESSAGE_GETPID
#define XMLRPC_RMESSAGE_PUBLISHERUPDATE
#define XMLRPC_RMESSAGE_PARAMUPDATE


#define XMLRPC_REQ_GETPID() XMLRPC_REQ_CALLERID_ONLY(MSG_TYPE_METHODNAME_GETPID)
#define XMLRPC_REQ_HASPARAM(PARAM) XMLRPC_REQ_ONE_STRING(MSG_TYPE_METHODNAME_HASPARAM, PARAM)
#define XMLRPC_REQ_SEARCHPARAM(PARAM) XMLRPC_REQ_ONE_STRING(MSG_TYPE_METHODNAME_SEARCHPARAM, PARAM)
#define XMLRPC_REQ_DELETEPARAM(PARAM) XMLRPC_REQ_ONE_STRING(MSG_TYPE_METHODNAME_DELETEPARAM, PARAM)
#define XMLRPC_REQ_UNSUBSCRIBEPARAM(PARAM) XMLRPC_REQ_ONE_STRING_RPCURI(MSG_TYPE_METHODNAME_UNSUBSCRIBEPARAM, PARAM)
#define XMLRPC_REQ_REGISTERPUBLISHER(TOPIC, TOPICTYPE)	XMLRPC_REQ_TWO_STRING_RPCURI(MSG_TYPE_METHODNAME_REGISTERPUBLISHER, TOPIC,TOPICTYPE)
#define XMLRPC_REQ_UNREGISTERPUBLISHER(TOPIC) XMLRPC_REQ_ONE_STRING_RPCURI(MSG_TYPE_METHODNAME_UNREGISTERPUBLISHER, TOPIC)
#define XMLRPC_REQ_REGISTERSUBSCRIBER(TOPIC, TOPICTYPE) XMLRPC_REQ_TWO_STRING_RPCURI(MSG_TYPE_METHODNAME_REGISTERSUBSCRIBER, TOPIC, TOPICTYPE)
#define XMLRPC_REQ_UNREGISTERSUBSCRIBER(TOPIC) XMLRPC_REQ_ONE_STRING_RPCURI(MSG_TYPE_METHODNAME_UNREGISTERPUBLISHER, TOPIC)
#define XMLRPC_REQ_UNREGISTERSERVICE(SERVICENAME, SERVICEPORT) XMLRPC_REQ_ONE_STRING_SERVICEURI_RPCURI(MSG_TYPE_METHODNAME_REGISTERSERVICE, SERVICENAME, SERVICEPORT)
#define XMLRPC_REQ_REGISTERSERVICE(SERVICENAME, SERVICEPORT) XMLRPC_REQ_ONE_STRING_SERVICEURI_RPCURI(MSG_TYPE_METHODNAME_REGISTERSERVICE, SERVICENAME, SERVICEPORT)
#define XMLRPC_REQ_SUBSCRIBEPARAM XMLRPC_REQ_ONE_STRING_RPCURI(MSG_TYPE_METHODNAME_SUBSCRIBEPARAM)
#define XMLRPC_REQ_REQUESTTOPIC(TOPIC)\
		xmlrpc_msg_request[2]=MSG_TYPE_METHODNAME_REQUESTTOPIC;\
		xmlrpc_msg_request[11]=MSG_TYPE_NONE;\
		xmlrpc_msg_request[52]=MSG_TYPE_NONE;\
		//xmlrpc_msg_request_data[0]=TOPIC;



#define XMLRPC_RESP_NO_RETURN()\
	xmlrpc_msg_response[19]=MSG_TYPE_NONE;\


#define XMLRPC_RESP_REQUESTTOPIC(PORT)\
	xmlrpc_msg_response[26]=MSG_TYPE_NONE;\
	xmlrpc_msg_response[33]=MSG_TYPE_NONE;\
	xmlrpc_msg_response[40]=MSG_TYPE_NONE;\
	//xmlrpc_msg_request_data[0]=PORT;


#define XMLRPC_REQ_RESET()\
		xmlrpc_msg_request[12]=MSG_TYPE_SKIP_ENTRIES;\
		xmlrpc_msg_request[19]=MSG_TYPE_SKIP_ENTRIES;\
		xmlrpc_msg_request[26]=MSG_TYPE_SKIP_ENTRIES;\
		xmlrpc_msg_request[33]=MSG_TYPE_SKIP_ENTRIES;\
		xmlrpc_msg_request[43]=MSG_TYPE_SKIP_ENTRIES;\
		xmlrpc_msg_request[53]=MSG_TYPE_SKIP_ENTRIES;

#define XMLRPC_RESP_RESET()\
		xmlrpc_msg_response[9]=MSG_TYPE_VAL_ONE;\
		xmlrpc_msg_response[20]=MSG_TYPE_SKIP_ENTRIES;\
		xmlrpc_msg_response[27]=MSG_TYPE_SKIP_ENTRIES;\
		xmlrpc_msg_response[34]=MSG_TYPE_SKIP_ENTRIES;\
		xmlrpc_msg_response[41]=MSG_TYPE_SKIP_ENTRIES;

#define XMLRPC_RESP_HD_RESET()\
		xmlrpc_hd_response[2]=MSG_TYPE_VAL_200_OK;\
		xmlrpc_hd_response[8]=MSG_TYPE_VAL_TEXT_XML

#endif /* MSG_GEN_COMMON_H_ */
