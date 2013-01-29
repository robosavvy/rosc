/*
 *  Copyright by Synapticon GmbH (www.synapticon.com)  ©2013
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
	ROS_IFACE_REGISTERED,
	ROS_IFACE_UNREGISTER,
}ros_iface_m_status_t;


typedef void (*ros_iface_callback)(void *);

typedef enum
{
	UNMARSCHALLING_CMD___SUBMESSAGE,
	UNMARSCHALLING_CMD___SUBMESSAGE_END,
	UNMARSCHALLING_CMD___SUBMESSAGE__ARRAY=-3,

	__UNMARSCHALLING_CMD_TYPES_START,
	UNMARSCHALLING_CMD_INT8,
	UNMARSCHALLING_CMD_INT16,
	UNMARSCHALLING_CMD_INT32,
	UNMARSCHALLING_CMD_INT64,
	UNMARSCHALLING_CMD_UINT8,
	UNMARSCHALLING_CMD_UINT16,
	UNMARSCHALLING_CMD_UINT32,
	UNMARSCHALLING_CMD_UINT64,
	UNMARSCHALLING_CMD_BOOL,
	UNMARSCHALLING_CMD_STRING,
	UNMARSCHALLING_CMD_FLOAT32,
	UNMARSCHALLING_CMD_FLOAT64,
	UNMARSCHALLING_CMD_TIME,
	UNMARSCHALLING_CMD_DURATION,
	__UNMARSCHALLING_CMD_TYPES_END,
	__UNMARSCHALLING_CMD_ARRAYS_START,
	//Values for Unmarschalling Arrays
	__UNMARSCHALLING_CMD_ARRAYS_END=__UNMARSCHALLING_CMD_ARRAYS_START+(__UNMARSCHALLING_CMD_TYPES_END-1)
}unmarshalling_cmd_t;

#define UNMARSCHALL_ARRAY +__UNMARSCHALLING_CMD_ARRAYS_START



typedef struct ros_iface_t
{
	char *topic;
	ros_iface_m_status_t state;
	ros_iface_callback callback_fct;
	unmarshalling_cmd_t *unmarschal_cmds;
}ros_iface_t;






#endif /* IFACE_SETUP_H_ */
