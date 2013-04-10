#include <rosc/com_xml/parse/handler/xmlrpc_server.h>

#include <stdio.h>



void xmlrpc_server_handler(parse_act_t * pact)
{
	xmlrpc_server_data_t* data=(xmlrpc_server_data_t*)pact->handler_data_storage;
	/*
	 * Check events
	 */
	DEBUG_PRINT(INT, "Submode Result",  pact->submode_result);


	if(pact->event<0)
	{
		printf("ERROR !\n");
		exit(1);
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





	}

}
