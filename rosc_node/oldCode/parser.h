

#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include <stdint.h>


/**
 * This specifies the commands which can be given from the handler function to the parser
 */
typedef enum
{
	PARSER_HANDLER_CMD_STORE_STRING,
	PARSER_HANDLER_CMD_GET_HOSTNAME_OR_IP,

}parser_handler_cmd;



typedef enum
{
	PARSER_CMD_SKIP_TAG,
}parse_cmd_t;


typedef enum
{
	PARSE_DATA_STATE_BEGIN=0, 	//!< parser currently not inside a data tag
	PARSE_DATA_STATE_EXCLAM,  	//!< parser found exclamation mark behind tag
	PARSE_DATA_STATE_D,		  	//!< parser found "D"
	PARSE_DATA_STATE_A1,		//!< parser found "A"
	PARSE_DATA_STATE_T,			//!< parser found "T"
	PARSE_DATA_STATE_A2,		//!< parser found "A" (second)
	PARSE_DATA_STATE_BRACKET1,	//!< parser found "["
	PARSE_DATA_STATE_BRACKET2,	//!< parser found "[" and is now inside data array
	PARSE_DATA_STATE_CBRACKET1, //!< parser found "]"
	PARSE_DATA_STATE_CBRACKET2, //!< parser found "]"
}parse_data_field_state;






//TODO do it like xmos does ... #define && function declaration for eclipse to force compiler to inline on xmos
//global macro for forcing inline functions??
inline void httpxml_parse_act_init(parse_act_t *pact, void *data_storage, parse_handler_fct handler_fct);


/**
 * This is the XMLRPC parser for http-header and xml
 * @param buf
 * @param len
 * @param pact
 */
void xmlrpc_parse(char *buf, uint32_t len, parse_act_t* pact);



#define buffersize 2

#endif /* PARSER_H_ */
