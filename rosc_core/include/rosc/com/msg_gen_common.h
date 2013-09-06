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

	__MSG_TYPE_NONE,
	//Single Values
	MSG_TYPE_PAYLOAD_SIZE_BINARY,
	MSG_TYPE_PAYLOAD_SIZE_STRING,

	MSG_TYPE_DOT,
	MSG_TYPE_COLON,

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

	MSG_TYPE_SKIP_ENTRIES,

}msg_gen_type_t;

typedef struct msg_gen_command_t
{
	msg_gen_type_t const *header;
	msg_gen_type_t const *payload;
	const void const **header_data;
	const void const **payload_data;
}msg_gen_command_t;

#define MSG_DEF_ROS_TOPIC_INIT(MESSAGE_DEFINITION, CALLERID, NODEDELAY, MD5, TOPIC)\
		&msg_def_ros_topic_init;\
		msg_def_ros_topic_init.payload_data[0]=(void*)MESSAGE_DEFINITION;\
		msg_def_ros_topic_init.payload_data[1]=(void*)CALLERID;\
		msg_def_ros_topic_init.payload_data[2]=(void*)NODEDELAY;\
		msg_def_ros_topic_init.payload_data[3]=(void*)MD5;\
		msg_def_ros_topic_init.payload_data[4]=(void*)TOPIC;
extern msg_gen_command_t msg_def_ros_topic_init;



#endif /* MSG_GEN_COMMON_H_ */
