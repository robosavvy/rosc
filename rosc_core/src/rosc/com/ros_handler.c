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
 *  ros_handler.c created by Christian Holl
 */

#include <rosc/com/ros_handler.h>
#include <rosc/debug/debug_out.h>
#include <rosc/system/status.h>

sebs_parse_return_t ros_handler(sebs_parser_data_t* pdata)
{
	ros_handler_data_t *hdata=pdata->handler_data;


	if(pdata->handler_init)
	{
		DEBUG_PRINT_STR("ROS HANDLER INIT");
		pdata->handler_init=false;
		pdata->return_to_handler=false;
		pdata->overall_len=0;
		pdata->security_len=1024;
		SEBS_PARSE_ROS_INIT(pdata,hdata->ros);
		switch(pdata->init_mode)
		{
			case IFACE_TYPE_ROSRPC_CLIENT:
			case IFACE_TYPE_ROSRPC_SERVER:
			case IFACE_TYPE_TOPIC_PUBLISHER:
			case IFACE_TYPE_TOPIC_SUBSCRIBER:
			case IFACE_TYPE_XMLRPC_CLIENT:
			case IFACE_TYPE_XMLRPC_SERVER:

				break;
			default:
				ROSC_FATAL("ros handler: Not a ros handler type!");
		}
	}

	sebs_parse_ros_event_t *ros_event=(sebs_parse_ros_event_t *)&pdata->event;







	switch(*ros_event)
	{
		case SEBS_PARSE_ROS_EVENT_RPC_FIELD_START:
			DEBUG_PRINT(STR,"Field",ros_field_strings[hdata->ros.rpc_field_id]);
			DEBUG_PRINT(INT,"Field Content Length", hdata->ros.field_length);
			break;
		case SEBS_PARSE_ROS_EVENT_MESSAGE_END:
			DEBUG_PRINT_STR("HANDLER: MESSAGE END!")
			break;

		default: //TODO check
			break;
	}



	return (SEBS_PARSE_RETURN_GO_AHEAD);
}
