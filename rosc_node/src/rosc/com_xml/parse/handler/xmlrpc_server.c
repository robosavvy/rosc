#include <rosc/com_xml/parse/handler/xmlrpc_server.h>
#include <rosc/com_xml/parse/sub/subs.h>


void xmlrpc_server_handler(parse_act_t * pact)
{
	xmlrpc_server_data_t* data=(xmlrpc_server_data_t*)pact->handler_data_storage;
	/*
	 * Check events
	 */



	if(pact->event<0)
	{
		DEBUG_PRINT_STR("ERROR !\n");
		//exit(1);
	}

	switch(pact->event)
	{

	case PARSE_EVENT_HANDLER_INIT:
		DEBUG_PRINT_STR("EVENT_HANDLER_INIT_SERVER");
		data->method=HTTP_METHOD_NOT_SET;
		data->rpcmethod=SLAVE_METHOD_NAME_NOT_SET;
		data->value_number=0;

		break;
	case PARSE_EVENT_NONE:
		DEBUG_PRINT(STR, "ERROR! this state should never be reached",  "PARSE_EVENT_NONE");
		break;

	case PARSE_EVENT_CONTENT_START:
		DEBUG_PRINT_STR("CONTENT!");
		break;
	case PARSE_EVENT_HANDLER_CALLED_SUBMODE_FINISHED:
		DEBUG_PRINT_STR("SUBMODE_FINISHED_FOR_HANDLER");
		switch (pact->mode_data.xml.tags[pact->mode_data.xml.depth])
		{
			case XML_TAG_METHODNAME:
				//if(data->rpcmethod == SLAVE_METHOD_NAME_NOT_SET)
				{
					data->rpcmethod=pact->submode_result;
					DEBUG_PRINT(STR, "MethodName", rpc_xml_slave_methodnames[data->rpcmethod]);
				}
			//	else
				{
					//ERROR!
				}
				break;
			default:
				break;
		}
		break;
	case PARSE_EVENT_HTTP_METHOD_PARSED:
		data->method=pact->submode_result;
		if(pact->submode_result <0) //Do we have that method?
		{
			DEBUG_PRINT(STR, "ERROR Method not supported 501 Cannot process request!",  "PARSE_EVENT_METHOD");
		}
		break;
	case PARSE_EVENT_HTTP_TARGET_PARSED:
		data->target=pact->submode_result;
		break;
		if(pact->submode_result <0) //Do we have that target?
		{
			DEBUG_PRINT(STR, "Page not found 404 !",  "PARSE_EVENT_METHOD");
		}
		break;

	case PARSE_EVENT_TAG_INSIDE:
		switch(pact->mode_data.xml.tags[pact->mode_data.xml.depth])
		{
		case XML_TAG_METHODNAME:
			//Check if we are at the right place otherwise break;
			if(!(pact->mode_data.xml.tags[1]==XML_TAG_METHODCALL
				&& pact->mode_data.xml.depth == 2)) break;

			if(data->rpcmethod!=SLAVE_METHOD_NAME_NOT_SET)
			{
				DEBUG_PRINT_STR("XML ERROR: MethodName already given before!");
				break;
			}
			DEBUG_PRINT_STR("::methodName");
			pact->submode_by_handler=true;
			PARSE_SUBMODE_INIT_SEEKSTRING(pact,rpc_xml_slave_methodnames,RPC_XML_SLAVE_METHODNAMES_LEN,"<");
			break;
		case XML_TAG_VALUE:
			//Check if we are at the right place otherwise break;
				if(!(pact->mode_data.xml.tags[1]==XML_TAG_METHODCALL &&
					 pact->mode_data.xml.tags[2]==XML_TAG_PARAMS &&
					 pact->mode_data.xml.tags[3]==XML_TAG_PARAM &&
					 pact->mode_data.xml.depth == 4)) break;
				DEBUG_PRINT(INT,"VALUE", data->value_number);
				switch(data->value_number)
				{
				case 0:
					//Caller-ID

					break;
				case 1:

					break;
				case 2:

					break;
				}
				data->value_number++;
			break;
		default:
			break;
		}

		break;

	case PARSE_EVENT_TAG:
		#ifdef __DEBUG__PRINTS__
		{
			unsigned int i=pact->mode_data.xml.depth;

				for(;i>0;--i)
				{
					printf("   ");
				}
		}

//		if(pact->event==PARSE_EVENT_TAG_CONTENT)
//		{
//			printf("->\n");
//			break;
//		}

		if(pact->mode_data.xml.tag_type==XML_TAG_TYPE_CLOSE)
		{
			printf("/");
		}



		#endif

		switch(pact->mode_data.xml.tag_type)
		{
		case XML_TAG_TYPE_CLOSE:
			DEBUG_PRINT_STR(pact->submode_data.seekString.stringlist[pact->submode_result]);

			break;
		case XML_TAG_TYPE_NORMAL:
			DEBUG_PRINT_STR(pact->submode_data.seekString.stringlist[pact->submode_result]);

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
