#include <rosc/com_xml/parse/handler/xmlrpc_client.h>


#include <stdio.h>
void xmlrpc_client_handler(parse_act_t * pact)
{
	if(pact->event<0)
	{
		DEBUG_PRINT_STR("ERROR EVENT!");
	}
}
