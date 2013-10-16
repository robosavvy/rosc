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

#ifndef XMLRPC_C_
#define XMLRPC_C_

#include <rosc/com/xmlrpc.h>
#include <rosc/system/status.h>
#include <rosc/com/msg_gen.h>




sebs_parse_return_t xmlrpc(sebs_parser_data_t* pdata)
{


	xmlrpc_data_t *hdata=pdata->handler_data;
	xmlrpc_t init_mode=((xmlrpc_init_data_t *)pdata->init_data)->type;
	/* ***************
	 * Initialization<- TODO data input for first initialization*
	 *****************/
	if(pdata->handler_init)
	{
		pdata->handler_init=false;

		DEBUG_PRINT_STR("XMLRPC --- INIT");

		uint8_t init_state;


		if (init_mode == XMLRPC_TYPE_SERVER)
		{
			DEBUG_PRINT_STR("INIT_XMLRPC_SERVER");
			init_state = SEBS_PARSE_HTTP_REQUEST_INIT;
		}
		else if (init_mode == XMLRPC_TYPE_CLIENT)
		{
			DEBUG_PRINT_STR("INIT_XMLRPC_TYPE_CLIENT");
			init_state = SEBS_PARSE_HTTP_RESPONSE_INIT;
		}
		else
		{
			ROSC_FATAL("ERROR XMLRPC Handler: Unknown XMLRPC type given in init!!!");
		}



		hdata->rpc_methodname = XMLRPC_METHODNAME_UNKNOWN;
		hdata->xmlrpc_state = XMLRPC_STATE_HTTP;
		hdata->result_handling = XMLRPC_RESULT_NONE;
		pdata->overall_len = 0;
		pdata->security_len = XMLRPC_SECURITY_MAX_MESSAGE_SIZE;
		pdata->out_len = SOCKET_NO_DATA;

		xmlrpc_tag_state_t tag_state = XMLRPC_TAG_STATE_NONE;
		xmlrpc_type_tag_t type_tag = XMLRPC_TYPE_TAG_NONE;
		uint8_t param_no = 0;
		uint32_t array_level = 0;
		hdata->param_no = 0;
		hdata->method = XMLRPC_METHOD_UNKNOWN;
		hdata->array_state = XMLRPC_ARRAY_STATE_NONE;
		hdata->array_level=0;
		hdata->type_tag = XMLRPC_TYPE_TAG_NONE;


		SEBS_PARSE_HTTP_INIT(pdata, hdata->http,
				init_state,
				xmlrpc_http_descriptors, XMLRPC_HTTP_DESCRIPTORS_LEN,
				xmlrpc_http_actions, XMLRPC_HTTP_ACTIONS_LEN,
				xmlrpc_http_methods, XMLRPC_HTTP_METHODS_LEN);
	}

	/* ********************
	 * Handle Frame Events*
	 **********************/
	switch (pdata->event)
	{
	case SEBS_PARSE_EVENT_LEN_EQUAL_SMALLER_ZERO:
			DEBUG_PRINT_STR("EVENT LEN SMALLER_ZERO");
			switch(*pdata->len)
			{
				case 0:
					DEBUG_PRINT_STR("Connection closed!!!");
					pdata->out_len=SOCKET_CLOSED;
					break;
				case -1:
					return (SEBS_PARSE_RETURN_GO_AHEAD);
					break;
				default:
					DEBUG_PRINT(INT,"LEN is",*pdata->len);
					break;
			}
		break;
	case SEBS_PARSE_EVENT_HANDLER_CALL_FUNCTION_END:
		switch (hdata->result_handling)
		{
		case XMLRPC_RESULT_NONE:
			break;
		case XMLRPC_RESULT_CONTENT_LENGTH:
			if (hdata->http.numberparse.result
					== SEBS_PARSE_NUMBERPARSE_ANOTHER_CHAR)
			{
				DEBUG_PRINT(INT,"CONTENT LENGTH IS: ",hdata->http.numberparse.number);
				hdata->xml_length = hdata->http.numberparse.number;
				return (false);
			}
			else
			{
				//TODO ERROR
			}
			break;
		case XMLRPC_RESULT_METHOD_NAME:
			hdata->rpc_methodname = hdata->xml.seekstring.result;
			DEBUG_PRINT(STR,"methodName",xmlrpc_slave_methodnames[hdata->rpc_methodname])
			break;

		case XMLRPC_RESULT_CALLERID:
			DEBUG_PRINT(STR,"caller_id",hdata->caller_id);
			break;

		case XMLRPC_RESULT_PUBLISHER_UPDATE_URL:
			DEBUG_PRINT(INT,"Port",hdata->url.port);
			DEBUG_PRINT(STR,"Scheme",hdata->url.scheme_list[hdata->url.url_scheme]);
			DEBUG_PRINT(STR,"Hostname",hdata->url.hostname_buf);
			DEBUG_PRINT(INT,"RESULT",hdata->url.result);
			break;

		default:
			break;
		}
		break;
	case SEBS_PARSE_EVENT_MESSAGE_SECURITY_OVER_SIZE:
		DEBUG_PRINT_STR("---FRAME-->SEBS_PARSE_EVENT_MESSAGE_SECURITY_OVER_SIZE");
		return (false);
		break;
		hdata->result_handling = XMLRPC_RESULT_NONE;
		while (1)
			;
		break;

	default:
		break;
	}

	/* *******************
	 * Handle HTTP Events*
	 *********************/
	if (hdata->xmlrpc_state == XMLRPC_STATE_HTTP)
	{
		sebs_parse_http_event_t http_event =
				(sebs_parse_http_event_t) pdata->event;
		switch (http_event)
		{


		case SEBS_PARSE_HTTP_EVENT_CONTENT_TYPE:
			DEBUG_PRINT_STR("---HTTP--->SEBS_PARSE_HTTP_EVENT_CONTENT_TYPE");
			break;

		case SEBS_PARSE_HTTP_EVENT_METHOD_PARSED:
			DEBUG_PRINT(INT,"---HTTP--->SEBS_PARSE_HTTP_EVENT_METHOD_PARSED",hdata->http.seekstring.result);
			hdata->method = hdata->http.seekstring.result;

			break;

		case SEBS_PARSE_HTTP_EVENT_ACTION_PARSED:
			DEBUG_PRINT(INT,"---HTTP--->SEBS_PARSE_HTTP_EVENT_ACTION_PARSED",hdata->http.seekstring.result);
			hdata->action = hdata->http.seekstring.result;
			break;

		case SEBS_PARSE_HTTP_EVENT_HEADER_CONTENT:
			DEBUG_PRINT_STR("---HTTP--->SEBS_PARSE_HTTP_EVENT_HEADER_CONTENT");
			switch (hdata->http.descriptor)
			{
			case XMLRPC_DESCRIPTOR_CONTENT_LENGTH:
				hdata->result_handling = XMLRPC_RESULT_CONTENT_LENGTH;
				SEBS_PARSE_NUMBERPARSE_INIT(pdata,
						hdata->http.numberparse, 3, false,10)
				;
				break;
			}
			break;

		case SEBS_PARSE_HTTP_EVENT_RESPONSE_CODE:
			if (hdata->http.numberparse.result
					== SEBS_PARSE_NUMBERPARSE_ANOTHER_CHAR)
			{
				hdata->http_response_code = hdata->http.numberparse.number;
			}
			else
			{
				//TODO Error -- Header Parsing
			}
			break;

		case SEBS_PARSE_HTTP_EVENT_HEADER_END: // GO TO XML
			DEBUG_PRINT_STR("HEADER END!");
			hdata->xmlrpc_state = XMLRPC_STATE_XML;
			SEBS_PARSE_XML_INIT(pdata, hdata->xml, xmlrpc_tag_strings,
					XMLRPC_TAG_STRINGS_LEN, xmlrpc_attribute_strings,
					XMLRPC_TAG_STRINGS_LEN)
			;

			break;


		case SEBS_PARSE_HTTP_EVENT_ERROR_CONTENT_LENGTH_TOO_LONG:
		case SEBS_PARSE_HTTP_EVENT_ERROR_CONTENT_LENGTH:
		case SEBS_PARSE_HTTP_EVENT_ERROR_ACTION_NOT_FOUND:
		case SEBS_PARSE_HTTP_EVENT_ERROR_VERSION_NOT_SUPPORTED:
		case SEBS_PARSE_HTTP_EVENT_ERROR_BAD_REQUEST:
		case SEBS_PARSE_HTTP_EVENT_ERROR_METHOD_NOT_ALLOWED:
		case SEBS_PARSE_HTTP_EVENT_ERROR_CONTENT_ENCODING:
		case SEBS_PARSE_HTTP_EVENT_ERROR_BAD_RESPONSE:
			DEBUG_PRINT_STR("---HTTP--->ERRORs...");
			pdata->out_len=SOCKET_CLOSED;
			*pdata->len=0;
			break;
		default:
			break;
		}
	}

	/* *********************
	 * Handle XMLRPC Events*
	 ***********************/

	else if (hdata->xmlrpc_state == XMLRPC_STATE_XML)
	{
		sebs_parse_xml_event_t xml_event =
				(sebs_parse_xml_event_t) pdata->event;
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
			switch (hdata->xml.tags[hdata->xml.depth])
			{
			case XMLRPC_TAG_METHODCALL:
				if (init_mode == XMLRPC_TYPE_SERVER && hdata->xml.depth == 1)
				{
					hdata->tag_state = XMLRPC_TAG_STATE_METHODRC;
				}
				break;
			case XMLRPC_TAG_METHODRESPONSE:
				if (init_mode == XMLRPC_TYPE_CLIENT && hdata->xml.depth == 1)
				{
					hdata->tag_state = XMLRPC_TAG_STATE_METHODRC;
				}
				break;
			case XMLRPC_TAG_PARAMS:
				if (hdata->tag_state == XMLRPC_TAG_STATE_METHODRC
						&& hdata->xml.depth == 2)
					hdata->tag_state = XMLRPC_TAG_STATE_PARAMS;
				break;
			case XMLRPC_TAG_PARAM:
				if (hdata->tag_state == XMLRPC_TAG_STATE_PARAMS
						&& hdata->xml.depth == 3)
					hdata->tag_state = XMLRPC_TAG_STATE_PARAM;
				hdata->param_no++;
				break;
			case XMLRPC_TAG_VALUE:
				if (hdata->tag_state == XMLRPC_TAG_STATE_PARAM
						&& hdata->xml.depth == 4)
				{
					hdata->tag_state = XMLRPC_TAG_STATE_VALUE;
				}
				else if (hdata->array_state == XMLRPC_ARRAY_STATE_DATA)
				{
					hdata->array_state = XMLRPC_ARRAY_STATE_VALUE;
				}
				break;

			case XMLRPC_TAG_ARRAY:
				if (hdata->tag_state == XMLRPC_TAG_STATE_VALUE)
				{
					if (hdata->array_state == XMLRPC_ARRAY_STATE_VALUE)
					{
						hdata->array_level++;
						if (hdata->array_level < XMLRPC_MAX_ARRAY_NESTING)
							hdata->array_value_number[hdata->array_level] = 0;
					}
					hdata->array_state = XMLRPC_ARRAY_STATE_ARRAY;
				}
				break;
			case XMLRPC_TAG_DATA:
				if (hdata->array_state == XMLRPC_ARRAY_STATE_ARRAY)
				{
					hdata->array_state = XMLRPC_ARRAY_STATE_DATA;
				}
				break;


			case XMLRPC_TAG_STRING:
				hdata->type_tag=XMLRPC_TYPE_TAG_STRING;
				break;
			case XMLRPC_TAG_I4:
			case XMLRPC_TAG_INT:
				hdata->type_tag=XMLRPC_TYPE_TAG_INT;
				break;
			case XMLRPC_TAG_BOOLEAN:
				hdata->type_tag=XMLRPC_TYPE_TAG_BOOLEAN;
				break;
			case XMLRPC_TAG_DOUBLE:
				hdata->type_tag=XMLRPC_TYPE_TAG_DOUBLE;
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
			if (hdata->xml.tag_type == SEBS_PARSE_XML_TAG_TYPE_CLOSE)
			{
				switch (hdata->xml.tags[hdata->xml.depth])
				{
				case XMLRPC_TAG_VALUE:
					if (hdata->tag_state == XMLRPC_TAG_STATE_VALUE
							&& hdata->xml.depth == 4)
					{
						hdata->tag_state = XMLRPC_TAG_STATE_PARAM;
					}
					else if (hdata->array_state == XMLRPC_ARRAY_STATE_VALUE)
					{
						hdata->array_state = XMLRPC_ARRAY_STATE_DATA;
						if (hdata->array_level < XMLRPC_MAX_ARRAY_NESTING)
							hdata->array_value_number[hdata->array_level]++;
					}
					break;

				case XMLRPC_TAG_DATA:
					if (hdata->array_state == XMLRPC_ARRAY_STATE_DATA)
					{
						hdata->array_state = XMLRPC_ARRAY_STATE_ARRAY;
					}
					break;

				case XMLRPC_TAG_ARRAY:
					if (hdata->array_state == XMLRPC_ARRAY_STATE_ARRAY)
					{
						if (hdata->array_level == 0)
						{
							hdata->array_state = XMLRPC_ARRAY_STATE_NONE;
						}
						else
						{
							hdata->array_state = XMLRPC_ARRAY_STATE_VALUE;
							hdata->array_level--;
						}
					}

				case XMLRPC_TAG_PARAM:
					if (hdata->tag_state == XMLRPC_TAG_STATE_PARAM
							&& hdata->xml.depth == 3)
						hdata->tag_state = XMLRPC_TAG_STATE_PARAMS;
					break;

				case XMLRPC_TAG_PARAMS:
					if (hdata->tag_state == XMLRPC_TAG_STATE_PARAMS
							&& hdata->xml.depth == 2)
						hdata->tag_state = XMLRPC_TAG_STATE_METHODRC;
					break;

				case XMLRPC_TAG_METHODCALL:
				case XMLRPC_TAG_METHODRESPONSE:
					if (hdata->tag_state == XMLRPC_TAG_STATE_METHODRC
							&& hdata->xml.depth == 1)
						hdata->tag_state = XMLRPC_TAG_STATE_NONE;
					break;

				case XMLRPC_TAG_STRING:
				case XMLRPC_TAG_I4:
				case XMLRPC_TAG_INT:
				case XMLRPC_TAG_BOOLEAN:
				case XMLRPC_TAG_DOUBLE:
					hdata->type_tag=XMLRPC_TYPE_TAG_NONE;
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
			if (hdata->xml.tags[hdata->xml.depth] == XMLRPC_TAG_METHODNAME
					&& hdata->tag_state == XMLRPC_TAG_STATE_METHODRC)
			{
				hdata->result_handling = XMLRPC_RESULT_METHOD_NAME;
				SEBS_PARSE_SEEKSTRING_INIT(pdata,
						hdata->xml.seekstring, xmlrpc_slave_methodnames,
						XMLRPC_SLAVE_METHODNAMES_LEN, "<,:>/", true,0);
			}

			if (init_mode == XMLRPC_TYPE_SERVER)
			{
				//The first field is always the caller_id in every known methodcall
				//so lets extract as many chars as possible
				//The caller
#ifndef ROSC_NO_CALLERID_EXTRACTION
				/*
				 * I haven't seen a use for the caller-id so far, that's
				 * why you can disable it here. If it must be used somewhere
				 * just feel free to tell...
				 * ROSc will use it for error reporting so far...
				 */
				if (hdata->tag_state == XMLRPC_TAG_STATE_VALUE
						&& hdata->param_no == 1
						&& hdata->array_state == XMLRPC_ARRAY_STATE_NONE)
				{
					if (hdata->type_tag == XMLRPC_TYPE_TAG_NONE
							|| hdata->type_tag == XMLRPC_TYPE_TAG_STRING)
					{
						hdata->result_handling = XMLRPC_RESULT_CALLERID;
						SEBS_PARSE_COPY2BUFFER_INIT(pdata,
								hdata->copy2buffer, hdata->caller_id,
								__NODENAME_MAX_LEN__, "<",0,1,0);
					}
					else
					{
						//TODO ERROR: WRONG PARAMETER TYPE GIVEN!
					}
				}
#endif
				else
				{
					switch (hdata->rpc_methodname)
					{
						case XMLRPC_METHODNAME_UNKNOWN:
						DEBUG_PRINT_STR("UNKNOWN METHODNAME!");
						//TODO
						break;
						/*
						 * Those methods have only one parameter (caller_id)
						 */
						case XMLRPC_METHODNAME_GETBUSINFO:
						case XMLRPC_METHODNAME_GETBUSSTATS:
						case XMLRPC_METHODNAME_GETMASTERURI:
						case XMLRPC_METHODNAME_GETPID:
						case XMLRPC_METHODNAME_GETPUBLICATIONS:
						case XMLRPC_METHODNAME_GETSUBSCRIPTIONS:
						break;

						/*
						 * Those methods have multiple parameters
						 */
						case XMLRPC_METHODNAME_PARAMUPDATE:
						break;
						case XMLRPC_METHODNAME_PUBLISHERUPDATE:
						//Second parameter is topicname
						if (hdata->tag_state == XMLRPC_TAG_STATE_VALUE
								&& hdata->param_no == 2
								&& hdata->array_state == XMLRPC_ARRAY_STATE_NONE)
						{
							if(hdata->type_tag == XMLRPC_TYPE_TAG_NONE
									|| hdata->type_tag == XMLRPC_TYPE_TAG_STRING)
							{
								DEBUG_PRINT_STR("TOPIC");
							}
							else
							{
								//TODO ERROR: WRONG PARAMETER TYPE GIVEN!
							}
						}

						if (hdata->tag_state == XMLRPC_TAG_STATE_VALUE
								&& hdata->param_no == 3
								&& hdata->array_state == XMLRPC_ARRAY_STATE_VALUE
								&& hdata->array_level == 0
							)
						{
							DEBUG_PRINT_STR("A PUBLISHER");
							hdata->result_handling=XMLRPC_RESULT_PUBLISHER_UPDATE_URL;
							SEBS_PARSE_URL_INIT(pdata,hdata->url,0,xmlrpc_url_scheme_string,XMLRPC_URL_SCHEME_STRING_LEN);
						}
						break;
						case XMLRPC_METHODNAME_REQUESTTOPIC:
						break;
						case XMLRPC_METHODNAME_SHUTDOWN:
						break;
					}
				}
			}
			else if (init_mode == XMLRPC_TYPE_CLIENT)
			{

			}
			break;
		}

		default:
			break;
		}
	}
	else if (hdata->xmlrpc_state == XMLRPC_STATE_RESPOND)
	{

	}
	else
	{
		ROSC_FATAL("xmlrpc_state value unexpected!");
	}





	return (SEBS_PARSE_RETURN_GO_AHEAD);
}

#endif /* XMLRPC_C_ */
