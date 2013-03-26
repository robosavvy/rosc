#include <rosc/com_xml/parse/handler/xmlrpc_server.h>

#include <stdio.h>



void xmlrpc_server_handler(parse_act_t * pact)
{
	xmlrpc_server_data_t* data=(xmlrpc_server_data_t*)pact->handler_data_storage;
	/*
	 * Check events
	 */
	printf("Submode Result: %i\n",pact->submode_result);
	switch(pact->event)
	{

	case PARSE_EVENT_NONE:
		printf("ERROR! this state should never be reached");
		break;
	case PARSE_EVENT_HTTP_METHOD_PARSED:
		data->method=pact->submode_result;
		if(pact->submode_result <0) //Do we have that method?
		{

			printf("ERROR Method not supported 501 Cannot process request!");
		}
		break;
	case PARSE_EVENT_HTTP_TARGET_PARSED:
		data->target=pact->submode_result;
		break;
		if(pact->submode_result <0) //Do we have that target?
		{
			printf("Page not found 404 !");
		}
		break;





	}

}
