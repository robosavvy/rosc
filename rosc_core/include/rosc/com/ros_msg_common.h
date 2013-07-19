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
 *  ros_msg_common.h created by Christian Holl
 */

#ifndef ROS_MSG_COMMON_H_
#define ROS_MSG_COMMON_H_

#include <rosc/system/types.h>

typedef enum
{
	ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY,
	ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY_UL,
	ROS_MSG_BUILDUP_TYPE_SUBMESSAGE,
	ROS_MSG_BUILDUP_TYPE_ARRAY,
	ROS_MSG_BUILDUP_TYPE_ARRAY_UL,
	ROS_MSG_BUILDUP_TYPE_STRING,

	ROS_MSG_BUILDUP_TYPE_CHAR,
	ROS_MSG_BUILDUP_TYPE_INT8,
	ROS_MSG_BUILDUP_TYPE_INT16,
	ROS_MSG_BUILDUP_TYPE_INT32,
	ROS_MSG_BUILDUP_TYPE_INT64,

	ROS_MSG_BUILDUP_TYPE_BYTE,
	ROS_MSG_BUILDUP_TYPE_UINT8,
	ROS_MSG_BUILDUP_TYPE_UINT16,
	ROS_MSG_BUILDUP_TYPE_UINT32,
	ROS_MSG_BUILDUP_TYPE_UINT64,

	ROS_MSG_BUILDUP_TYPE_FLOAT32,
	ROS_MSG_BUILDUP_TYPE_FLOAT64,

	ROS_MSG_BUILDUP_TYPE_TIME,
	ROS_MSG_BUILDUP_TYPE_DURATION,
	ROS_MSG_BUILDUP_TYPE_BOOL,

	ROS_MSG_BUILDUP_TYPE_MESSAGE_END,
}ros_msg_buildup_type_t;


typedef enum
{
	ROS_HANDLER_TYPE_SERVICE_SERVER,
	ROS_HANDLER_TYPE_SERVICE_CLIENT,
	ROS_HANDLER_TYPE_TOPIC_PUBLISHER,
	ROS_HANDLER_TYPE_TOPIC_SUBSCRIBER,
	ROS_HANDLER_TYPE_ROSRPC_CLIENT,
	ROS_HANDLER_TYPE_ROSRPC_SERVER,
}ros_handler_type_t;

typedef void (*ros_callbackFkt_t)(const void* const __msg);

typedef struct
{
	const ros_handler_type_t handler_type;
	const ros_msg_buildup_type_t* const buildup;
	const size_t* const submessage_sizes;
	const size_t* const array_lengths;
	const size_t* const memory_offsets;
	const int8_t* const message_definition;
	const int8_t* const md5sum;
	const ros_callbackFkt_t callback;
}ros_msg_init_t;

#endif /* ROS_MSG_COMMON_H_ */
