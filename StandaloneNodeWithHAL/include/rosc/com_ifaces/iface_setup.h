/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2013, Synapticon GmbH
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Synapticon GmbH nor the names of its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * iface_setup.h
 *
 *  Created on: 27.01.2013
 *      Author: Christian Holl
 */

#ifndef IFACE_SETUP_H_
#define IFACE_SETUP_H_


typedef enum
{
	ROS_IFACE_OFF,
	ROS_IFACE_REGISTER,
	__ROS_IFACE_REGISTERED,
	__ROS_IFACE_UNREGISTER,
}ros_iface_m_state_t;

//TODO Add callback additions stuff (xmos channels)
typedef void (*ros_iface_callback)(void *);

#define MARSHALLING_CMD_UNDEF_ARRAY(TYPE) 100+TYPE
#define MARSHALLING_CMD_DEF_ARRAY(TYPE) -100-TYPE

typedef enum
{
	MARSHALLING_CMD_SUBMESSAGE      =-3,
	MARSHALLING_CMD_SUBMESSAGE_END  =-2,
	MARSHALLING_CMD_SUBMESSAGE_ARRAY=-1,

	MARSHALLING_CMD_INT8 =11,
	MARSHALLING_CMD_INT16=12,
	MARSHALLING_CMD_INT32=14,
	MARSHALLING_CMD_INT64=18,

	MARSHALLING_CMD_UINT8 =21,
	MARSHALLING_CMD_UINT16=22,
	MARSHALLING_CMD_UINT32=24,
	MARSHALLING_CMD_UINT64=28,

	MARSHALLING_CMD_FLOAT32=34,
	MARSHALLING_CMD_FLOAT64=38,

	MARSHALLING_CMD_TIME    =44,
	MARSHALLING_CMD_DURATION=54,
	MARSHALLING_CMD_BOOL    =61,
	MARSHALLING_CMD_STRING  =71

//	two 32 BIT integer values, there is no msg file for it
}marshalling_cmd_t;


typedef enum
{
	PUBSUB_TYPE_SUBSCRIBER,
	PUBSUB_TYPE_PUBLISHER,
}pubSub_type;


typedef struct ros_topic_t
{
	char *topic_str;
	pubSub_type type;
	ros_iface_m_state_t state;
	ros_iface_callback callback_fct;
	const marshalling_cmd_t *marshalling_cmds;
}ros_topic_t;

extern ros_topic_t pubSub_list[];


//TODO:
// Into ENTRY:
// -Array with message identification string
// -Marshalling stuff array (by Typename)
// -



#define PUBLISHER_SUBSCRIBER_LIST_HEAD\
	ros_topic_t pubSub_list[]=\
	{

#define PUBLISHER_ENTRY(TOPIC,INITIAL_STATE,CALLBACK_FUNCTION,MARSHALLING_CMD_ARRAY)\
{ .topic_str=TOPIC,	.type=PUBSUB_TYPE_PUBLISHER, .state=INITIAL_STATE, .callback_fct=CALLBACK_FUNCTION .marshalling_cmds=0 }

#define SUBSCRIBER_ENTRY(TOPIC,INITIAL_STATE,CALLBACK_FUNCTION,MARSHALLING_CMD_ARRAY)


#define PUBLISHER_SUBSCRIBER_LIST_FOOT\
	};





#endif /* IFACE_SETUP_H_ */
