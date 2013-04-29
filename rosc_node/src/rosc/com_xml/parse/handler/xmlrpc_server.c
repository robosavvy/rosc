#include <rosc/com_xml/parse/handler/xmlrpc_server.h>
#include <rosc/com_xml/parse/sub/subs.h>

void xmlrpc_server_handler_n(parse_act_t * pact)
{
	xmlrpc_server_data_t* data=(xmlrpc_server_data_t*)pact->handler_data_storage;
	switch(pact->event)
	{

		case PARSE_EVENT_HANDLER_INIT:
			DEBUG_PRINT_STR("EVENT_HANDLER_INIT_SERVER");
			data->method=HTTP_METHOD_NOT_SET;
			data->rpcmethod=SLAVE_METHOD_NAME_NOT_SET;
			data->value_number=0;
			data->fetch_content=false;
			data->content_is_mandatory=false;
			data->content_cnt=0;
			pact->mode_data.http.content_type_text_xml_found=false;
		break;

		case PARSE_EVENT_HTTP_METHOD_PARSED:
				data->method=pact->submode_result;
				break;


		case PARSE_EVENT_HTTP_ACTION_PARSED:
			if(pact->submode_result <0) //Do we have that target?
			{
				DEBUG_PRINT(STR, "not found 404 !",  "PARSE_EVENT_METHOD");
			}
			else
			{
				data->target=pact->submode_result;
				switch(data->target)
				{
				case HTTP_ACTION_ROOT:
					if(data->method!=HTTP_METHOD_POST)
					{
						//TODO ERROR Forbidden
						PARSE_SUBMODE_INIT_SKIPWHOLEMESSAGE(pact);
					}
				case HTTP_ACTION_TEST:
					if(data->method==HTTP_METHOD_GET)
					{
						//TODO Create Test Answer
					}
					else
					{
						//TODO ERROR Method not allowed
						PARSE_SUBMODE_INIT_SKIPWHOLEMESSAGE(pact);
					}
				}
			}
			break;

		case PARSE_EVENT_HTTP_HEADER_FIELD_CONTENT:

			break;

		case PARSE_EVENT_XML_TAG:

			break;

		case PARSE_EVENT_XML_INSIDE_TAG:

			break;

		case PARSE_EVENT_XML_HANDLER_CALLED_SUBMODE_FINISHED:

			break;

		case PARSE_EVENT_XML_CONTENT_START:

			break;




		/*
		 * ERROR EVENT HANDLING
		 */

		case PARSE_EVENT_ERROR_HTTP_CONTENT_LENGTH_TOO_LONG:

			break;

		case PARSE_EVENT_ERROR_HTTP_CONTENT_LENGTH:

			break;

		case PARSE_EVENT_ERROR_HTTP_NOT_FOUND:

			break;

		case PARSE_EVENT_ERROR_HTTP_VERSION_NOT_SUPPORTED:

			break;

		case PARSE_EVENT_ERROR_HTTP_BAD_REQUEST:

			break;

		case PARSE_EVENT_ERROR_HTTP_LENGTH_REQUIRED:

			break;

		case PARSE_EVENT_ERROR_HTTP_METHOD_NOT_ALLOWED:

			break;

		case PARSE_EVENT_ERROR_HTTP_CONTENT_TYPE:

			break;

		case PARSE_EVENT_ERROR_HTTP_CONTENT_ENCODING:

			break;

		case PARSE_EVENT_ERROR_XML_DEPTH:

			break;

		case PARSE_EVENT_ERROR_XML_MALFORMED:

			break;



		case PARSE_EVENT_ERROR_HTTP_BAD_RESPONSE:
		case PARSE_EVENT_HTTP_RESPONSE_CODE:
		default:
			break;
	}
}


