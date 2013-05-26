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
#include <rosc/system/status.h>


bool xmlrpc(xmlrpc_data_t *data, void** in_type_out_parser_data)
{

	if (in_type_out_parser_data
			!= 0||data->parser_data.event==SEBS_PARSE_EVENT_LEN_SMALLER_ZERO) //If not null lets init the stuff
	{
		DEBUG_PRINT_STR("XMLRPC --- INIT");

		SEBS_PARSE_HTTP_INIT(0, data->parser_data,
				data->parser_data.current_parser, data->main_module_data.http,
				xmlrpc_http_descriptors, XMLRPC_HTTP_DESCRIPTORS_LEN,
				xmlrpc_http_actions, XMLRPC_HTTP_ACTIONS_LEN,
				xmlrpc_http_methods, XMLRPC_HTTP_METHODS_LEN);

		xmlrpc_init_t* type = (xmlrpc_init_t*) *in_type_out_parser_data;
		if (1) //(*type==XMLRPC_SERVER)
		{
			DEBUG_PRINT_STR("INIT_XMLRPC_SERVER");
			data->main_module_data.http.state =
					SEBS_PARSE_HTTP_STATE_REQUEST_METHOD;
		}
		else
		{
			DEBUG_PRINT_STR("INIT_XMLRPC_CLIENT");
			data->main_module_data.http.state =
					SEBS_PARSE_HTTP_STATE_RESPONSE_HTTP_VER;
		}

		data->parser_data.return_to_handler = false;
		data->parser_data.event = SEBS_PARSE_EVENT_NONE;
		data->parser_data.handler_function =
				(sebs_parse_handler_function_t) &xmlrpc;
		data->xmlrpc_state = XMLRPC_STATE_HTTP;
		data->result_handling = XMLRPC_RESULT_NONE;

		//Event = 0
		data->parser_data.event = 0;

		//set pointer to parser data
		*in_type_out_parser_data = &data->parser_data;

		return (false);
	}

	switch (data->parser_data.event)
	{
	case SEBS_PARSE_EVENT_LEN_SMALLER_ZERO:
		DEBUG_PRINT_STR("---FRAME-->DAFUQ? SHOULDN'T COME HERE, SEBS_PARSE_EVENT_LEN_SMALLER_ZERO");
		//Should never get here... -> reset is above
		break;
	case SEBS_PARSE_EVENT_HANDLER_CALL_FUNCTION_END:
		DEBUG_PRINT_STR("---FRAME-->SEBS_PARSE_EVENT_HANDLER_CALL_FUNCTION_END");

		switch(data->result_handling)
		{
		case XMLRPC_RESULT_NONE:
			DEBUG_PRINT_STR(".......................................................................................");
			break;
		case XMLRPC_RESULT_CONTENT_LENGTH:
				if(data->main_module_data.http.std_func_data.numberparse.result==NUMBERPARSE_ANOTHER_CHAR)
				{
					DEBUG_PRINT(INT,"CONTENT LENGTH IS: ",data->main_module_data.http.std_func_data.numberparse.number);
					return false;
				}
				else
				{

				}
			break;
		}
		data->result_handling=XMLRPC_RESULT_NONE;
		while(1);
		break;

	default:
		break;
	}

	if (data->xmlrpc_state == XMLRPC_STATE_HTTP)
	{
		sebs_parse_http_event_t http_event =
				(sebs_parse_http_event_t) data->parser_data.event;
		switch (http_event)
		{

		case SEBS_PARSE_HTTP_EVENT_ERROR_CONTENT_LENGTH_TOO_LONG:
			DEBUG_PRINT_STR("---HTTP--->SEBS_PARSE_HTTP_EVENT_ERROR_CONTENT_LENGTH_TOO_LONG");
			break;
		case SEBS_PARSE_HTTP_EVENT_ERROR_CONTENT_LENGTH:
			DEBUG_PRINT_STR("---HTTP--->SEBS_PARSE_HTTP_EVENT_ERROR_CONTENT_LENGTH");
			break;
		case SEBS_PARSE_HTTP_EVENT_ERROR_ACTION_NOT_FOUND:
			DEBUG_PRINT_STR("---HTTP--->SEBS_PARSE_HTTP_EVENT_ERROR_ACTION_NOT_FOUND");
			break;
		case SEBS_PARSE_HTTP_EVENT_ERROR_VERSION_NOT_SUPPORTED:
			DEBUG_PRINT_STR("---HTTP--->SEBS_PARSE_HTTP_EVENT_ERROR_VERSION_NOT_SUPPORTED");
			break;
		case SEBS_PARSE_HTTP_EVENT_ERROR_BAD_REQUEST:
			DEBUG_PRINT_STR("---HTTP--->SEBS_PARSE_HTTP_EVENT_ERROR_BAD_REQUEST");
			break;
		case SEBS_PARSE_HTTP_EVENT_ERROR_LENGTH_REQUIRED:
			DEBUG_PRINT_STR("---HTTP--->SEBS_PARSE_HTTP_EVENT_ERROR_LENGTH_REQUIRED");
			break;
		case SEBS_PARSE_HTTP_EVENT_ERROR_METHOD_NOT_ALLOWED:
			DEBUG_PRINT_STR("---HTTP--->SEBS_PARSE_HTTP_EVENT_ERROR_METHOD_NOT_ALLOWED");
			break;
		case SEBS_PARSE_HTTP_EVENT_CONTENT_TYPE:
			DEBUG_PRINT_STR("---HTTP--->SEBS_PARSE_HTTP_EVENT_CONTENT_TYPE");
			break;
		case SEBS_PARSE_HTTP_EVENT_ERROR_CONTENT_ENCODING:
			DEBUG_PRINT_STR("---HTTP--->SEBS_PARSE_HTTP_EVENT_ERROR_CONTENT_ENCODING");
			break;
		case SEBS_PARSE_HTTP_EVENT_ERROR_BAD_RESPONSE:
			DEBUG_PRINT_STR("---HTTP--->SEBS_PARSE_HTTP_EVENT_ERROR_BAD_RESPONSE");
			break;
		case SEBS_PARSE_HTTP_EVENT_METHOD_PARSED:
			DEBUG_PRINT_STR("---HTTP--->SEBS_PARSE_HTTP_EVENT_METHOD_PARSED");
			break;
		case SEBS_PARSE_HTTP_EVENT_ACTION_PARSED:
			DEBUG_PRINT_STR("---HTTP--->SEBS_PARSE_HTTP_EVENT_ACTION_PARSED");
			break;
		case SEBS_PARSE_HTTP_EVENT_HEADER_CONTENT:
			DEBUG_PRINT_STR("---HTTP--->SEBS_PARSE_HTTP_EVENT_HEADER_CONTENT");
			switch(data->main_module_data.http.descriptor)
			{
			case XMLRPC_DESCRIPTOR_CONTENT_LENGTH:
					data->result_handling=XMLRPC_RESULT_CONTENT_LENGTH;
					SEBS_PARSE_NUMBERPARSE_INIT(data->parser_data.next_parser,
												data->main_module_data.http.std_func_data.numberparse,3,false);
				break;
			}

			break;
		case SEBS_PARSE_HTTP_EVENT_RESPONSE_CODE:
			DEBUG_PRINT_STR("---HTTP--->SEBS_PARSE_HTTP_EVENT_RESPONSE_CODE");
			break;

		case SEBS_PARSE_HTTP_EVENT_HEADER_END: // GO TO XML
			DEBUG_PRINT_STR("HEADER END!");
			data->xmlrpc_state=XMLRPC_STATE_XML;
			SEBS_PARSE_XML_INIT(data->parser_data.current_parser,
					&data->parser_data, data->main_module_data.xml,
					xmlrpc_tag_strings, XMLRPC_TAG_STRINGS_LEN,
					xmlrpc_attribute_strings, XMLRPC_TAG_STRINGS_LEN)
			;

			data->main_module_data.xml.parser_data = &data->parser_data;
			break;
		default:
			break;
		}
	}
	else if (data->xmlrpc_state == XMLRPC_STATE_XML)
	{
		sebs_parse_xml_event_t xml_event =
				(sebs_parse_xml_event_t) data->parser_data.event;
		switch (xml_event)
		{

		case SEBS_PARSE_XML_EVENT_ATTRIBUTE_APOSTROPHE:
			DEBUG_PRINT_STR("---XML-------->SEBS_PARSE_XML_EVENT_ATTRIBUTE_APOSTROPHE ");
			break;
		case SEBS_PARSE_XML_EVENT_ATTRIBUTE_QUOTES:
			DEBUG_PRINT_STR("---XML-------->SEBS_PARSE_XML_EVENT_ATTRIBUTE_QUOTES ");
			break;
		case SEBS_PARSE_XML_EVENT_CDATA:
			DEBUG_PRINT_STR("---XML-------->SEBS_PARSE_XML_EVENT_CDATA ");
			break;
		case SEBS_PARSE_XML_EVENT_NONE:
			DEBUG_PRINT_STR("---XML-------->SEBS_PARSE_XML_EVENT_NONE ");
			break;
		case SEBS_PARSE_XML_EVENT_ERROR_DEPTH:
			DEBUG_PRINT_STR("---XML-------->SEBS_PARSE_XML_EVENT_ERROR_DEPTH ");
			break;
		case SEBS_PARSE_XML_EVENT_ERROR_MALFORMED:
			DEBUG_PRINT_STR("---XML-------->SEBS_PARSE_XML_EVENT_ERROR_MALFORMED ");
			break;
		case SEBS_PARSE_XML_EVENT_TAG:
			DEBUG_PRINT_STR("---XML-------->SEBS_PARSE_XML_EVENT_TAG ");
			break;
		case SEBS_PARSE_XML_EVENT_INSIDE_TAG:
			DEBUG_PRINT_STR("---XML-------->SEBS_PARSE_XML_EVENT_INSIDE_TAG ");
			break;
		case SEBS_PARSE_XML_EVENT_HANDLER_CALLED_SUBMODE_FINISHED:
			DEBUG_PRINT_STR("---XML-------->SEBS_PARSE_XML_EVENT_HANDLER_CALLED_SUBMODE_FINISHED ");
			break;
		case SEBS_PARSE_XML_EVENT_CONTENT_START:
			DEBUG_PRINT_STR("---XML-------->SEBS_PARSE_XML_EVENT_CONTENT_START ");
			break;
		default:
			DEBUG_PRINT_STR("---XML-------->UNKNOWN XML EVENT!");
			break;
		}
	}
	else
	{
		ROSC_FATAL("xmlrpc_state value unexpected!");
	}
	return (false);
}

#endif /* XMLRPC_SERVER_C_ */
