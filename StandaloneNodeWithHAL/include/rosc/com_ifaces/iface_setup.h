/*
 *  Copyright by Synapticon GmbH (www.synapticon.com)  ��2013
 *
 *  iface_setup.h
 *
 *  This file is part of ROScNode Library.
 *
 *  ROScNode Library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ROScNode Library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ROScNode Library. If not, see <http://www.gnu.org/licenses/>.
 *
 *	File created by choll on 29.01.2013
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

typedef enum
{
	MARSHALLING_CMD___SUBMESSAGE,
	MARSHALLING_CMD___SUBMESSAGE_END,
	MARSHALLING_CMD___SUBMESSAGE__ARRAY=-3,

	__MARSHALLING_CMD_TYPES_START,
	MARSHALLING_CMD_INT8,
	MARSHALLING_CMD_INT16,
	MARSHALLING_CMD_INT32,
	MARSHALLING_CMD_INT64,
	MARSHALLING_CMD_UINT8,
	MARSHALLING_CMD_UINT16,
	MARSHALLING_CMD_UINT32,
	MARSHALLING_CMD_UINT64,
	MARSHALLING_CMD_BOOL,
	MARSHALLING_CMD_STRING,
	MARSHALLING_CMD_FLOAT32,
	MARSHALLING_CMD_FLOAT64,
	MARSHALLING_CMD_TIME,
	MARSHALLING_CMD_DURATION,
	__MARSHALLING_CMD_TYPES_END,
	__MARSHALLING_CMD_ARRAYS_START,
	//Reserved Values for MARSHALLING array commands
	__MARSHALLING_CMD_ARRAYS_END=__MARSHALLING_CMD_ARRAYS_START+(__MARSHALLING_CMD_TYPES_END-1)

}marshalling_cmd_t;

#define MARSCHALL_ARRAY +__MARSHALLING_CMD_ARRAYS_START

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
