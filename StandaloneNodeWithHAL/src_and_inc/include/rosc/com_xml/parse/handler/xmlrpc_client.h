#ifndef XMLRPC_CLIENT_H_
#define XMLRPC_CLIENT_H_

#include <rosc/com_xml/parse/parser_structure.h>


typedef struct xmlrpc_client_data_t
{
	char test[12];
}xmlrpc_server_data_t;

void xmlrpc_client_handler(parse_act_t * pact);


#endif /* XMLRPC_CLIENT_H_ */
