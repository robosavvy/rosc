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

/* ******************/
/*     TESTING      */
/* ******************/
#include<rosc/sebs_parse_fw/send_modules/socket_connect.h>
#include<stdio.h>
#include<string.h>

/* ******************/


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
	ROSC_STATIC_LOOKUP_ENTRY(Computer1,IP(192,168,0,3))
ROSC_STATIC_LOOKUP_TABLE_END


MASTER_URI_STATIC("http://localhost:11311");

NODE_NAME("roscnode");

int main()
{
	printf("Socket Memory Statistics\n");
	printf("#############################################################\n");


	printf("Size of the ROS data struct in system part: %i bytes\n",(int) sizeof(ros_handler_data_t));
	printf("Size of the XMLRPC data struct in system part: %i bytes\n",(int) sizeof(xmlrpc_data_t));
	printf("------------------------------------------\n");
	printf("Overall size of the system part: %i bytes\n",(int) offsetof(rosc_socket_memory_size_def_t,message_data));
	printf("Size of the connect url length: %i bytes\n",  (int) rosc_static_url_max_size);
	printf("Size of the user defined part: %i bytes\n",(int) sizeof(rosc_socket_memory_size_def_t) - (int) offsetof(rosc_socket_memory_size_def_t,message_data));
	printf("------------------------------------------\n");
	printf("Overall size of a single socket: %i bytes\n",(int) sizeof(rosc_socket_memory_size_def_t));
	printf("\n");
	printf("Overall size selected %i sockets: %i bytes\n",(int) __SOCKET_MAXIMUM__,(int) sizeof(rosc_static_socket_mem));

	printf("#############################################################\n");
	printf("\n");

	printf("Lookup Table Memory \n");
	printf("#############################################################\n");
	printf("Maximum Hostname Size: %i bytes\n",(int) __HOSTNAME_BUFFER_LEN__);
	printf("Lookup Entry Size: %i bytes\n",(int) sizeof(lookup_table_entry_t));
	printf("------------------------------------------\n");
	printf("Lookup Table Size: (%i entries) %i bytes\n", (int)lookup_table_size , (int)lookup_table_size * (int)sizeof(lookup_table_entry_t));
	printf("#############################################################\n");
	printf("\n");


	rosc_init();
	iface_list_insert(ROSC_STATIC_SUBSCRIBER__rosc_linux_test__simple1(sim1, simpleTopic1));
	iface_list_insert(ROSC_STATIC_SUBSCRIBER__rosc_linux_test__simple2(sim2, simpleTopic2));


	//TODO register function
	(ROSC_STATIC_SUBSCRIBER__rosc_linux_test__simple1(sim1, simpleTopic1))->state=IFACE_STATE_DO_REGISTER;
	(ROSC_STATIC_SUBSCRIBER__rosc_linux_test__simple2(sim2, simpleTopic2))->state=IFACE_STATE_DO_REGISTER;

	rosc_spin();
}
