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

#include <rosc/com/msg_gen.h>
#include <rosc/system/eth.h>


ROSC_STATIC_MSG_BUILDUP__rosc_linux_test__simple1();
ROSC_STATIC_MSG_BUILDUP__rosc_linux_test__simple2();

ROSC_STATIC_MSG_USER_DEF__rosc_linux_test__simple1(sim1, 4);
ROSC_STATIC_MSG_USER_DEF__rosc_linux_test__simple2(sim2);

ROSC_STATIC_SYSTEM_MESSAGE_TYPE_LIST_BEGIN
	ROSC_SIZE_LIST_ENTRY__rosc_linux_test__simple1(sim1);
	ROSC_SIZE_LIST_ENTRY__rosc_linux_test__simple2(sim2);
	ROSC_SIZE_LIST_ENTRY_MIN_XMLRPC_OUTPUT_BUFFER(100);
ROSC_STATIC_SYSTEM_MESSAGE_TYPE_LIST_END

ROSC_STATIC_CALLBACK_HEAD__rosc_linux_test__simple1(sim1,simpleTopic1)
	printf("simple1 callback");
}

ROSC_STATIC_CALLBACK_HEAD__rosc_linux_test__simple1(sim1,simpleTopic2)
	printf("simple2 callback");
}

ROSC_STATIC_SUBSCRIBER_INIT__rosc_linux_test__simple1(sim1, simpleTopic1)
ROSC_STATIC_SUBSCRIBER_INIT__rosc_linux_test__simple2(sim2, simpleTopic2)

ROSC_STATIC_LOOKUP_TABLE_HEAD()
	ROSC_STATIC_LOOKUP_ENTRY(Computer0,IP(192,168,0,2))
ROSC_STATIC_LOOKUP_TABLE_END

NODE_NAME("master");
HOST_NAME("Host");

int main()
{

	printf("Socket Memory Statistics\n");
	printf("#############################################################\n");


	printf("Size of the ROS data struct in system part: %i bytes\n",(int) sizeof(ros_handler_data_t));
	printf("Size of the XMLRPC data struct in system part: %i bytes\n",(int) sizeof(xmlrpc_data_t));
	printf("------------------------------------------\n");
	printf("Overall size of the system part:  %i bytes\n",(int) offsetof(rosc_socket_memory_size_def_t,message_data));
	printf("Size of the user defined part:    %i bytes\n",(int) sizeof(rosc_socket_memory_size_def_t) - (int) offsetof(rosc_socket_memory_size_def_t,message_data));
	printf("------------------------------------------\n");
	printf("Overall size of a single socket:  %i bytes\n",(int) sizeof(rosc_socket_memory_size_def_t));
	printf("\n");
	printf("Overall size selected %i sockets: %i bytes\n",(int) __SOCKET_MAXIMUM__,(int) sizeof(rosc_static_socket_mem));

	printf("#############################################################\n");
	printf("\n");



	printf("Lookup Table Memory \n");
	printf("#############################################################\n");
	printf("Maximum Hostname Size: %i bytes\n", __HOSTNAME_MAX_LEN__);
	printf("Lookup Entry Size: %i bytes\n", sizeof(lookup_table_entry_t));
	printf("------------------------------------------\n");
	printf("Lookup Table Size: (%i entries) %i bytes\n", lookup_table_size ,lookup_table_size * sizeof(lookup_table_entry_t));

	printf("#############################################################\n");
	printf("\n");




	rosc_init();
	iface_list_insert(ROSC_STATIC_SUBSCRIBER__rosc_linux_test__simple1(sim1, simpleTopic1));
	iface_list_insert(ROSC_STATIC_SUBSCRIBER__rosc_linux_test__simple2(sim2, simpleTopic2));

	///TESTING STUFF

	char *narf="narf";
	uint16_t port=99;


	//XMLRPC_MSG_RESPONSE_REQUESTTOPIC(&port);

	char buffer[1000];

	//msg_gen_mode_t def->submode = MSG_GEN_MODE_TYPE;
	//uint8_t def->def_state = 0;
	//const msg_gen_type_t *def->type = def->header;
	//void** def->data = def->header_data;


	//def->out={0,0,0};
	//def->size ={ MSG_GEN_SIZE_MODE_NONE };
	//def->buf = { buffer_size, buffer };


	int size=100;

	msg_gen_command_t msg_def_xmlrpc_error;

	msg_def_xmlrpc_error.submode =MSG_GEN_MODE_TYPE;
	msg_def_xmlrpc_error.def_state = 0;
	msg_def_xmlrpc_error.header=xmlrpc_hd_response;
	msg_def_xmlrpc_error.payload=xmlrpc_msg_error;
	msg_def_xmlrpc_error.type = msg_def_xmlrpc_error.header;
	msg_def_xmlrpc_error.data = msg_def_xmlrpc_error.header_data;
	msg_def_xmlrpc_error.size.payload_size=0;
	msg_def_xmlrpc_error.size.payload_size_available=false;
	msg_def_xmlrpc_error.out.curPos=0;
	msg_def_xmlrpc_error.size.mode=MSG_GEN_SIZE_MODE_NONE;


	//

	int i;
	while(msg_gen(buffer, &size, &msg_def_xmlrpc_error))
	{

		for(i=0;i<size;i++)
		{
			printf("%c",buffer[i]);
		}
		size=100;
	}
	for(i=0;i<size;i++)
	{
		printf("%c",buffer[i]);
	}

	printf("\n----");



//
//
//
//	ip_address_t ip;
//	printf("\n---END---\n");
//	port_t p2=0;
//	int a=abstract_start_listening_on_port(&p2);
//
//
//	abstract_static_initHostname();
//	printf("Host: %s\n",host_name);
//	printf("Port: %i\n",p2);
//
//	printf("got ip: %i\n", abstract_resolveIP("ThinkTank.local",ip));
//	{
//		int i;
//		for(i=0;i<4;i++)
//		{
//			if(i)printf(".");
//			printf("%i",ip[i]);
//		}
//		printf("\n");
//	}
//
//
//	socket_id_t sock=abstract_connect_socket(ip,12345);
//
//	abstract_send_packet(sock,"narf",4);
//
//	abstract_close_socket(sock);


	rosc_spin();


}
