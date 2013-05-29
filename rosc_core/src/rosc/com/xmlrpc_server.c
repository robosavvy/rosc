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

	/* ***************
	 * Initialization<- TODO data input for first initialization*
	 *****************/
	if (in_type_out_parser_data
			!= 0||data->parser_data.event==SEBS_PARSE_EVENT_LEN_SMALLER_ZERO) //If not null lets init the stuff
	{
		DEBUG_PRINT_STR("XMLRPC --- INIT");

		SEBS_PARSE_HTTP_INIT(0, data->parser_data,
				data->parser_data.current_parser, data->http,
				xmlrpc_http_descriptors, XMLRPC_HTTP_DESCRIPTORS_LEN,
				xmlrpc_http_actions, XMLRPC_HTTP_ACTIONS_LEN,
				xmlrpc_http_methods, XMLRPC_HTTP_METHODS_LEN);

		xmlrpc_type_t* type = (xmlrpc_type_t*) *in_type_out_parser_data;
		if (1) //(*type==XMLRPC_SERVER)
		{
			DEBUG_PRINT_STR("INIT_XMLRPC_SERVER");
			data->http.state = SEBS_PARSE_HTTP_STATE_REQUEST_METHOD;
			data->xmlrpc_type = XMLRPC_SERVER;
			data->rpc_methodname = XMLRPC_METHODNAME_UNKNOWN;
		}
		else
		{
			DEBUG_PRINT_STR("INIT_XMLRPC_CLIENT");
			data->http.state = SEBS_PARSE_HTTP_STATE_RESPONSE_HTTP_VER;
			data->xmlrpc_type = XMLRPC_CLIENT;
			data->rpc_methodname = XMLRPC_METHODNAME_UNKNOWN; //TODO must be set!
		}

		data->xmlrpc_state = XMLRPC_STATE_HTTP;
		data->result_handling = XMLRPC_RESULT_NONE;

		data->parser_data.return_to_handler = false;
		data->parser_data.event = SEBS_PARSE_EVENT_NONE;
		data->parser_data.handler_function =
				(sebs_parse_handler_function_t) &xmlrpc;
		data->parser_data.overall_len = 0;
		data->parser_data.security_len = 1024;
		data->parser_data.event = 0;

		xmlrpc_tag_state_t tag_state = XMLRPC_TAG_STATE_NONE;
		xmlrpc_type_tag_t type_tag = XMLRPC_TYPE_TAG_NONE;
		uint8_t param_no = 0;
		uint32_t array_level = 0;
		data->param_no = 0;
		data->method = XMLRPC_METHOD_UNKNOWN;
		data->array_state = XMLRPC_ARRAY_STATE_NONE;

		//set pointer to parser data
		*in_type_out_parser_data = &data->parser_data;

		return (false);
	}

	/* ********************
	 * Handle Frame Events*
	 **********************/
	switch (data->parser_data.event)
	{
	case SEBS_PARSE_EVENT_LEN_SMALLER_ZERO:
		//Should never get here... -> reset is above
		break;
	case SEBS_PARSE_EVENT_HANDLER_CALL_FUNCTION_END:
		switch (data->result_handling)
		{
		case XMLRPC_RESULT_NONE:
			break;
		case XMLRPC_RESULT_CONTENT_LENGTH:
			if (data->http.numberparse.result
					== SEBS_PARSE_NUMBERPARSE_ANOTHER_CHAR)
			{
				DEBUG_PRINT(INT,"CONTENT LENGTH IS: ",data->http.numberparse.number);
				data->xml_length = data->http.numberparse.number;
				return false;
			}
			else
			{
				//TODO ERROR
			}
			break;
		case XMLRPC_RESULT_METHOD_NAME:
			data->rpc_methodname = data->xml.seekstring.result;
			DEBUG_PRINT(STR,"methodName",xmlrpc_slave_methodnames[data->rpc_methodname])
			break;

		case XMLRPC_RESULT_CALLERID:
			DEBUG_PRINT(STR,"caller_id",data->caller_id);
			break;


		default:
			break;
		}
		break;
	case SEBS_PARSE_EVENT_MESSAGE_SECURITY_OVER_SIZE:
		DEBUG_PRINT_STR("---FRAME-->SEBS_PARSE_EVENT_MESSAGE_SECURITY_OVER_SIZE");
		return false;
		break;
		data->result_handling = XMLRPC_RESULT_NONE;
		while (1)
			;
		break;

	default:
		break;
	}

	/* *******************
	 * Handle HTTP Events*
	 *********************/
	if (data->xmlrpc_state == XMLRPC_STATE_HTTP)
	{
		sebs_parse_http_event_t http_event =
				(sebs_parse_http_event_t) data->parser_data.event;
		switch (http_event)
		{

		case SEBS_PARSE_HTTP_EVENT_ERROR_CONTENT_LENGTH_TOO_LONG:
		case SEBS_PARSE_HTTP_EVENT_ERROR_CONTENT_LENGTH:
		case SEBS_PARSE_HTTP_EVENT_ERROR_ACTION_NOT_FOUND:
		case SEBS_PARSE_HTTP_EVENT_ERROR_VERSION_NOT_SUPPORTED:
		case SEBS_PARSE_HTTP_EVENT_ERROR_BAD_REQUEST:
		case SEBS_PARSE_HTTP_EVENT_ERROR_LENGTH_REQUIRED:
		case SEBS_PARSE_HTTP_EVENT_ERROR_METHOD_NOT_ALLOWED:
		case SEBS_PARSE_HTTP_EVENT_ERROR_CONTENT_ENCODING:
		case SEBS_PARSE_HTTP_EVENT_ERROR_BAD_RESPONSE:
			DEBUG_PRINT_STR("---HTTP--->ERRORs...");

			//TODO return error message (server)
			break;

		case SEBS_PARSE_HTTP_EVENT_CONTENT_TYPE:
			DEBUG_PRINT_STR("---HTTP--->SEBS_PARSE_HTTP_EVENT_CONTENT_TYPE");
			break;

		case SEBS_PARSE_HTTP_EVENT_METHOD_PARSED:
			DEBUG_PRINT(INT,"---HTTP--->SEBS_PARSE_HTTP_EVENT_METHOD_PARSED",data->http.seekstring.result);
			data->method = data->http.seekstring.result;
			break;

		case SEBS_PARSE_HTTP_EVENT_ACTION_PARSED:
			DEBUG_PRINT(INT,"---HTTP--->SEBS_PARSE_HTTP_EVENT_ACTION_PARSED",data->http.seekstring.result);
			data->action = data->http.seekstring.result;
			break;

		case SEBS_PARSE_HTTP_EVENT_HEADER_CONTENT:
			DEBUG_PRINT_STR("---HTTP--->SEBS_PARSE_HTTP_EVENT_HEADER_CONTENT");
			switch (data->http.descriptor)
			{
			case XMLRPC_DESCRIPTOR_CONTENT_LENGTH:
				data->result_handling = XMLRPC_RESULT_CONTENT_LENGTH;
				SEBS_PARSE_NUMBERPARSE_INIT(data->parser_data.next_parser,
						data->http.numberparse, 3, false)
				;
				break;
			}
			break;

		case SEBS_PARSE_HTTP_EVENT_RESPONSE_CODE:
			if (data->http.numberparse.result
					== SEBS_PARSE_NUMBERPARSE_ANOTHER_CHAR)
			{
				data->http_response_code = data->http.numberparse.number;
			}
			else
			{
				//TODO Error -- Header Parsing
			}
			break;

		case SEBS_PARSE_HTTP_EVENT_HEADER_END: // GO TO XML
			DEBUG_PRINT_STR("HEADER END!");
			data->xmlrpc_state = XMLRPC_STATE_XML;
			SEBS_PARSE_XML_INIT(data->parser_data.current_parser,
					&data->parser_data, data->xml, xmlrpc_tag_strings,
					XMLRPC_TAG_STRINGS_LEN, xmlrpc_attribute_strings,
					XMLRPC_TAG_STRINGS_LEN)
			;

			data->xml.parser_data = &data->parser_data;
			break;
		default:
			break;
		}
	}

	/* *********************
	 * Handle XMLRPC Events*
	 ***********************/

	else if (data->xmlrpc_state == XMLRPC_STATE_XML)
	{
		sebs_parse_xml_event_t xml_event =
				(sebs_parse_xml_event_t) data->parser_data.event;
		switch (xml_event)
		{

		/*
		 * Unused events
		 */
		case SEBS_PARSE_XML_EVENT_NONE:
			break;
		case SEBS_PARSE_XML_EVENT_HANDLER_CALLED_SUBMODE_FINISHED:
			break;
		case SEBS_PARSE_XML_EVENT_ATTRIBUTE_APOSTROPHE:
		case SEBS_PARSE_XML_EVENT_ATTRIBUTE_QUOTES:
			//Attribute? I don't give a F*ck about the xml version...
			break;

		/*
		 * Extract Data from a CDATA Tag
		 * TODO check out if this is necessary somewhere.
		 */
		case SEBS_PARSE_XML_EVENT_CDATA:
			//Is this used anywhere? Some JSON somewhere I remember... but where?
			break;


		/*
		 * XML Parser Errors
		 */
		case SEBS_PARSE_XML_EVENT_ERROR_DEPTH:
		case SEBS_PARSE_XML_EVENT_ERROR_MALFORMED:
			//TODO error
			break;



		/*
		 * On each tag we check where we are and where we have been before.
		 * this is for easier figuring out, if we will grab some info or not.
		 *
		 * XMLRPC uses value tags which contain specific values according to
		 * their order, so we have to count the tags too.
		 *
		 * ROS uses also some XMLRPC arrays also nested ones.
		 * That makes it also necessary to check in which array level
		 * and there in which array value we are.
		 */
		case SEBS_PARSE_XML_EVENT_INSIDE_TAG:
			//create tag position information
			switch (data->xml.tags[data->xml.depth])
			{
			case XMLRPC_TAG_METHODCALL:
				if (data->xmlrpc_type == XMLRPC_SERVER && data->xml.depth == 1)
				{
					data->tag_state = XMLRPC_TAG_STATE_METHODRC;
				}
				break;
			case XMLRPC_TAG_METHODRESPONSE:
				if (data->xmlrpc_type == XMLRPC_CLIENT && data->xml.depth == 1)
				{
					data->tag_state = XMLRPC_TAG_STATE_METHODRC;
				}
				break;
			case XMLRPC_TAG_PARAMS:
				if (data->tag_state == XMLRPC_TAG_STATE_METHODRC
						&& data->xml.depth == 2)
					data->tag_state = XMLRPC_TAG_STATE_PARAMS;
				break;
			case XMLRPC_TAG_PARAM:
				if (data->tag_state == XMLRPC_TAG_STATE_PARAMS
						&& data->xml.depth == 3)
					data->tag_state = XMLRPC_TAG_STATE_PARAM;
				data->param_no++;
				break;
			case XMLRPC_TAG_VALUE:
				if (data->tag_state == XMLRPC_TAG_STATE_PARAM
						&& data->xml.depth == 4)
				{
					data->tag_state = XMLRPC_TAG_STATE_VALUE;
				}
				else if (data->array_state == XMLRPC_ARRAY_STATE_DATA)
				{
					data->array_state = XMLRPC_ARRAY_STATE_VALUE;
				}
				break;

			case XMLRPC_TAG_ARRAY:
				if (data->tag_state == XMLRPC_TAG_STATE_VALUE)
				{
					if (data->array_state == XMLRPC_ARRAY_STATE_VALUE)
					{
						data->array_level++;
						if (data->array_level < XMLRPC_MAX_ARRAY_NESTING)
							data->array_value_number[data->array_level] = 0;
					}
					data->array_state = XMLRPC_ARRAY_STATE_ARRAY;
				}
				break;
			case XMLRPC_TAG_DATA:
				if (data->array_state == XMLRPC_ARRAY_STATE_ARRAY)
				{
					data->array_state = XMLRPC_ARRAY_STATE_DATA;
				}
				break;

			default:
				break;
			}

			break;

		/*
		 * In here we check for close tags, that's for going back in the state (from above) for
		 * other values or arrays.
		 */
		case SEBS_PARSE_XML_EVENT_TAG:
			if (data->xml.tag_type == SEBS_PARSE_XML_TAG_TYPE_CLOSE)
			{
				switch (data->xml.tags[data->xml.depth])
				{
				case XMLRPC_TAG_VALUE:
					if (data->tag_state == XMLRPC_TAG_STATE_VALUE
							&& data->xml.depth == 4)
					{
						data->tag_state = XMLRPC_TAG_STATE_PARAM;
					}
					else if (data->array_state == XMLRPC_ARRAY_STATE_VALUE)
					{
						data->array_state = XMLRPC_ARRAY_STATE_DATA;
						if (data->array_level < XMLRPC_MAX_ARRAY_NESTING)
							data->array_value_number[data->array_level]++;
					}
					break;

				case XMLRPC_TAG_DATA:
					if (data->array_state == XMLRPC_ARRAY_STATE_DATA)
					{
						data->array_state = XMLRPC_ARRAY_STATE_ARRAY;
					}
					break;

				case XMLRPC_TAG_ARRAY:
					if (data->array_state == XMLRPC_ARRAY_STATE_ARRAY)
					{
						if (data->array_level == 0)
						{
							data->array_state = XMLRPC_ARRAY_STATE_NONE;
						}
						else
						{
							data->array_state = XMLRPC_ARRAY_STATE_VALUE;
							data->array_level--;
						}
					}

				case XMLRPC_TAG_PARAM:
					if (data->tag_state == XMLRPC_TAG_STATE_PARAM
							&& data->xml.depth == 3)
						data->tag_state = XMLRPC_TAG_STATE_PARAMS;
					break;

				case XMLRPC_TAG_PARAMS:
					if (data->tag_state == XMLRPC_TAG_STATE_PARAMS
							&& data->xml.depth == 2)
						data->tag_state = XMLRPC_TAG_STATE_METHODRC;
					break;

				case XMLRPC_TAG_METHODCALL:
				case XMLRPC_TAG_METHODRESPONSE:
					if (data->tag_state == XMLRPC_TAG_STATE_METHODRC
							&& data->xml.depth == 1)
						data->tag_state = XMLRPC_TAG_STATE_NONE;
					break;
				default:
					break;
				}
			}

			break;


		/*
		 * And here we start extraction methods for the information we are
		 * really interested in:
		 */
		case SEBS_PARSE_XML_EVENT_CONTENT_START:
		{
			/*
			 * Methodname, for knowing what the supplied values stand for,
			 * the methodname is required first!
			 */
			if (data->xml.tags[data->xml.depth] == XMLRPC_TAG_METHODNAME &&
				data->tag_state==XMLRPC_TAG_STATE_METHODRC)
			{
				data->result_handling = XMLRPC_RESULT_METHOD_NAME;
				SEBS_PARSE_SEEKSTRING_INIT(data->parser_data.next_parser,
						data->xml.seekstring, xmlrpc_slave_methodnames,
						XMLRPC_SLAVE_METHODNAMES_LEN, "<,:>/", true);
			}

			if (data->xmlrpc_type == XMLRPC_SERVER)
			{
				//The first field is always the caller_id in every known methodcall
				//so lets extract as many chars as possible
				//The caller
#ifndef ROSC_NO_CALLERID_EXTRACTION
				/*
				 * I haven't seen a use for the caller-id so far, that's
				 * why you can disable it here. If it must be used somewhere
				 * just feel free to tell me. -> Christian Holl.
				 * ROSc will use it for error reporting so far...
				 */
				if(data->tag_state==XMLRPC_TAG_STATE_VALUE &&
				   data->param_no==1 &&
				   data->array_state==XMLRPC_ARRAY_STATE_NONE)
				{
					data->result_handling=XMLRPC_RESULT_CALLERID;
					SEBS_PARSE_COPY2BUFFER_INIT(data->parser_data.next_parser, data->copy2buffer,data->caller_id,__NODENAME_MAX_LEN__,"<");
				}
#endif
				else
				{
					switch (data->rpc_methodname)
					{
					case XMLRPC_METHODNAME_UNKNOWN:
						DEBUG_PRINT_STR("UNKNOWN METHODNAME!")
						//TODO
						break;

					}
				}
			}
			else if (data->xmlrpc_type == XMLRPC_CLIENT)
			{

			}
			break;
		}

	default:
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
