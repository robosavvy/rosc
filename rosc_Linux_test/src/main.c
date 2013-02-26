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
	PARSE_ACT_STATE_FINSIHED,
	PARSE_ACT_STATE_BEGIN=PARSE_ACT_STATE_FINSIHED,
	PRASE_ACT_STATE_HEADER,
	PARSE_ACT_STATE_XML_DEG,
	PARSE_ACT_STATE_TAG,
	PARSE_ACT_STATE_TAG_INNER_CONTENT,
	PARSE_ACT_STATE_IN_ATTRIBUTE,
	PARSE_ACT_STATE_IN_DATA,
}parse_act_state_t;

typedef struct parse_act_t
{
	const char** tag_array;
	parse_act_state_t state;
	uint32_t depth;
}parse_act_t;


void httpxml_parse_act(parse_act_t *pact)
{
	pact->state=PARSE_ACT_STATE_BEGIN;

}


void xmlrpc_parse(char *buf, uint32_t len, parse_act_t* pact)
{
#if 1
	printf("Chunk Length: %i\n",len);
#endif
}


void xmlrpc_handler_test_fct()
{

}


#define buffersize 200
int main()
{
	int rlen;
	parse_act_t pact;
	httpxml_parse_act(&pact);

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


