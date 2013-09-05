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
 *  msg_gen_common.c created by Christian Holl
 */

#include <rosc/sebs_parse_fw/gen_modules/msg_gen_common.h>

static void *header_storage[5];
static void *payload_storage[5];



#define XMLRPC_METHODCALL_HASPARAM(STR_CALLERID, STR_PARAM)

#define XMLRPC_METHODCALL_SEARCHPARAM(STR_CALLERID, STR_PARAM)

#define XMLRPC_METHODCALL_DELETEPARAM(STR_CALLERID, STR_PARAM)









static const msg_gen_type_t rosrpc_common_message_header[]=
{
	MSG_TYPE_PAYLOAD_SIZE_BINARY,
	MSG_TYPE_HEADER_END,
};

static const msg_gen_type_t rosrpc_topic_init_payload[]=
{
	  MSG_TYPE_ROS_FIELD_MESSAGE_DEFINITION,
	  MSG_TYPE_STRING, //MESSAGE DEFINITION
	  MSG_TYPE_ROSRPC_FIELD_END,

	  MSG_TYPE_ROS_FIELD_CALLERID,
	  MSG_TYPE_STRING,//CALLERID
	  MSG_TYPE_ROSRPC_FIELD_END,

	  MSG_TYPE_ROS_FIELD_TCP_NODELAY,
	  MSG_TYPE_UINT8_STRING, //NODEDELAY
	  MSG_TYPE_ROSRPC_FIELD_END,

	  MSG_TYPE_ROS_FIELD_MD5SUM,
	  MSG_TYPE_STRING, //MD5
	  MSG_TYPE_ROSRPC_FIELD_END,

	  MSG_TYPE_ROS_FIELD_TOPIC,
	  MSG_TYPE_STRING, //TOPIC
	  MSG_TYPE_ROSRPC_FIELD_END,

	  MSG_TYPE_MESSAGE_END
};

	static const msg_gen_type_t xmlrpc_request_header[]=
	{
		MSG_TYPE_VAL_METHOD_POST,
		MSG_TYPE_DESCRIPTOR_END,
		MSG_TYPE_DESCRIPTOR_USER_AGENT,
		MSG_TYPE_VAL_XMLRPC_ROSC_NODELIB,
		MSG_TYPE_DESCRIPTOR_END,

		MSG_TYPE_DESCRIPTOR_ACCEPT_ENCODING,
		MSG_TYPE_DESCRIPTOR_END,

		MSG_TYPE_DESCRIPTOR_CONTENT_ENCODING,
		MSG_TYPE_DESCRIPTOR_END,

		MSG_TYPE_DESCRIPTOR_CONTENT_LENGTH,
			MSG_TYPE_PAYLOAD_SIZE_STRING,
		MSG_TYPE_DESCRIPTOR_END,
		MSG_TYPE_DESCRIPTOR_HTTP_HEADER_END,
	};

	static msg_gen_type_t xmlrpc_request[]=
	{
			MSG_TYPE_OPEN_TAG_METHODCALL,

				MSG_TYPE_OPEN_TAG_METHODNAME,
					MSG_TYPE_METHODNAME_GETSUBSCRIPTIONS,
				MSG_TYPE_CLOSE_TAG_METHODNAME,

				MSG_TYPE_OPEN_TAG_PARAMS,

					MSG_TYPE_OPEN_TAG_PARAM,
						MSG_TYPE_OPEN_TAG_VALUE,
							//MSG_TYPE_STRING,
						MSG_TYPE_CLOSE_TAG_VALUE,
					MSG_TYPE_CLOSE_TAG_PARAM,

				MSG_TYPE_CLOSE_TAG_PARAMS,
			MSG_TYPE_CLOSE_TAG_METHODCALL,

			MSG_TYPE_MESSAGE_END
	};


	static msg_gen_type_t xmlrpc_response[]=
	{
			MSG_TYPE_OPEN_TAG_METHODRESPONSE,

			MSG_TYPE_OPEN_TAG_PARAMS,
				MSG_TYPE_OPEN_TAG_PARAM,
					MSG_TYPE_OPEN_TAG_VALUE,
						MSG_TYPE_OPEN_TAG_ARRAY,
							MSG_TYPE_OPEN_TAG_DATA,


								MSG_TYPE_OPEN_TAG_VALUE,
									MSG_TYPE_OPEN_TAG_INT,
										MSG_TYPE_UINT8_STRING,
									MSG_TYPE_CLOSE_TAG_INT,
								MSG_TYPE_CLOSE_TAG_VALUE,

								MSG_TYPE_SKIP_ENTRIES,

								MSG_TYPE_OPEN_TAG_VALUE,
									MSG_TYPE_OPEN_TAG_STRING,
										MSG_TYPE_STRING,
									MSG_TYPE_CLOSE_TAG_STRING,
								MSG_TYPE_CLOSE_TAG_VALUE,

								MSG_TYPE_OPEN_TAG_VALUE,
									MSG_TYPE_OPEN_TAG_ARRAY,
										MSG_TYPE_OPEN_TAG_DATA,

											MSG_TYPE_SKIP_ENTRIES,

											MSG_TYPE_OPEN_TAG_VALUE,
												MSG_TYPE_STDTXT_TCPROS,
											MSG_TYPE_CLOSE_TAG_VALUE,

											MSG_TYPE_OPEN_TAG_VALUE,
												MSG_TYPE_STDTXT_ROS,
											MSG_TYPE_CLOSE_TAG_VALUE,

											MSG_TYPE_OPEN_TAG_VALUE,
												MSG_TYPE_INT16_STRING,
											MSG_TYPE_CLOSE_TAG_VALUE,

										MSG_TYPE_CLOSE_TAG_DATA,
									MSG_TYPE_CLOSE_TAG_ARRAY,
								MSG_TYPE_CLOSE_TAG_VALUE,

							MSG_TYPE_CLOSE_TAG_DATA,
						MSG_TYPE_CLOSE_TAG_ARRAY,
					MSG_TYPE_CLOSE_TAG_VALUE,
				MSG_TYPE_CLOSE_TAG_PARAM,
			MSG_TYPE_CLOSE_TAG_PARAMS,

			MSG_TYPE_CLOSE_TAG_METHODRESPONSE
	};


		msg_gen_command_t msg_def_ros_topic_init={xmlrpc_request_header,
												  xmlrpc_request,(const void const **)header_storage,
												 (const void const **)payload_storage};

