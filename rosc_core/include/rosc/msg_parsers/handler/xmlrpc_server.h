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
 *  xmlrpc_server.h created by Christian Holl
 */

#ifndef XMLRPC_SERVER_H_
#define XMLRPC_SERVER_H_

#include <rosc/msg_parsers/xml_parser_structure.h>
#include <rosc/msg_parsers/handler/xmlrpc_string_id.h>



typedef struct xmlrpc_server_data_t
{
	xmlrpc_server_method_t method;
	xmlrpc_server_target_t target;
	xmlrpc_slave_api_method_t rpcmethod;
	uint8_t value_number;
	uint32_t current_value_tag;
	uint8_t array_value;
	bool fetch_content;
	bool content_is_mandatory;
	uint32_t content_cnt;


	uint8_t respond_code;

	bool error_message;

	union
	{
		struct
		{
			char caller_id[__NODENAME_MAX_LEN__];
			char parameter_key[__ROS_PARAMETER_MAX_LEN__];
		}paramUpdate;

		struct
		{
			char caller_id[__NODENAME_MAX_LEN__];
			char parameter_key[__ROS_PARAMETER_MAX_LEN__];
		}publisherUpdate;

		struct
		{
			char caller_id[__NODENAME_MAX_LEN__];
			char parameter_key[__ROS_PARAMETER_MAX_LEN__];
		}requestTopic;

	};



}xmlrpc_server_data_t;


void xmlrpc_server_handler(parse_act_t * pact);

#endif /* XMLRPC_SERVER_H_ */
