#include <rosc/com_xml/parse/handler/xmlrpc_server.h>
#include <stdio.h>
void xmlrpc_server_handler(parse_act_t * pact)
{
	printf("Server Handler Called -> Event: %i \n",pact->event);
}
