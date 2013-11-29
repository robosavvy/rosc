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
 *  ros_handler.h created by Christian Holl
 */

#ifndef ROS_HANDLER_H_
#define ROS_HANDLER_H_

#include <rosc/sebs_parse_fw/sebs_parser_frame.h>
#include <rosc/sebs_parse_fw/adv_modules/sebs_parse_ros.h>
#include <rosc/com/ros_msg_common.h>
#include <rosc/sebs_parse_fw/send_modules/msggen.h>
#include <rosc/system/eth.h>



typedef enum
{
	ROS_HANDLER_STATE_NONE,
	ROS_HANDLER_STATE_CHECK_MD5SUM,
	ROS_HANDLER_STATE_CHECK_IFACE_NAME,
	ROS_HANDLER_STATE_SUBSCRIBER_HEADER_SEND,
	ROS_HANDLER_STATE_PUBLISHER_HEADER_SEND,
}ros_handler_state;


typedef struct
{
	iface_t iface;
	ros_handler_state hstate;
	bool md5sum_ok;
	bool iface_ok;

	const void *genPayloadData[4];
	sebs_msggen_t gen;
	sebs_parse_ros_data_t ros;

}ros_handler_data_t;

sebs_parse_return_t ros_handler(sebs_parser_data_t* pdata);

#endif /* ROS_HANDLER_H_ */
