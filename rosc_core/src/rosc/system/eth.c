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

	//Memory for the port structs itself
	static socket_t __socket_struct_mem_reservation[__SOCKET_MAXIMUM__];
	static listen_socket_t __listen_socket_struct_mem_reservation[__LISTENING_SOCKET_MAXIMUM__];


	//external memory (defined by STATIC_SYSTEM_MESSAGE_TYPE_LIST in rosc_init.h)
	extern void* rosc_static_socket_mem[];
	extern const size_t rosc_static_socket_mem_size;
	extern const size_t rosc_static_socket_mem_message_offset;
	extern const size_t rosc_static_socket_mem_hdata_offset;


	//pointers first elements of sockets, listensockets, interfaces lists
	socket_t* socket_list_start;
	listen_socket_t* listen_socket_list_start;
	iface_t* interface_list_start;

	extern size_t rosc_static_lookup_table_size;
	extern lookup_table_entry_t *rosc_static_lookup_table;






void rosc_lists_init()
{
	int i;
	//Init listen socket list
	listen_socket_list_start=__listen_socket_struct_mem_reservation;
	for(i=0;i<__LISTENING_SOCKET_MAXIMUM__;++i)
	{
		__listen_socket_struct_mem_reservation[i].next=&(__listen_socket_struct_mem_reservation[i+1]);
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
		__socket_struct_mem_reservation[i].next=&(__socket_struct_mem_reservation[i+1]);
		__socket_struct_mem_reservation[i].iface=0;
		__socket_struct_mem_reservation[i].state=false;
		__socket_struct_mem_reservation[i].data=&rosc_static_socket_mem[i];
		__socket_struct_mem_reservation[i].pdata.handler_data=((char*)rosc_static_socket_mem)+i*rosc_static_socket_mem_size+rosc_static_socket_mem_hdata_offset;
		__socket_struct_mem_reservation[i].pdata.additional_storage=((char*)rosc_static_socket_mem)+i*rosc_static_socket_mem_size+rosc_static_socket_mem_message_offset;
	}
	__socket_struct_mem_reservation[i-1].next=0; //Set last items next address to zero


	interface_list_start=0;
}

bool iface_listen( iface_t *iface, port_t *port_number)
{

	listen_socket_t *cur=listen_socket_list_start;
	while(1)
	{
		if(cur->next==0)break;
		if(cur->state!=LISTEN_SOCKET_STATE_ACTIVE) break;
		cur=cur->next;
	}

	if(cur->state!=LISTEN_SOCKET_STATE_ACTIVE)
	{
		listen_socket_id_t sock;
		sock=abstract_start_listening_on_port(port_number);

		if(sock==-1)
		{
			ROSC_ERROR("Could not open socket!");
			return(false);
		}

		cur->id=sock;
		cur->interface=iface;
		DEBUG_PRINT(INT,"Port opened",*port_number)
		cur->port=*port_number;
		cur->state=LISTEN_SOCKET_STATE_ACTIVE;
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

bool iface_list_insert(iface_t *interface)
{
	iface_t* cur=interface_list_start;



	if(cur)//Does the list already contain elements?
	{
		//Go to the end of the list
		while(cur->next != 0 && cur!= interface) cur=cur->next;

		if(cur != interface)
		{
			cur->next=interface;
			interface->next=0;
			return(0);
		}
		else
		{
			//Element already in the list
			ROSC_ERROR("Tried to insert a element into the list, which is already there!");
			return(1);
		}
	}
	else
	{
		interface_list_start=interface;
		interface->next=0;
		return(0);
	}
}

bool register_interface(iface_t *interface)
{
	iface_t* cur=interface_list_start;
	while(cur->next != 0 && cur->next != interface) cur=cur->next;

	if(cur != interface)
	{
		cur->next=interface;
		interface->state=IFACE_STATE_DO_REGISTER;
		interface->next=0;
	}
	else
	{
		if(cur->state!=IFACE_STATE_UNREGISTERED)
		{
			return (true);
		}
		else
		{
			interface->state=IFACE_STATE_DO_REGISTER;
		}
	}
	return (false);
}

bool unregister_interface(iface_t *interface)
{
	iface_t* cur=interface_list_start;
	while(cur->next != 0 && cur->next != interface) cur=cur->next;

	//TODO I guess some additional stuff (states) must be done here ... but currently I leave it like that.
	if(cur == interface)
	{
		if(cur->state==IFACE_STATE_REGISTERED)
		{
			cur->state=IFACE_STATE_DO_UNREGISTER;
		}
	}

	return (true);
}

void iface_list_remove(iface_t *interface)
{
	//TODO check if list start is 0 (when changed)
	//TODO set list start to 0 when removing the last interface

	iface_t* cur=interface_list_start;
	iface_t* last;

	if(cur)
		ROSC_ERROR("Can not remove a interface from a empty list!");

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

bool rosc_hostlist_resolve(const char* hostname, size_t size, ip_address_ptr ip)
{
	bool nfound=true;
	int e;
	for (e = 0; e < rosc_static_lookup_table_size; ++e)
	{
		int c;
		for ( c = 0; c < size; ++c)
		{
			if((rosc_static_lookup_table+e)->hostname[c]!=hostname[c])
			{
				break;
			}
		}

		if(c==size
		   && (rosc_static_lookup_table+e)->hostname[c] == '\0')
		{
			nfound=false;
			break;
		}
	}

	if(nfound)
	{
		nfound=abstract_resolveIP(hostname,  size,  ip);
	}
	else
	{
		int p;

			for(p=0;p<4;p++)
			{
				ip[p]=(rosc_static_lookup_table+e)->ip[p];
			}

	}
	return (nfound);
}
