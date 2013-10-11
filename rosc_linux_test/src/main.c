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

#include <rosc/rosc.h>

#include <rosc/msg/rosc_linux_test/simple1.h>
#include <rosc/msg/rosc_linux_test/simple2.h>

#include <rosc/com/ros_handler.h>

#include <rosc/com/msg_gen_common.h>
#include <rosc/system/eth.h>


ROSC_STATIC_MSG_BUILDUP__rosc_linux_test__simple1();
ROSC_STATIC_MSG_BUILDUP__rosc_linux_test__simple2();

ROSC_STATIC_MSG_USER_DEF__rosc_linux_test__simple1(sim1, 4);
ROSC_STATIC_MSG_USER_DEF__rosc_linux_test__simple2(sim2);

ROSC_STATIC_SYSTEM_MESSAGE_TYPE_LIST_BEGIN
	ROSC_SIZE_LIST_ENTRY__rosc_linux_test__simple1(sim1);
	ROSC_SIZE_LIST_ENTRY__rosc_linux_test__simple2(sim2);
ROSC_STATIC_SYSTEM_MESSAGE_TYPE_LIST_END


ROSC_STATIC_CALLBACK_HEAD__rosc_linux_test__simple1(sim1,simpleTopic1)
	printf("simple1 callback");
}

ROSC_STATIC_CALLBACK_HEAD__rosc_linux_test__simple1(sim1,simpleTopic2)
	printf("simple2 callback");
}

ROSC_STATIC_SUBSCRIBER_INIT__rosc_linux_test__simple1(sim1, simpleTopic1)
ROSC_STATIC_SUBSCRIBER_INIT__rosc_linux_test__simple2(sim2, simpleTopic2)

NODE_NAME("master");
HOST_NAME("Host");

int main()
{

	rosc_init();
	register_interface(ROSC_STATIC_SUBSCRIBER__rosc_linux_test__simple1(sim1, simpleTopic1));
	register_interface(ROSC_STATIC_SUBSCRIBER__rosc_linux_test__simple2(sim2, simpleTopic2));







	char *narf="narf";
	uint16_t port=99;


	XMLRPC_MSG_RESPONSE_REQUESTTOPIC(&port)

	char buffer[1000];

	//msg_gen_mode_t def->submode = MSG_GEN_MODE_TYPE;
	//uint8_t def->def_state = 0;
	//const msg_gen_type_t *def->type = def->header;
	//void** def->data = def->header_data;


	//def->out={0,0,0};
	//def->size ={ MSG_GEN_SIZE_MODE_NONE };
	//def->buf = { buffer_size, buffer };


	int size=100;

	msg_def_xmlrpc_response.submode =MSG_GEN_MODE_TYPE;
	msg_def_xmlrpc_response.def_state = 0;
	msg_def_xmlrpc_response.type = msg_def_xmlrpc_response.header;
	msg_def_xmlrpc_response.data = msg_def_xmlrpc_response.header_data;
	msg_def_xmlrpc_response.out.curPos=0;
	msg_def_xmlrpc_response.size.mode=MSG_GEN_SIZE_MODE_NONE;


	int i;
	while(send_rpc(buffer, &size, &msg_def_xmlrpc_response))
	{

		for(i=0;i<100-size;i++)
		{
			printf("%c",buffer[i]);
		}
		size=100;
	}
	for(i=0;i<100-size;i++)
	{
		printf("%c",buffer[i]);
	}




	ip_address_t ip;
	printf("\n---END---\n");
	port_t p2=0;
	int a=abstract_start_listening_on_port(&p2);


	abstract_static_initHostname();
	printf("Host: %s\n",host_name);
	printf("Port: %i\n",p2);

	printf("got ip: %i\n", abstract_resolveIP("ThinkTank.local",ip));
	{
		int i;
		for(i=0;i<4;i++)
		{
			if(i)printf(".");
			printf("%i",ip[i]);
		}
		printf("\n");
	}


	socket_id_t sock=abstract_connect_socket(ip,12345);

	abstract_send_packet(sock,"narf",4);

	abstract_close_socket(sock);


	while(1);
}
