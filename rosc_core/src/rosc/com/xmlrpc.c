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



enum
{
	HTTP_HEADER_STDTEXT(HD_TXT)
};

#define RESPOND()\
hdata->xmlrpc_state = XMLRPC_STATE_RESPOND;

sebs_parse_return_t xmlrpc(sebs_parser_data_t* pdata)
{
	xmlrpc_data_t *hdata = pdata->handler_data;
	iface_t *iface=pdata->init_data;




	/* ***************
	 * Initialization<- TODO data input for first initialization*
	 *****************/
	if (pdata->handler_init)
	{
		pdata->handler_init = false;

		DEBUG_PRINT_STR("XMLRPC --- INIT");

		pdata->sending = false;

		hdata->rpc_methodname = XMLRPC_METHODNAME_UNKNOWN;
		hdata->xmlrpc_state = XMLRPC_STATE_HTTP;
		hdata->result_handling = XMLRPC_RESULT_NONE;
		pdata->overall_len = 0;
		pdata->security_len = XMLRPC_SECURITY_MAX_MESSAGE_SIZE;
		pdata->out_len = SOCKET_SIG_NO_DATA;

		xmlrpc_tag_state_t tag_state = XMLRPC_TAG_STATE_NONE;
		xmlrpc_type_tag_t type_tag = XMLRPC_TYPE_TAG_NONE;
		uint8_t param_no = 0;
		uint32_t array_level = 0;
		hdata->param_no = 0;
		hdata->method = XMLRPC_METHOD_UNKNOWN;
		hdata->array_state = XMLRPC_ARRAY_STATE_NONE;
		hdata->array_level = 0;
		hdata->array_value_number[0] = 0;
		hdata->type_tag = XMLRPC_TYPE_TAG_NONE;

		if (pdata->init_data == 0)
		{
			DEBUG_PRINT_STR("INIT_XMLRPC_SERVER");
			hdata->xmlrpc_type = XMLRPC_TYPE_SERVER;
			hdata->xmlrpc_state=XMLRPC_STATE_HTTP;
			SEBS_PARSE_HTTP_INIT(pdata, hdata->http,
					SEBS_PARSE_HTTP_REQUEST_INIT, xmlrpc_http_descriptors,
					XMLRPC_HTTP_DESCRIPTORS_LEN, xmlrpc_http_actions,
					XMLRPC_HTTP_ACTIONS_LEN, xmlrpc_http_methods,
					XMLRPC_HTTP_METHODS_LEN);
		}
		else
		{
			DEBUG_PRINT_STR("INIT_XMLRPC_TYPE_CLIENT");
			hdata->xmlrpc_type = XMLRPC_TYPE_CLIENT;

			hdata->xmlrpc_state = XMLRPC_STATE_CONNECT;
			socket_t *socket=pdata->connection_interface;

			switch(hdata->client_type)
			{
				case XMLRPC_CLIENT_TYPE_REGISTER:
					strcpy(socket->connect_info.url,master_uri); //TODO ... check if we replace this by our own implementation
				break;

				default:
					break;
			}

			socket->connect_info.data_state=CONNECT_DATA_STATE_URL;
			SOCKET_CONNECT_INIT(pdata,hdata->connect,&socket->connect_info);
		}
	}


	/* ********************
	 * Handle Frame Events*
	 **********************/
	switch (pdata->event)
	{
	case SEBS_PARSE_EVENT_LEN_EQUAL_SMALLER_ZERO:
		switch (*pdata->len)
		{

		case SOCKET_SIG_CLOSE:
			DEBUG_PRINT_STR("XMLRPC Connection close");
				pdata->out_len = SOCKET_SIG_RELEASE;
				return (SEBS_PARSE_RETURN_GO_AHEAD);

		case SOCKET_SIG_TIMEOUT:
			DEBUG_PRINT_STR("XMLRPC Timeout");
			pdata->out_len=SOCKET_SIG_RELEASE;
			return(SEBS_PARSE_RETURN_GO_AHEAD);
			break;

		case SOCKET_SIG_NO_CONNECTION:
			if(hdata->xmlrpc_state==XMLRPC_STATE_SUBSCRIBER_RECONNECT)
			{

				DEBUG_PRINT_STR("XMLRPC reconnect to subscriber interface");
				pdata->out_len=SOCKET_SIG_CONNECT;
				break;
			}
			//DEBUG_PRINT_STR("XMLRPC No Connection");
			break;

        /* ***********/
		/*Only Client*/
		/* ***********/

		case SOCKET_SIG_COULD_NOT_CONNECT:
			DEBUG_PRINT_STR("XMLRPC could not connect");
			pdata->out_len=SOCKET_SIG_RELEASE;
			return(SEBS_PARSE_RETURN_GO_AHEAD);
			break;

		case SOCKET_SIG_COULD_NOT_RESOLVE_HOST:
			DEBUG_PRINT_STR("XMLRPC Resolve Host");
			pdata->out_len=SOCKET_SIG_RELEASE;
			return(SEBS_PARSE_RETURN_GO_AHEAD);
			break;

		case SOCKET_SIG_CONNECTED:
		{
			DEBUG_PRINT_STR("CONNECTED");

			if(hdata->xmlrpc_state==XMLRPC_STATE_SUBSCRIBER_RECONNECT)
			{

				DEBUG_PRINT_STR("RECONNECTED TO PUBLISHER ... ");
				pdata->handler_init=true;
				pdata->init_data=((iface_t *)pdata->init_data)->init_data;
				pdata->len=0;
				pdata->handler_function=&ros_handler;
				return (SEBS_PARSE_RETURN_GO_AHEAD);
			}

			hdata->xmlrpc_state = XMLRPC_STATE_HTTP;
			hdata->result_handling =XMLRPC_RESULT_REQUEST_SENT;


			msggen_message_type_t type;
			switch(hdata->client_type)
			{
			case XMLRPC_CLIENT_TYPE_UNREGISTER:
			case XMLRPC_CLIENT_TYPE_REGISTER:
				{
					hdata->genPayloadData[0]=((ros_iface_init_t*)iface->init_data)->iface_name; //Topic/Service
					hdata->genPayloadData[1]=((ros_iface_init_t*)iface->init_data)->type_name; //Message/ServiceType

					switch(((ros_iface_init_t*)iface->init_data)->ros_type)
					{
						case ROS_HANDLER_TYPE_TOPIC_PUBLISHER:
							type = (hdata->client_type == XMLRPC_CLIENT_TYPE_REGISTER) ?
									MSGGEN_TYPE_XMLRPC_REQ_REGISTER_PUBLISHER :
									MSGGEN_TYPE_XMLRPC_REQ_UNREGISTER_PUBLISHER;
							break;
						case ROS_HANDLER_TYPE_TOPIC_SUBSCRIBER:
							type = (hdata->client_type == XMLRPC_CLIENT_TYPE_REGISTER) ?
									MSGGEN_TYPE_XMLRPC_REQ_REGISTER_SUBSCRIBER :
									MSGGEN_TYPE_XMLRPC_REQ_UNREGISTER_SUBSCRIBER;
								break;
					}
				}
				break;
			case XMLRPC_CLIENT_TYPE_REQUEST_TOPIC:

				//TODO
				hdata->genPayloadData[0]=((ros_iface_init_t*)iface->init_data)->iface_name; //Topic/Service
				type =MSGGEN_TYPE_XMLRPC_REQUEST_TOPIC;
				break;

			}


			SEBS_PARSE_MSG_GEN(pdata, hdata->gen, pdata->additional_storage, rosc_static_socket_additional_data_size, type, 0, hdata->genPayloadData);
		}
			break;



		/* SIGNALS which should not occur here: */

		case SOCKET_SIG_DATA_SENT: /*should be caught by sending function*/
			DEBUG_PRINT_STR("XMLRPC ignoring unexpected signal  Data Sent");
			return (SEBS_PARSE_RETURN_GO_AHEAD);
			break;
		case SOCKET_SIG_RELEASE: /*outgoing*/
			DEBUG_PRINT_STR("XMLRPC ignoring unexpected signal  Release");
			return (SEBS_PARSE_RETURN_GO_AHEAD);
			break;
		case SOCKET_SIG_CONNECT: /*outgoing*/
			DEBUG_PRINT_STR("XMLRPC ignoring unexpected signal  Connect");
			return (SEBS_PARSE_RETURN_GO_AHEAD);
			break;

		default:
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
			}
			else
			{
				DEBUG_PRINT_STR("ERROR CONTENT LENGTH");
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

		case XMLRPC_RESULT_REQUEST_SENT:
			SEBS_PARSE_HTTP_INIT(pdata, hdata->http,
					SEBS_PARSE_HTTP_RESPONSE_INIT, xmlrpc_http_descriptors,
					XMLRPC_HTTP_DESCRIPTORS_LEN, xmlrpc_http_actions,
					XMLRPC_HTTP_ACTIONS_LEN, xmlrpc_http_methods,
					XMLRPC_HTTP_METHODS_LEN);
			break;

		case XMLRPC_RESULT_PUBLISHER_UPDATE_URL:
			DEBUG_PRINT_STR("URL:");
			DEBUG_PRINT_STR((char*)pdata->additional_storage);
			if(hdata->iface)
			{
				DEBUG_PRINT_STR("CHECKING FOR EXISTING CONNECTION");
				socket_t *socket=socket_list_start;
				socket_t *inactive_socket=0;

				while(socket)
				{
					if(socket->state!=SOCKET_STATE_INACTIVE)
					{
						if(socket->iface->handler_function==&xmlrpc)
						{
							xmlrpc_data_t *hdata = (xmlrpc_data_t*) socket->pdata.handler_data;

							if(hdata->client_type == XMLRPC_CLIENT_TYPE_REQUEST_TOPIC)
							{
								if(!strcmp(socket->connect_info.url,(char*)pdata->additional_storage))
								{
									DEBUG_PRINT_STR("FOUND XMLRPC REQUESTING TOPIC");
									break;
								}
							}
						}
						else if(socket->iface->handler_function==&ros_handler)
						{
							ros_handler_data_t *hdata = (ros_handler_data_t *) socket->pdata.handler_data;
							if(!strcmp(socket->connect_info.url,(char*)pdata->additional_storage))
							{
								DEBUG_PRINT_STR("FOUND TOPIC SUBSCRIBER");
								break;
							}
						}
					}
					else if(inactive_socket==0)
					{
						inactive_socket=socket;
					}
					socket=socket->next;
				}

				if(!socket)//No socket found
				{
					DEBUG_PRINT_STR("NEW PUBLISHER");
					if(inactive_socket)
					{
						DEBUG_PRINT_STR("Creating Subscriber");
						strcpy(inactive_socket->connect_info.url,(char*)pdata->additional_storage);
						inactive_socket->iface=hdata->iface;

						xmlrpc_data_t *n_hdata=inactive_socket->pdata.handler_data;
						n_hdata->client_type=XMLRPC_CLIENT_TYPE_REQUEST_TOPIC;

						inactive_socket->state=SOCKET_STATE_NOT_CONNECTED;
					    inactive_socket->pdata.init_data=hdata->iface;
					    inactive_socket->pdata.handler_init=true;
					    inactive_socket->pdata.handler_function=&xmlrpc;
					    inactive_socket->pdata.connection_interface=inactive_socket;
					}
					else
					{
						DEBUG_PRINT_STR("CAN NOT CREATE SUBSCRIBER - NO FREE SOCKET!");
					}
				}
				else
				{
					DEBUG_PRINT_STR("ALREADY CONNECTED");
				}
			}
			break;

		case XMLRPC_RESULT_PUBLISHER_UPDATE_TOPIC:
			DEBUG_PRINT_STR("TOPIC NAME:");
				DEBUG_PRINT_STR((char*)pdata->additional_storage);
				hdata->iface=interface_list_start;
				while(hdata->iface)
				{
					if(hdata->iface->handler_function == &ros_handler)
					{
						ros_iface_init_t* init=(ros_iface_init_t*) hdata->iface->init_data;
						if(!strcmp(init->iface_name,pdata->additional_storage))
						{
							DEBUG_PRINT_STR("INTERFACE FOUND");
							break;
						}

					}
					hdata->iface=hdata->iface->next;
				}
			break;

		case XMLRPC_RESULT_REQUEST_TOPIC_PORT:
			if(hdata->xml.numberparse.result == SEBS_PARSE_NUMBERPARSE_ANOTHER_CHAR
			   && hdata->xml.numberparse.last_byte == '<')
			{
				DEBUG_PRINT(INT,"Port: ",hdata->xml.numberparse.number);

				if(hdata->xml.numberparse.number<=0xFFFF)
				{
					socket_t * socket=(socket_t *)pdata->connection_interface;
					socket->connect_info.remote_port=hdata->xml.numberparse.number;
					hdata->xmlrpc_state=XMLRPC_STATE_SUBSCRIBER_RECONNECT;
					pdata->out_len=SOCKET_SIG_CLOSE;
				}
				else
				{
					pdata->out_len=SOCKET_SIG_RELEASE;
				}
			}
			break;

		default:
			break;
		}
		hdata->result_handling=XMLRPC_RESULT_NONE;
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
			if(hdata->http.seekstring.result != HTTP_VAL_TEXT_XML)
			{
				DEBUG_PRINT_STR("WRONG CONTENT");

				hdata->genPayloadData[0]=http_header_stdtext[HD_TXT_VAL_404_NOT_FOUND];
				SEBS_PARSE_MSG_GEN(pdata, hdata->gen, pdata->additional_storage, rosc_static_socket_additional_data_size, MSGGEN_TYPE_HTTP_ERROR, hdata->genPayloadData, hdata->genPayloadData);
			}

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
				SEBS_PARSE_NUMBERPARSE_INIT(pdata, hdata->http.numberparse, 3,
						false, 10)
;				break;

			case XMLRPC_DESCRIPTOR_CONTENT_TYPE:
				DEBUG_PRINT_STR("CONTENT_TYPE");
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
			hdata->xmlrpc_state = XMLRPC_STATE_CLOSE;



			switch(http_event)
			{
				case SEBS_PARSE_HTTP_EVENT_ERROR_CONTENT_LENGTH_TOO_LONG:

				break;

				case SEBS_PARSE_HTTP_EVENT_ERROR_CONTENT_LENGTH:

				break;

				case SEBS_PARSE_HTTP_EVENT_ERROR_ACTION_NOT_FOUND:
					hdata->genPayloadData[0]=http_header_stdtext[HD_TXT_VAL_404_NOT_FOUND];
				break;

				case SEBS_PARSE_HTTP_EVENT_ERROR_VERSION_NOT_SUPPORTED:

				break;

				case SEBS_PARSE_HTTP_EVENT_ERROR_BAD_REQUEST:

				break;

				case SEBS_PARSE_HTTP_EVENT_ERROR_METHOD_NOT_ALLOWED:
					hdata->genPayloadData[0]=http_header_stdtext[HD_TXT_VAL_501_METHOD_NOT_IMPLEMENTED];
				break;

				case SEBS_PARSE_HTTP_EVENT_ERROR_CONTENT_ENCODING:

				break;

				case SEBS_PARSE_HTTP_EVENT_ERROR_BAD_RESPONSE:

				break;

				default:

				break;
			}
			hdata->xmlrpc_state = XMLRPC_STATE_CLOSE;
			SEBS_PARSE_MSG_GEN(pdata, hdata->gen, pdata->additional_storage, rosc_static_socket_additional_data_size, MSGGEN_TYPE_HTTP_ERROR, hdata->genPayloadData, hdata->genPayloadData);

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
				if (hdata->xmlrpc_type == XMLRPC_TYPE_SERVER && hdata->xml.depth == 1)
				{
					hdata->tag_state = XMLRPC_TAG_STATE_METHODRC;
				}
				break;
				case XMLRPC_TAG_METHODRESPONSE:
				if (hdata->xmlrpc_type == XMLRPC_TYPE_CLIENT && hdata->xml.depth == 1)
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


					if(hdata->xml.tags[hdata->xml.depth] == XMLRPC_TAG_METHODCALL)
					{
						hdata->xmlrpc_state=XMLRPC_STATE_CLOSE;
						SEBS_PARSE_MSG_GEN(pdata, hdata->gen,pdata->additional_storage,rosc_static_socket_additional_data_size, MSGGEN_TYPE_XMLRPC_ACK,0,0);
					}
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

				if (hdata->xmlrpc_type == XMLRPC_TYPE_SERVER)
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

									DEBUG_PRINT_STR("TOPIC --- ");
									hdata->result_handling=XMLRPC_RESULT_PUBLISHER_UPDATE_TOPIC;
									//TODO size of callerid
									SEBS_PARSE_COPY2BUFFER_INIT(pdata,hdata->copy2buffer,pdata->additional_storage,__URI_MAX_LENGTH__,"<",0,true,0);
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
								hdata->result_handling=XMLRPC_RESULT_PUBLISHER_UPDATE_URL;
								DEBUG_PRINT_STR("Publisher Update A PUBLISHER");
								SEBS_PARSE_COPY2BUFFER_INIT(pdata,
										hdata->copy2buffer, pdata->additional_storage,
										__URI_MAX_LENGTH__, "<",0,1,0);

							}
							break;
							case XMLRPC_METHODNAME_REQUESTTOPIC:
							break;
							case XMLRPC_METHODNAME_SHUTDOWN:
							break;
						}
					}
				}
				else if (hdata->xmlrpc_type == XMLRPC_TYPE_CLIENT)
				{

					if(hdata->client_type==XMLRPC_CLIENT_TYPE_REQUEST_TOPIC)
						if(hdata->tag_state == XMLRPC_TAG_STATE_VALUE
								&& hdata->param_no == 1
								&& hdata->array_state == XMLRPC_ARRAY_STATE_VALUE)
						{
							switch(hdata->array_level)
							{
							case 0:
								if(hdata->array_value_number[0]==0
										&&(hdata->xml.tags[hdata->xml.depth] == XMLRPC_TAG_VALUE
										|| hdata->xml.tags[hdata->xml.depth] == XMLRPC_TAG_INT
										|| hdata->xml.tags[hdata->xml.depth] == XMLRPC_TAG_I4
										))
								{
									DEBUG_PRINT_STR("ACKNOWLEDGE?");
								}
								break;

							case 1:
								if(hdata->array_value_number[0]==2)
								{
									//Sometimes the port can be given on level zero by just an int value
									//and sometimes its given inside array with TCPROS and HOSTNAME m(

									if(
										((hdata->array_level == 1 && hdata->array_value_number[1]==2) || hdata->array_level==0)
										&&
										(
										   hdata->xml.tags[hdata->xml.depth] == XMLRPC_TAG_VALUE
										|| hdata->xml.tags[hdata->xml.depth] == XMLRPC_TAG_INT
										|| hdata->xml.tags[hdata->xml.depth] == XMLRPC_TAG_I4
										)
									)
								{
									hdata->result_handling=XMLRPC_RESULT_REQUEST_TOPIC_PORT;
									SEBS_PARSE_NUMBERPARSE_INIT(pdata, hdata->xml.numberparse, 5,
												false, 10);
								}
								break;
								}
							}
						}
				}
				break;
			}

			default:
			break;
		}
	}


	if(hdata->xmlrpc_state == XMLRPC_STATE_CLOSE)
	{
		DEBUG_PRINT_STR("XMLRPC CLOSE");
		pdata->len=0;
		pdata->out_len=SOCKET_SIG_RELEASE;
	}

	return (SEBS_PARSE_RETURN_GO_AHEAD);
}

#endif /* XMLRPC_C_ */
