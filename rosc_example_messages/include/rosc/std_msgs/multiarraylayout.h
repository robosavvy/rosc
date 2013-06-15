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
 *  MultiArrayLayout.h created by Christian Holl
 */

#ifndef MULTIARRAYLAYOUT_H_
#define MULTIARRAYLAYOUT_H_

#include <rosc/system/types.h>
#include <rosc/com/ros_msg_buildup.h>
#include <rosc/std_msgs/multiarraydimension.h>

//std_msgs/MultiArrayDimension[] dim
//  string label
//  uint32 size
//  uint32 stride
//uint32 data_offset

typedef struct
{
	struct
	{
		uint32_t size;
		ros_msg_MultiArrayDimension *data;
	}dim;
	uint32_t data_offset;
}ros_msg_MultiArrayLayout;


extern const ros_msg_buildup_t const *rosc_msg_MultiArrayLayout_buildup;

#define ROS_MSG_NEW_STATIC_SUBSCRIBER_TYPE(NAME, LABEL_SIZE)\
typedef struct\
{\
	struct\
	{\
		char data[LABEL_SIZE];\
		uint32_t size;\
	}label;\
	uint32_t size;\
	uint32_t stride;\
}ros_msg_static_subscriber_MultiArrayDimension_ ## NAME;\
uint32_t ros_msg_static_subscriber_sizeinfo_MultiArrayDimension_ ## NAME[]={ LABEL_SIZE }

#endif /* MULTIARRAYLAYOUT_H_ */
