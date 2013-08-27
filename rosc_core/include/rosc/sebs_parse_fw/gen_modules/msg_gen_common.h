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
	MSG_TYPE_PAYLOAD_SIZE_BINARY,
	MSG_TYPE_PAYLOAD_SIZE_STRING,
	MSG_TYPE_PAYLOAD_SIZE_START,

	MSG_TYPE_CHAR,
	MSG_TYPE_STRING,

	FLOATTYPES(MSG_TYPE, BINARY),
	INTTYPES(MSG_TYPE, BINARY),

	FLOATTYPES(MSG_TYPE, STRING),
	INTTYPES(MSG_TYPE, STRING),

	__MSG_TYPE_DESCRIPTORS,
	XMLRPC_HTTP_DESCRIPTORS(MSG_TYPE),

	__MSG_TYPE_XMLRPC_OPEN_TAGS,
	XMLRPC_TAG_STRINGS(MSG_TYPE_OPEN),

	__MSG_TYPE_XMLRPC_CLOSE_TAGS,
	XMLRPC_TAG_STRINGS(MSG_TYPE_CLOSE),

	__MSG_TYPE_ROS_FIELD_STRINGS,
	ROS_FIELD_STRINGS(MSG_TYPE),
	ROS_FIELD_END,

	MSG_TYPE_ROS_FIELD_END,

	MSG_TYPE_END,
}msg_gen_type_t;

typedef struct msg_gen_command_t
{
	const msg_gen_type_t const *type;
	void **data;
}msg_gen_command_t;



#define MSG_DEF_ROS_TOPIC_HEADER(MESSAGE_DEFINITION, CALLERID, NODEDELAY, MD5, TOPIC)\
		&msg_def_ros_topic_header;\
		msg_def_ros_topic_header.data[0]=(void*)MESSAGE_DEFINITION;\
		msg_def_ros_topic_header.data[1]=(void*)CALLERID;\
		msg_def_ros_topic_header.data[2]=(void*)NODEDELAY;\
		msg_def_ros_topic_header.data[3]=(void*)MD5;\
		msg_def_ros_topic_header.data[4]=(void*)TOPIC;
extern msg_gen_command_t msg_def_ros_topic_header;



#endif /* MSG_GEN_COMMON_H_ */
