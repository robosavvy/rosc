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
 *  eth.c created by Christian Holl
 */
#include <rosc/system/eth.h>
#include <rosc/system/status.h>


#ifndef __SYSTEM_HAS_MALLOC__


	//Memory for the port structs itself
	static socket_t __socket_struct_mem_reservation[__SOCKET_MAXIMUM__];
	static listen_socket_t __listen_socket_struct_mem_reservation[__LISTENING_SOCKET_MAXIMUM__];

	//point the hub pointer to the first array entry
	static socket_t* socket_list_start;
	static listen_socket_t* listen_socket_list_start;

	//external memory (defined by STATIC_SYSTEM_MESSAGE_TYPE_LIST in rosc_init.h)
	extern void* rosc_static_socket_mem;
	extern const size_t rosc_static_socket_mem_size;
	extern const size_t rosc_static_socket_mem_message_offset;
	extern const size_t rosc_static_socket_mem_hdata_offset;

#else
	socket_t __port_list_hub;
	const socket_t* socket_list_start=__port_list_hub;
#endif


void rosc_sockets_init()
{
	int i;
	//Init listen socket list
	listen_socket_list_start=__listen_socket_struct_mem_reservation;
	for(i=0;i<__LISTENING_SOCKET_MAXIMUM__;++i)
	{
		__listen_socket_struct_mem_reservation[i].next=(listen_socket_t *)__listen_socket_struct_mem_reservation+sizeof(listen_socket_t)*(i+1);
		__listen_socket_struct_mem_reservation[i].interface=0;
		__listen_socket_struct_mem_reservation[i].state=LISTEN_SOCKET_STATE_CLOSED;
		__listen_socket_struct_mem_reservation[i].port=-1;
		__listen_socket_struct_mem_reservation[i].id=-1;
	}
	__listen_socket_struct_mem_reservation[i-1].next=0; //Set last items next address to zero


	//Init socket list
	socket_list_start=__socket_struct_mem_reservation;
	for(i=0;i<__SOCKET_MAXIMUM__;++i)
	{
		__socket_struct_mem_reservation[i].next=(socket_t *)__socket_struct_mem_reservation+sizeof(socket_t)*(i+1);
		__socket_struct_mem_reservation[i].iface=0;
		__socket_struct_mem_reservation[i].is_active=false;
	}
	__socket_struct_mem_reservation[i-1].next=0; //Set last items next address to zero

}

bool rosc_iface_listen( iface_t *iface, port_t port_number)
{

	socket_t *cur=socket_list_start;
	while(1)
	{
		if(cur->next==0)break;
		if(!cur->is_active) break;
		cur=cur->next;
	}

	if(!cur->is_active)
	{
		listen_socket_id_t sock;
		sock=abstract_start_listening_on_port(&port_number);

		if(sock==-1)
		{
			return false;
		}

		//TODO create error
		cur->iface=iface;
		cur->pdata.handler_init=true;
		cur->pdata.init_data=iface->init_data;
		cur->pdata.handler_data=cur->data+rosc_static_socket_mem_hdata_offset;
		cur->pdata.additional_storage=cur->data+rosc_static_socket_mem_message_offset;
		cur->pdata.handler_function=iface->handler_function;
		cur->pdata.communication_port=cur;
	}
	else
	{
		return (false);
	}
	return (true);
}



void rosc_receive_by_socketid(uint32_t socket_id, uint8_t *buffer, uint32_t len)
{
	socket_t *cur=socket_list_start;
	while(1)
	{
		if(cur->next==0)break;
		if(cur->socket_id==socket_id) break;
		cur=cur->next;
	}
	sebs_parser_frame(buffer,len,&cur->pdata);
}


static iface_t interface_list_hub;


void rosc_init_interface_list()
{

	//TODO change
	interface_list_hub.isListHub=true;
	interface_list_hub.next=0;
}


void register_interface(iface_t *interface)
{
	//TODO check if list start is 0 (when changed)


	iface_t* cur=&interface_list_hub;
	//Go to the end of the list
	while(cur->next != 0 && cur->next != interface) cur=cur->next;

	if(cur->next != interface && cur->next == 0)
	{
		cur->next=interface;
		cur->next->next=0;
		cur->state=IFACE_STATE_DO_REGISTER;
	}
}


void unregister_interface(iface_t *interface)
{
	//TODO check if list start is 0 (when changed)
	//TODO set list start to 0 when removing the last interface
	iface_t* cur=&interface_list_hub;
	while(cur->next != 0 && cur->next != interface) cur=cur->next;

	//TODO I guess some additional stuff (states) must be done here ... but currently I leave it like that.
	if(cur == interface)
	{
		cur->state=IFACE_STATE_DO_UNREGISTER;
	}
}

void remove_interface(iface_t *interface)
{
	iface_t* cur=&interface_list_hub;
	iface_t* last;
	//Go to the entry of the list
	while(cur && cur != interface)
	{
		last=cur;
		cur=cur->next;
	}

	if(cur==interface)
	{
		last->next=cur->next;
	}
}


