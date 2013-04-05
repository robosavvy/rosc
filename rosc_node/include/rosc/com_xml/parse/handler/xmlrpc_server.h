#ifndef XMLRPC_SERVER_H_
#define XMLRPC_SERVER_H_

#include <rosc/com_xml/parse/parser_structure.h>
#include <rosc/com_xml/parse/handler/xmlrpc_string_id.h>


typedef struct xmlrpc_server_data_t
{
	xmlrpc_server_method_t method;
	xmlrpc_server_target_t target;
}xmlrpc_server_data_t;

void xmlrpc_server_handler(parse_act_t * pact);

#endif /* XMLRPC_SERVER_H_ */
