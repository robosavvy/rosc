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
 *  MultiArrayDimension.h created by Christian Holl
 */

#ifndef MULTIARRAYDIMENSION_H_
#define MULTIARRAYDIMENSION_H_

#include <rosc/sebs_parse_fw/adv_modules/sebs_parse_ros.h>


union
{
	rosc_msg_MultiArrayDimension dim;
	char *array;
}rosc_msg_MultiArrayDimension_Padding={ { {0xFFFFFFFF,ROSC_PADDING_ARRAY_START_VALUE} }, 0xFFFFFFFF,0xFFFFFFFF };

#define ROSC_DEPEND_MSG_MULTIARRAYDIMENSION\
const ros_type_t rosc_msg_buildup_MultiArrayDimension[]=\
{\
	ROS_TYPE_STRING,\
	ROS_TYPE_UINT32,\
	ROS_TYPE_UINT32,\
	ROS_TYPE_UINT32,\
};\

typedef struct
{
	struct
	{
		uint32_t size;
		char* data;
	}label;
	uint32_t size;
	uint32_t stride;
}rosc_msg_MultiArrayDimension;


#define ROSC_NEW_MULTIARRAYDIMENSION_STATIC_TYPE(STATIC_NAME, MULTIARRAYDIMENSION_LABEL_MAX_SIZE)\
typedef struct\
{\
	struct\
	{\
		uint32_t size;\
		char data[MULTIARRAYDIMENSION_LABEL_MAX_SIZE];\
	}label;\
	uint32_t size;\
	uint32_t stride;\
}rosc_msg_MultiArrayDimension_static_STATIC_NAME;\




#endif /* MULTIARRAYDIMENSION_H_ */
