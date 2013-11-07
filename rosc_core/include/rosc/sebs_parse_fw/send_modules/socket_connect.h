


#ifndef SOCKET_CONNECT_H_
#define SOCKET_CONNECT_H_

#include <rosc/system/eth.h>
#include <rosc/sebs_parse_fw/sebs_parser_frame.h>


#define SOCKET_CONNECT_INIT(PARSER_DATA, DATA_STORAGE, CONNECT_DATA)\
		PARSER_DATA->next_parser.parser_function=(sebs_parse_function_t) &socket_connect;\
		PARSER_DATA->next_parser.parser_data=(void *)(&DATA_STORAGE);\
		DATA_STORAGE.connect_data=CONNECT_DATA;\
		return (SEBS_PARSE_RETURN_INIT)


typedef enum
{
	SOCKET_CONNECT_STATE_URL_SCHEME,
	SOCKET_CONNECT_STATE_URL_IPV4_HOSTNAME,
	SOCKET_CONNECT_STATE_URI_HOSTNAME,
	SOCKET_CONNECT_STATE_URL_IPV6,
	SOCKET_CONNECT_STATE_URL_PORT,
	SOCKET_CONNECT_STATE_CONNECT,
	SOCKET_CONNECT_STATE_ERROR,
}socket_connect_state_t;

typedef struct
{
	socket_connect_state_t state;
	socket_connect_info_t* connect_data;
}socket_connect_data_t;



sebs_parse_return_t socket_connect(sebs_parser_data_t* pdata);

#endif /* SOCKET_CONNECT_H_ */
