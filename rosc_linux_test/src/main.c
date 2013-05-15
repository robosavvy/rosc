/*
 *	Copyright (c) 2013, Synapticon GmbH
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions are met:
 *
 *	1. Redistributions of source code must retain the above copyright notice, this
 *	   list of conditions and the following disclaimer.
 *	2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *	ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *	The views and conclusions contained in the software and documentation are those
 *	of the authors and should not be interpreted as representing official policies,
 *	either expressed or implied, of the FreeBSD Project.
 *
 *  main.c created by Christian Holl
 */


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
//#include <rosc/examplepack/exmsg.h>
#include <rosc/com_xml/parse/parser.h>

#include <rosc/com_ifaces/iface.h>

char *test_request_message=
		"POST / HTTP/1.1\n"
		"Host: sncn-10:53556\n"
		"Accept-Encoding: gzip\n"
		"User-Agent: xmlrpclib.py/1.0.1 (by www.pythonware.com)\n"
		"Content-Type: text/xml\n"
//		"Content-Encoding: gzip\n"
		"Content-length: 339\n"
		"\n" //157
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


char *test_response_message=
		"HTTP/1.0 200 OK\n"
		"Server: BaseHTTP/0.3 Python/2.7.3\n"
		"Date: Thu, 25 Apr 2013 15:28:39 GMT\n"
		"Content-type: text/xml\n"
		"Content-length: 303\n"
		"\n"
		"<?xml version='1.0'?>\n"
		"<methodResponse>\n"
		"<params>\n"
		"<param>\n"
		"<value><array><data>\n"
		"<value><int>1</int></value>\n"
		"<value><string>Registered [/ListeningNode] as provider of [/ListeningNode/set_logger_level]</string></value>\n"
		"<value><int>1</int></value>\n"
		"</data></array></value>\n"
		"</param>\n"
		"</params>\n"
		"</methodResponse>";


/*
 *	Copyright (c) 2013, Synapticon GmbH
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions are met:
 *
 *	1. Redistributions of source code must retain the above copyright notice, this
 *	   list of conditions and the following disclaimer.
 *	2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *	ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *	The views and conclusions contained in the software and documentation are those
 *	of the authors and should not be interpreted as representing official policies,
 *	either expressed or implied, of the FreeBSD Project.
 *
 *  main.c created by Christian Holl
 */

int main_xmlrpctest()
{
	int rlen;
	int buffersize=1;
	parse_act_t pact;

#if(1)
	char *msg=test_request_message;
	xmlrpc_server_data_t server_data;
	xmlrpc_parse_act_init(&pact,XMLRPC_SERVER,&server_data);
#else
	char *msg=test_response_message;
	xmlrpc_client_data_t client_data;
	xmlrpc_parse_act_init(&pact,XMLRPC_CLIENT,&client_data);
#endif

	for(rlen=0;msg[rlen]!=0;rlen++);

	printf("Test Message Length: %i\n",rlen);

	int i;
	for(i=0;rlen-i*buffersize+buffersize>0;i++)
	{
		int len=buffersize;
		if(i*buffersize>rlen)
		{
			len=len-(i*buffersize-rlen);
		}
	//	printf("Current Chunk %i, Size %i: \n",i, len);
		xmlrpc_parse(msg+i*len,len,&pact);
	}

	return 0;
}




int main()
{
	printf("\n\nExecute --> rosc_linux_test\n\n");
	rosc_init();

	//main_xmlrpctest();

	return 0;
}


