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

#define MARSCHALL_CMD_UNDEF_ARRAY(TYPE) 100+TYPE
#define MARSCHALL_CMD_DEF_ARRAY(TYPE) -100-TYPE

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

	MARSHALLING_CMD_STRING=MARSCHALL_CMD_UNDEF_ARRAY(MARSHALLING_CMD_INT8),

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
