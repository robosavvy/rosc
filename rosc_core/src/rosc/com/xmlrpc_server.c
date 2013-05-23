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
 *  xmlrpc_server.c created by Christian Holl
 */

#ifndef XMLRPC_SERVER_C_
#define XMLRPC_SERVER_C_

#include <rosc/com/xmlrpc_server.h>

bool xmlrpc_server(xmlrpc_server_data_t *data, void** parser_data_ptr)
{

	if(parser_data_ptr!=0)//If not null lets init the stuff
	{
		DEBUG_PRINT_STR("INIT_XMLRPC_SERVER");
		data->parser_data.event=SEBS_PARSE_EVENT_NONE;
		data->parser_data.handler_function=(sebs_parse_handler_function_t)&xmlrpc_server;

		//Select http header parser as current called function on incoming data
		data->parser_data.current_parser.parser_function=(sebs_parse_function_t)&sebs_parse_http;
		data->parser_data.current_parser.parser_data=&data->main_module_data;
		data->parser_data.return_to_handler=false;

		data->main_module_data.http.content_length=-1;
		data->main_module_data.http.descriptor;
		data->main_module_data.http.state=SEBS_PARSE_HTTP_STATE_REQUEST_METHOD;
		data->main_module_data.http.substate=SEBS_PARSE_HTTP_SUBSTATE_STATE_NONE;
		data->main_module_data.http.parser_data=&data->parser_data;

		data->main_module_data.http.descriptors=xmlrpc_http_descriptors;
		data->main_module_data.http.descriptors_len=XMLRPC_HTTP_DESCRIPTORS_LEN;

		data->main_module_data.http.actions=xmlrpc_http_actions;
		data->main_module_data.http.actions_len=XMLRPC_HTTP_ACTIONS_LEN;

		data->main_module_data.http.methods=xmlrpc_http_methods;
		data->main_module_data.http.methods_len=XMLRPC_HTTP_METHODS_LEN;

		//Event = 0
		data->parser_data.event=0;

		*parser_data_ptr=&data->parser_data;

		return (false);
	}


	switch(data->parser_data.event)
	{
	case SEBS_PARSE_EVENT_HTTP_ERROR_CONTENT_LENGTH:


	case SEBS_PARSE_EVENT_HTTP_HEADER_END:
		DEBUG_PRINT_STR("HEADER END!");
		data->parser_data.current_parser.parser_function=(sebs_parse_function_t)&sebs_parse_xml;
		data->parser_data.current_parser.parser_data=&data->main_module_data;

		data->main_module_data.xml.current_tag;
		data->main_module_data.xml.depth=0;
		data->main_module_data.xml.processed_bytes=0;
		data->main_module_data.xml.state=SEBS_PARSE_XML_STATE_ROOT;
		data->main_module_data.xml.substate=SEBS_PARSE_XML_SUBSTATE_NONE;
		data->main_module_data.xml.tag_type=SEBS_PARSE_XML_TAG_NONE;


		data->main_module_data.http.parser_data=&data->parser_data;


		break;
	default:
		break;

	}


	return(false);
}


#endif /* XMLRPC_SERVER_C_ */
