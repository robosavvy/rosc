#include <rosc/com_xml/parse/handler/xmlrpc_server.h>

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

	case PARSE_EVENT_NONE:
		DEBUG_PRINT(STR, "ERROR! this state should never be reached",  "PARSE_EVENT_NONE");
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
	case PARSE_EVENT_TAG:
		#ifdef __DEBUG__PRINTS__
		{
			unsigned int i=pact->mode_data.xml.depth;

				for(;i>0;--i)
				{
					printf("   ");
				}
		}

		if(pact->event==PARSE_EVENT_TAG_CONTENT)
		{
			printf("->\n");
			break;
		}

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
