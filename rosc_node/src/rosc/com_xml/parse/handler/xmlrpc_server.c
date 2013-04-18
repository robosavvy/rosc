#include <rosc/com_xml/parse/handler/xmlrpc_server.h>
#include <rosc/com_xml/parse/sub/subs.h>
#include <stdio.h>



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
		data->last_open_tag=XML_TAG_NONE;
		data->method=HTTP_METHOD_NOT_SET;
		data->rpcmethod=SLAVE_METHOD_NAME_NOT_SET;
		break;
	case PARSE_EVENT_NONE:
		DEBUG_PRINT(STR, "ERROR! this state should never be reached",  "PARSE_EVENT_NONE");
		break;
	case PARSE_EVENT_HANDLER_CALLED_SUBMODE_FINISHED:
		DEBUG_PRINT_STR("SUBMODE_FINISHED_FOR_HANDLER");
		switch (data->last_open_tag)
		{
			case XML_TAG_METHODNAME:
				//if(data->rpcmethod==SLAVE_METHOD_NAME_NOT_SET)
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

	case PARSE_EVENT_TAG_CONTENT:
		switch(data->last_open_tag)
		{
		case XML_TAG_METHODNAME:
			//PARSE_SUBMODE_INIT_SEEKSTRING
			DEBUG_PRINT_STR("::methodName");
			pact->submode_by_handler=true;
			PARSE_SUBMODE_INIT_SEEKSTRING(pact,rpc_xml_slave_methodnames,RPC_XML_SLAVE_METHODNAMES_LEN,"<");

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
			data->last_open_tag=pact->mode_data.xml.current_tag;
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