void xmlrpc_server_handler(parse_act_t * pact)
{
	xmlrpc_server_data_t* data=(xmlrpc_server_data_t*)pact->handler_data_storage;

	switch(pact->event)
	{

	case PARSE_EVENT_HANDLER_INIT:
		DEBUG_PRINT_STR("EVENT_HANDLER_INIT_SERVER");
		data->method=HTTP_METHOD_NOT_SET;
		data->rpcmethod=SLAVE_METHOD_NAME_NOT_SET;
		data->value_number=0;
		data->fetch_content=false;
		data->content_is_mandatory=false;
		data->content_cnt=0;
		pact->mode_data.http.content_type_text_xml_found=false;
		break;

	case PARSE_EVENT_NONE:
		DEBUG_PRINT(STR, "ERROR! this state should never be reached",  "PARSE_EVENT_NONE");
		break;

	case PARSE_EVENT_XML_HANDLER_CALLED_SUBMODE_FINISHED:
		DEBUG_PRINT_STR("SUBMODE_FINISHED_FOR_HANDLER");
		switch (pact->mode_data.xml.tags[pact->mode_data.xml.depth])
		{
			case XML_TAG_METHODNAME:
				if(pact->submode_result>0)
				{
					data->rpcmethod=pact->submode_result;
					DEBUG_PRINT(STR, "MethodName", rpc_xml_slave_methodnames[data->rpcmethod]);
				}
				else
				{
					DEBUG_PRINT_STR("MethodName not found!");
					PARSE_SUBMODE_INIT_SKIPWHOLEMESSAGE(pact);
				}
				break;
			default:
				break;
		}
		break;
	case PARSE_EVENT_HTTP_METHOD_PARSED:
		if(pact->submode_result <0) //Do we have that method?
		{
			DEBUG_PRINT(STR, "ERROR Method not supported 501 Cannot process request!",  "PARSE_EVENT_METHOD");
			PARSE_SUBMODE_INIT_SKIPWHOLEMESSAGE(pact);
		}
		else
		{
			data->method=pact->submode_result;
		}
		break;

	case PARSE_EVENT_HTTP_ACTION_PARSED:
		data->target=pact->submode_result;
		if(pact->submode_result <0) //Do we have that target?
		{
			DEBUG_PRINT(STR, "not found 404 !",  "PARSE_EVENT_METHOD");
		}
		break;

	case PARSE_EVENT_XML_INSIDE_TAG:
		switch(pact->mode_data.xml.tags[pact->mode_data.xml.depth])
		{
		case XML_TAG_METHODNAME:
			if(pact->mode_data.xml.tags[1]==XML_TAG_METHODCALL &&
			   pact->mode_data.xml.depth==2)
			{
				data->fetch_content=true;
				data->content_is_mandatory=true;
			}
		break;

		case XML_TAG_VALUE:
			//Number the current value tag (param)
			if(pact->mode_data.xml.tags[1]==XML_TAG_METHODCALL &&
			   pact->mode_data.xml.tags[2]==XML_TAG_PARAMS &&
			   pact->mode_data.xml.tags[3]==XML_TAG_PARAM &&
			   pact->mode_data.xml.depth == 4)
			{
				DEBUG_PRINT(INT,"VALUE", data->value_number);
				++data->value_number;
				data->current_value_tag=data->value_number;
			}
			break;


		case XML_TAG_STRING:
			switch(data->current_value_tag)
			{
			//Numbers start with one! 0 stands for NO value tag
			case 1:
			case 2:
			case 3:
				data->fetch_content=true;
				break;
			default:
				break;
			}
				break;

		default:
			break;
		}

		break;



	case PARSE_EVENT_XML_CONTENT_START:
		DEBUG_PRINT_STR("CONTENT!!!");

		//Check if we have multiple content inside that tag (which is not allowed)
		if(data->content_cnt != 0)
		{
			DEBUG_PRINT_STR("ERROR: Only single strings are allowed inside tags!!!");
			//TODO ERROR!
			PARSE_SUBMODE_INIT_SKIPWHOLEMESSAGE(pact);
			break;
		}

		//Method Name
		if(pact->mode_data.xml.tags[pact->mode_data.xml.depth]==XML_TAG_METHODNAME)
		{

			if(!(pact->mode_data.xml.tags[1]==XML_TAG_METHODCALL
				&& pact->mode_data.xml.depth == 2)) break;

			if(data->rpcmethod!=SLAVE_METHOD_NAME_NOT_SET) //Check if it was not set before
			{
				DEBUG_PRINT_STR("XML ERROR: MethodName already given before!");
				PARSE_SUBMODE_INIT_SKIPWHOLEMESSAGE(pact);
				break;
			}
			pact->submode_by_handler=true;
			PARSE_SUBMODE_INIT_SEEKSTRING(pact,rpc_xml_slave_methodnames,RPC_XML_SLAVE_METHODNAMES_LEN," <>");
		}


		//Getting Values
		if(data->fetch_content)
		{
			DEBUG_PRINT_STR("FETCH!");
			if(data->value_number!=0 && pact->mode_data.xml.tags[pact->mode_data.xml.depth]==XML_TAG_VALUE)
			{
				DEBUG_PRINT_STR("Interesting Value Content!");
			}

		}

		data->content_cnt++;
		break;



	case PARSE_EVENT_XML_TAG:
		#ifdef __DEBUG__PRINTS__
		{
			unsigned int i=pact->mode_data.xml.depth;

			if(pact->mode_data.xml.tag_type==XML_TAG_TYPE_CLOSE)
				--i;
				for(;i>0;--i)
				{
					printf("   ");
				}
		}

		if(pact->mode_data.xml.tag_type==XML_TAG_TYPE_CLOSE)
		{
			printf("/");
		}

		if(pact->submode_result>=0)
		{
			DEBUG_PRINT_STR(pact->submode_data.seekString.stringlist[pact->submode_result]);
		}
		else
		{
			DEBUG_PRINT_STR("UNKNOWN_TAG");
		}

		#endif


		if(data->fetch_content && pact->mode_data.xml.tag_type != XML_TAG_TYPE_CLOSE)
		{
			DEBUG_PRINT_STR("Unexpected Tag Opening!");
			PARSE_SUBMODE_INIT_SKIPWHOLEMESSAGE(pact);
		}

		switch(pact->mode_data.xml.tag_type)
		{
		case XML_TAG_TYPE_CLOSE:

			//reset the value for being inside a params/param/value tag back to zero
			//when leaving the current tag
			if(pact->mode_data.xml.tags[1]==XML_TAG_METHODCALL &&
			   pact->mode_data.xml.tags[2]==XML_TAG_PARAMS &&
			   pact->mode_data.xml.tags[3]==XML_TAG_PARAM &&
			   pact->mode_data.xml.depth == 4)
			{
				DEBUG_PRINT_STR("VALUE END!");
				data->current_value_tag=0;
			}
			if(data->content_is_mandatory && !data->content_cnt)
			{
				DEBUG_PRINT_STR("Required content not found!");
			}
			//set fetch content to false when leaving the current tag
			data->fetch_content=false;
			data->content_is_mandatory=false;
			//set the content counter back to zero when leaving the tag
			data->content_cnt=0;
			break;
		case XML_TAG_TYPE_NORMAL:

			break;
		case XML_TAG_TYPE_CDATA:
			DEBUG_PRINT_STR("<CDATA>");
			break;
		case XML_TAG_TYPE_EXCLAMATION_MARK:
			DEBUG_PRINT_STR("<!>");
			break;
		case XML_TAG_TYPE_QUESTION_MARK:
			DEBUG_PRINT(STR,"<?>",  pact->submode_data.seekString.stringlist[pact->submode_result]);
			break;
		}
		break;
	}

}
