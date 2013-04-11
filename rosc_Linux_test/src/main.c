


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
#include <rosc/com_xml/parse/parser.h>

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



int main()
{
	int rlen;
	int buffersize=1;
	parse_act_t pact;

	xmlrpc_server_data_t server_data;
	xmlrpc_parse_act_init(&pact,XMLRPC_SERVER,&server_data);

	for(rlen=0;test_message[rlen]!=0;rlen++);

	//printf("Test Message Length: %i\n",rlen);

	int i;
	for(i=0;rlen-i*buffersize+buffersize>0;i++)
	{
		int len=buffersize;
		if(i*buffersize>rlen)
		{
			len=len-(i*buffersize-rlen);
		}
	//	printf("Current Chunk %i, Size %i: \n",i, len);
		xmlrpc_parse(test_message+i*len,len,&pact);
	}

	return 0;
}


