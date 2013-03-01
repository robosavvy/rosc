// /////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// /////////////////////////

#include <rosc/rosc.h>
#include <rosc/system/eth.h>
#include <rosc/examplepack/exmsg.h>


char *test_message=
"POST / HTTP/1.1\n"
"Host: sncn-10:53556\n"
"Accept-Encoding: gzip\n"
"User-Agent: xmlrpclib.py/1.0.1 (by www.pythonware.com)\n"
"Content-Type: text/xml\n"
"Content-Length: 339\n"
"\n"
"<?xml version='1.0'?>\n"
"<methodCall>\n"
"<methodName>publisherUpdate</methodName>\n"
"<params>\n"
"<param>\n"
"<value><string>/master</string></value>\n"
"</param>\n"
"<param>\n"
"<value><string>/rosout</string></value>\n"
"</param>\n"
"<param>\n"
"<value><array><data>\n"
"<value><string>http://sncn-10:56998/</string></value>\n"
"</data></array></value>\n"
"</param>\n"
"</params>\n"
"</methodCall>";


typedef enum
{
	PARSE_ACT_STATE_FINSIHED=0,
	PARSE_ACT_STATE_HTTP_METHOD=PARSE_ACT_STATE_FINSIHED,
	PARSE_ACT_STATE_HEADER_FIELD_START,
	PRASE_ACT_STATE_HEADER_FIELD_DESCRIPTOR,
	PARSE_ACT_STATE_HEADER_FIELD,
	PARSE_ACT_STATE_XML_DEF,
	PARSE_ACT_STATE_TAG,
	PARSE_ACT_STATE_TAG_INNER_CONTENT,
	PARSE_ACT_STATE_IN_ATTRIBUTE,
	PARSE_ACT_STATE_IN_DATA,
}parse_act_state_t;

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

/**
 * Definition for handler function type
 */
typedef struct parse_act_t
{
	parse_act_state_t state;
	uint32_t depth;			 		     //!< This variable contains the current depth in the xml document
	uint32_t xml_length;     		     //!< If already parsed this field contains the length of the http header
	parse_data_field_state data_parse_state; //!< State of data field handler

	union
	{
		struct
		{
			char *string_storage[255];
		};
	};
	void (*handler_fct)(/*parse_act_t *ptr */);//!< Function pointer to handler function with one parameter from type parse_act_t*
	/**pointer to a data storage location of the handler function, this data is to be used by the handler for saving states*/
	void *handler_data_storage;
}parse_act_t;

/**
 * Definition for handler function type
 */
typedef void (*parse_handler_fct)(parse_act_t *pact);


//TODO do it like xmos does ... #define && function declaration for eclipse to force compiler to inline on xmos
//global macro for forcing inline functions??
inline void httpxml_parse_act_init(parse_act_t *pact, void *data_storage, parse_handler_fct handler_fct)
{
	pact->state=PARSE_ACT_STATE_HTTP_METHOD;
	pact->handler_data_storage=data_storage;
	pact->handler_fct=handler_fct;
}

/**
 * This is the XMLRPC parser for http-header and xml
 * @param buf
 * @param len
 * @param pact
 */
void xmlrpc_parse(char *buf, uint32_t len, parse_act_t* pact)
{
#if 1
	printf("Chunk Length: %i\n",len);
#endif


	switch(pact->state)
	{
	//#################################################
	//#############PARSING HTTP METHOD#################
	//#################################################
	case PARSE_ACT_STATE_HTTP_METHOD:


		break;

	//#################################################
	//##########SEEK/PARSE XML DEFINITION##############
	//#################################################
	case PARSE_ACT_STATE_XML_DEF:

		break;

	}

}


void xmlrpc_handler_test_fct(parse_act_t *pact)
{

}


#define buffersize 2
int main()
{
	int rlen;
	parse_act_t pact;

	httpxml_parse_act_init(&pact,0,0);

	for(rlen=0;test_message[rlen]!=0;rlen++);

	printf("Test Message Length: %i\n",rlen);

	int i;
	for(i=0;rlen-i*buffersize+buffersize>=0;i++)
	{
		printf("i: %i %i\n",i, i*buffersize);
		int len=buffersize;
		if(i*buffersize>rlen)
		{
			len=len-(i*buffersize-rlen);
		}
		xmlrpc_parse(test_message,len,&pact);
	}

	return 0;
}


