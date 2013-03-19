#ifndef XMLRPC_SERVER_H_
#define XMLRPC_SERVER_H_

#include <rosc/com_xml/parse/parser_structure.h>


typedef struct xmlrpc_server_data_t
{
	char test[12];
}xmlrpc_server_data_t;

void xmlrpc_server_handler(parse_act_t * pact);

#endif /* XMLRPC_SERVER_H_ */
