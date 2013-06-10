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
 *  sebs_parse_ros.c created by Christian Holl
 */

#include <rosc/sebs_parse_fw/adv_modules/sebs_parse_ros.h>
#include <rosc/debug/debug_out.h>
#include <rosc/system/endian.h>


sebs_parse_return_t sebs_parse_ros(sebs_parser_data_t* pdata)
{
	sebs_parse_ros_data_t *fdata=(sebs_parse_ros_data_t *)pdata->current_parser.parser_data;
	if(pdata->function_init)
	{
		pdata->function_init=false;
		fdata->state=SEBS_PARSE_ROSPRC_MESSAGE_LENGTH;

		DEBUG_PRINT_STR("ROS PARSER");
	}

	while (*pdata->len > 0)
	{
		switch(fdata->state)
		{
			case SEBS_PARSE_ROSPRC_MESSAGE_LENGTH:
				fdata->state=SEBS_PARSE_ROSRPC_FIELD_LENGTH;
				SEBS_PARSE_COPY2BUFFER_INIT(pdata,fdata->copy2buffer,&fdata->message_length,4,0,g_byte_order_correction_to_system->SIZE_4_B,0);
				break;

			case SEBS_PARSE_ROSRPC_FIELD_LENGTH:
				DEBUG_PRINT(INT,"MSG LEN",fdata->message_length);
				fdata->state=SEBS_PARSE_ROSRPC_FIELD_ID;
				SEBS_PARSE_COPY2BUFFER_INIT(pdata,fdata->copy2buffer,&fdata->field_length,4,0,g_byte_order_correction_to_system->SIZE_4_B,0);

				break;

			case SEBS_PARSE_ROSRPC_FIELD_ID:
				fdata->message_length-=4;
				DEBUG_PRINT(INT,"Field LEN",fdata->field_length);

				break;

		}

	}
	return (SEBS_PARSE_RETURN_GO_AHEAD);
}
