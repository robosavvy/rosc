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
#include <rosc/system/spec.h>
#include <rosc/sebs_parse_fw/sebs_parser_frame.h>
#include <rosc/sebs_parse_fw/std_modules/sebs_parse_skip.h>
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
		DATA_STORAGE.buildup_type_array=BUILDUP;\
		DATA_STORAGE.submessage_size_array=SUBSIZES;\
		DATA_STORAGE.array_length_array=ARRAY_LENGTHS;\
		DATA_STORAGE.memory_offset_array=MEM_OFFSETS;\
		DATA_STORAGE.message_definition=MSG_DEF;\
		DATA_STORAGE.msg_storage=MSG_STORE;\
		DATA_STORAGE.submessage_state_array=ARRAY_STATES;\
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
	SEBS_PARSE_ROSBINARY_MESSAGE_FIELD,
	SEBS_PARSE_ROSBINARY_SKIP_BYTES,
	SEBS_PARSE_ROSBINARY_MESSAGE_BUILTIN_ARRAY,
	SEBS_PARSE_ROSBINARY_MESSAGE_SUBMESSAGE_ARRAY,
	SEBS_PARSE_ROSBINARY_STRING,
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



	/**
	 * Storage for value events (RPC)
	 */
	union
	{
		bool boolean;

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

	
	/*
	 * ROSMSG Variables
	 */

	bool     builtin_is_array; 				 /**< true if the current field is a builtin type array */
	bool     builtin_is_dyn_array;			 /**< true if the current array is a dynamic one */
	uint32_t builtin_array_size; 			 /**< stores the current array size of a builtin type array */
	uint32_t builtin_array_elements_to_skip; /**< stores the number of bytes to skip when a builtin array is bigger than memory */

	uint32_t string_size; 					/**<  stores the size for a string */
	uint32_t string_array_element_number;   /**<  stores the number of strings in an string array */
	uint32_t string_array_element_size;     /**<  stores the number of elements in an string array */
	uint32_t string_array_elements_to_skip; /**<  stores the number of strings to skip in an dynamic string array*/
	void *string_array_memory;	    		/**<  stores the location of the memory of the string array */

	const ros_buildup_type_t*  buildup_type_array;		/**< contains the message bildup information*/
	uint32_t buildup_type_current_field;				/**< stores array element number of the current buildup information*/

	const size_t* submessage_size_array;		/**< contains the information for the size of submessages and string array elements*/
	uint32_t submessage_size_current_element;	/**< contains the number of the current element*/

	rosc_msg_submessage_state_t *submessage_state_array;	/**< this array contains data for submessages or submessage arrays*/
	uint32_t submessage_depth;								/**< this is the current message depth, which is used to access data inside submessage_state array*/

	const size_t* array_length_array;			/**< this array contains all array lengths of the current message */
	uint32_t array_length_current_element;		/**< this is the number of the current array length */

	const size_t* memory_offset_array;			/**< this array contains all offsets to the msg_storage or to the current struct */
	uint32_t memory_offset_current_element;		/**< this points to the current element in the memory offset array */

	const int8_t* message_definition;		/**< this points to the message definition string*/

	int8_t* msg_storage;					/**< this points to the place in memory where the message will be stored*/

	/**
	 * Submode data storage
	 */
	union
	{
		sebs_parse_copy2buffer_data_t copy2buffer;
		sebs_parse_seekstring_data_t seekstring;
		sebs_parse_skip_data_t skip;
	};

}sebs_parse_ros_data_t;



sebs_parse_return_t sebs_parse_ros(sebs_parser_data_t* pdata);

#endif /* SEBS_PARSE_ROS_H_ */
