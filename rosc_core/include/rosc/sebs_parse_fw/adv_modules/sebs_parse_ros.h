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
 *  sebs_parse_ros.h created by Christian Holl
 */

#ifndef SEBS_PARSE_ROS_H_
#define SEBS_PARSE_ROS_H_

#include <endian.h>
#include <rosc/sebs_parse_fw/sebs_parser_frame.h>
#include <rosc/sebs_parse_fw/std_modules/sebs_parse_copy2buffer.h>
#include <rosc/sebs_parse_fw/std_modules/sebs_parse_seekstring.h>
#include <rosc/string_res/msg_strings.h>
#include <rosc/com/ros_msg_common.h>



#define SEBS_PARSE_ROS_INIT_RPC(PARSER_DATA, DATA_STORAGE)\
		PARSER_DATA->next_parser.parser_function=(sebs_parse_function_t) &sebs_parse_ros;\
		PARSER_DATA->next_parser.parser_data=(void *)(&DATA_STORAGE);\
		DATA_STORAGE.mode=SEBS_PARSE_ROS_MODE_ROSRPC;\
		return (SEBS_PARSE_RETURN_INIT_ADV)

#define SEBS_PARSE_ROS_INIT_MSG(PARSER_DATA, DATA_STORAGE,BUILDUP,SUBSIZES,ARRAY_LENGTHS,MEM_OFFSETS,MSG_DEF,MSG_STORE,ARRAY_STATES)\
		PARSER_DATA->next_parser.parser_function=(sebs_parse_function_t) &sebs_parse_ros;\
		PARSER_DATA->next_parser.parser_data=(void *)(&DATA_STORAGE);\
		DATA_STORAGE.mode=SEBS_PARSE_ROS_MODE_BINARY;\
		DATA_STORAGE.buildup=BUILDUP;\
		DATA_STORAGE.submessage_sizes=SUBSIZES;\
		DATA_STORAGE.array_lengths=ARRAY_LENGTHS;\
		DATA_STORAGE.memory_offsets=MEM_OFFSETS;\
		DATA_STORAGE.message_definition=MSG_DEF;\
		DATA_STORAGE.msg_storage=MSG_STORE;\
		DATA_STORAGE.array_state=ARRAY_STATES;\
		return (SEBS_PARSE_RETURN_INIT_ADV)


typedef enum
{
	SEBS_PARSE_ROS_MODE_ROSRPC,
	SEBS_PARSE_ROS_MODE_BINARY,
}sebs_parse_ros_mode_t;

typedef enum
{
	SEBS_PARSE_ROSPRC_MESSAGE_LENGTH,
	SEBS_PARSE_ROSRPC_FIELD_LENGTH,
	SEBS_PARSE_ROSRPC_FIELD_ID,
	SEBS_PARSE_ROSRPC_FIELD_VALUE,
	SEBS_PARSE_ROSRPC_FIELD_EQUAL,
	SEBS_PARSE_ROSRPC_SKIP_FIELD_CONTENT,
	SEBS_PARSE_ROSBINARY_MESSAGE_LENGTH,
	SEBS_PARSE_ROSBINARY_ARRAY_LENGTH,
	SEBS_PARSE_ROSBINARY_VALUE,
}sebs_parse_ros_state_t;


typedef enum
{
	ROS_FIELD_STRINGS(SEBS_PARSE),
}sebs_parse_ros_rpc_field_t;

typedef enum
{
	SEBS_PARSE_ROS_EVENT_RPC_NONE=SEBS_PARSE_EVENT_NONE,
	SEBS_PARSE_ROS_EVENT_RPC_FIELD_START,
	SEBS_PARSE_ROS_EVENT_MESSAGE_END,

}sebs_parse_ros_event_t;

typedef struct
{
	uint32_t message_length;
	uint32_t field_length;
	sebs_parse_ros_mode_t mode;
	sebs_parse_ros_state_t state;
	sebs_parse_ros_rpc_field_t rpc_field_id;

	const ros_buildup_type_t* binary_buildup;

	void *binary_data_io;

	/**
	 * Storage for value events (RPC)
	 */
	union
	{
		bool	boolean;

		uint8_t uint8;
		uint16_t uint16;
		uint32_t uint32;
		uint64_t uint64;

		int8_t int8;
		int16_t int16;
		int32_t int32;
		int64_t int64;

		float64_t float64;
		float32_t float32;
	}parsed_value;



	const ros_buildup_type_t*  buildup;
	const size_t* submessage_sizes;
	const size_t* array_lengths;
	const size_t* memory_offsets;
	const int8_t* message_definition;
	int8_t* msg_storage;
	rosc_msg_array_state_t *array_state;

	/**
	 * Submode data storage
	 */
	union
	{
		sebs_parse_copy2buffer_data_t copy2buffer;
		sebs_parse_seekstring_data_t seekstring;
	};

}sebs_parse_ros_data_t;



sebs_parse_return_t sebs_parse_ros(sebs_parser_data_t* pdata);

#endif /* SEBS_PARSE_ROS_H_ */
