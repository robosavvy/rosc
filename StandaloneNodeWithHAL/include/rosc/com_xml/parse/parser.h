#ifndef PARSER_H_
#define PARSER_H_

#include <rosc/com_xml/parse/parser_structure.h>

void xmlrpc_parse(char *buf, uint32_t len, parse_act_t* pact);

#endif /* PARSER_H_ */
