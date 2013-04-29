#ifndef PARSER_H_
#define PARSER_H_

#include <rosc/com_xml/parse/parser_structure.h>
#include <rosc/com_xml/parse/handler/xmlrpc_server.h>
#include <rosc/com_xml/parse/handler/xmlrpc_client.h>


typedef enum
{
	XMLRPC_SERVER,
	XMLRPC_CLIENT
}xmlrpc_parser_type_t;

void xmlrpc_parse_act_init(parse_act_t *pact, xmlrpc_parser_type_t type, void * handler_data_storage);

void xmlrpc_parse(char *buf, uint32_t len, parse_act_t* pact);

#endif /* PARSER_H_ */
