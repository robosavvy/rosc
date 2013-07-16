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
 *  ports.c created by Christian Holl
 */

#include <rosc/system/ports.h>
#include <rosc/system/status.h>


#ifndef __SYSTEM_HAS_MALLOC__
	#ifndef PORTS_STATIC_MAX_NUMBER
		#error No port setting macro defined, define PORTS_DYNAMIC or PORTS_STATIC_MAX_NUMBER <maximal ports>
	#endif

	//Memory for the port structs itself
	static port_t __port_mem_reservation[PORTS_STATIC_MAX_NUMBER];

	//point the hub pointer to the first array entry
	static port_t port_list_hub;

	//external memory (defined by STATIC_SYSTEM_MESSAGE_TYPE_LIST in rosc_init.h)
	extern void* rosc_static_port_mem;
	extern const size_t rosc_static_port_mem_size;
	extern const size_t rosc_static_port_mem_pdata_offset;
	extern const size_t rosc_static_port_mem_message_offset;
	extern const size_t rosc_static_port_mem_hdata_offset;

#else
	port_t __port_list_hub;
	const port_t* port_list_hub=__port_list_hub;


#endif


void rosc_ports_init()
{
	//Init list hub
	port_list_hub.data=0;
	port_list_hub.interface=0;
	port_list_hub.socket_id=0;
	port_list_hub.port_number=0;
	port_list_hub.type=PORT_TYPE_HUB;
	port_list_hub.state=PORT_STATE_UNUSABLE;
	port_list_hub.next=0;

	//Init for static systems on
	#ifndef __SYSTEM_HAS_MALLOC__
		port_list_hub.next=(port_t*)__port_mem_reservation;
		int i;
		for(i=0;i<PORTS_STATIC_MAX_NUMBER;++i)
		{
			__port_mem_reservation[i].next=(port_t *)__port_mem_reservation+sizeof(port_t)*(i+1);
			__port_mem_reservation[i].data=(void*)(rosc_static_port_mem+rosc_static_port_mem_size*i);
			__port_mem_reservation[i].interface=0;
			__port_mem_reservation[i].socket_id=0;
			__port_mem_reservation[i].port_number=0;
			__port_mem_reservation[i].type=PORT_TYPE_UNUSED;
			__port_mem_reservation[i].state=PORT_STATE_CLOSED;
		}
		__port_mem_reservation[i-1].next=0; //Set last items next address to zero
	#endif
}

bool rosc_open_port( iface_t *iface, uint16_t port_number)
{
	port_t *cur=port_list_hub.next;
	while(1)
	{
		if(cur->next==0)break;
		if(cur->state!=PORT_STATE_CLOSED ||
			cur->state!=PORT_STATE_UNUSABLE) break;
		cur=cur->next;
	}
	cur->port_number=port_number;

#ifndef __SYSTEM_HAS_MALLOC__
	if(cur->state==PORT_STATE_CLOSED)
	{
		sebs_parser_data_t *pdata=cur->data+rosc_static_port_mem_pdata_offset;
		pdata->handler_data=cur->data+rosc_static_port_mem_hdata_offset;
		void *message_data=cur->data+rosc_static_port_mem_message_offset;
		pdata->handler_init=true;

		switch(iface->type)
		{
			case IFACE_TYPE_XMLRPC_SERVER:
			case IFACE_TYPE_XMLRPC_CLIENT:
					pdata->handler_function=&xmlrpc;
					((xmlrpc_data_t *)&pdata->handler_data)->xmlrpc_type=iface->type;
				break;

			case IFACE_TYPE_ROSRPC_SERVER:
					pdata->handler_function=&ros_handler;
		//			((ros_handler_data_t *)&pdata->handler_data);
				break;

			case IFACE_TYPE_TOPIC_PUBLISHER:
					pdata->handler_function=&ros_handler;
				break;

			case IFACE_TYPE_SERVICE_SERVER:
					pdata->handler_function=&ros_handler;
				break;

			case IFACE_TYPE_TOPIC_SUBSCRIBER:
					pdata->handler_function=&ros_handler;
				break;

			case IFACE_TYPE_SERVICE_CLIENT:
					pdata->handler_function=&ros_handler;
				break;

			case IFACE_TYPE_LIST_HUB:
				ROSC_FATAL("FATAL ERROR, FOUND IFACE HUB NOT AT THE BEGINNING!");
				break;
		}
		pdata->handler_function(pdata); //initialize the handler;

		return (true);
	}
	else
	{
		return (false);
	}
#else
	//TODO allocate new port memory and stuff here

	//Needs additions for type sizes of ros message...
#endif
}
