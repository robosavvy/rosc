


#ifndef SOCKET_CONNECT_H_
#define SOCKET_CONNECT_H_

#include <rosc/system/eth.h>
#include <rosc/sebs_parse_fw/sebs_parser_frame.h>


#define SOCKET_CONNECT_INIT(PARSER_DATA, DATA_STORAGE, CONNECT_DATA)\
		PARSER_DATA->next_parser.parser_function=(sebs_parse_function_t) &socket_connect;\
		PARSER_DATA->next_parser.parser_data=(void *)(&DATA_STORAGE);\
		DATA_STORAGE.connect_data=CONNECT_DATA;\
		return (SEBS_PARSE_RETURN_INIT)

sebs_parse_return_t socket_connect(sebs_parser_data_t* pdata);

#endif /* SOCKET_CONNECT_H_ */
