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
 *  rosc_spin.c created by Christian Holl
 */

#include <rosc/system/setup.h>
#include <rosc/system/eth.h>
#include <rosc/system/rosc_spin.h>
#include <rosc/system/rosc_init.h>
#include <rosc/com/xmlrpc.h>
#include <rosc/com/ros_handler.h>


void rosc_spin()
{
	int i;


	while(1)
	{
		//Check for interface tasks
		iface_t *iface=interface_list_start;
		socket_t* con_sock=socket_list_start;

		while(iface)
		{
			//Check for available interface
			while(con_sock)
			{
				if(con_sock->state==SOCKET_STATE_INACTIVE
				   && ( con_sock->reserved == 0 ||
						con_sock->reserved == iface_rpc_client))
				{
					break;
				}
				con_sock=con_sock->next;
			}

			if(con_sock)
			{

				switch(iface->state)
				{
				case IFACE_STATE_DO_REGISTER:
				case IFACE_STATE_DO_UNREGISTER:
					if(iface->handler_function==&ros_handler)
					{
						xmlrpc_init_data_t* init=iface->init_data;
						init->iface=(iface_t*)iface;
						init->type=XMLRPC_TYPE_CLIENT;

						con_sock->state=SOCKET_STATE_NOT_CONNECTED;
						con_sock->pdata.init_data=iface->init_data;
						con_sock->pdata.handler_init=true;
						con_sock->pdata.handler_function=&xmlrpc;

						//Init here, because the memory can change later ...
						//TODO probably unnecessary call when later the init of the rpc points only to current iface
						sebs_parser_frame(0,SOCKET_SIG_NO_DATA,&con_sock->pdata);
					}
					break;

					break;
				default:
					break;
				}
			}
			iface=iface->next;
		}

		//Check for new connections on the listen ports
			listen_socket_t *listen_sock=listen_socket_list_start;
			int i;

			while(listen_sock)
			{
				socket_t* con_sock=socket_list_start;
				while(con_sock)
				{
					if(con_sock->state==SOCKET_STATE_INACTIVE
					   && ( con_sock->reserved == 0 ||
							con_sock->reserved ==
							listen_sock->interface))
					{
						break;
					}
					con_sock=con_sock->next;
				}

				if(con_sock)
				{
					socket_id_t con_sock_id = abstract_socket_accept(listen_sock->id);
					if(con_sock_id >= 0)
					{
						con_sock->socket_id=con_sock_id;
						con_sock->state=SOCKET_STATE_CONNECTED;
						con_sock->iface=listen_sock->interface;
						con_sock->pdata.handler_init=true;
						con_sock->pdata.handler_function=listen_sock->interface->handler_function;
						con_sock->pdata.init_data=listen_sock->interface->init_data;


						DEBUG_PRINT_STR("New connection ... ");
					}
				}
				listen_sock=listen_sock->next;
			}




			//Receive incoming data
			char buffer[3];
			int size=3;
			int s;


			while(con_sock)
			{
				if(con_sock->state!=SOCKET_STATE_INACTIVE)
				{
					bool had_data=false;
					do
					{
						if(con_sock->state==SOCKET_STATE_CONNECTED)
							s=recv_packet(con_sock->socket_id,buffer,size);
						else
							s=SOCKET_SIG_NO_DATA;


						//if(s!=SOCKET_SIG_NO_DATA)
						{
							sebs_parser_frame(buffer,s, &con_sock->pdata);

							while(con_sock->pdata.out_len>0)
							{
								unsigned sig;
								switch(abstract_send_packet(con_sock->socket_id,con_sock->pdata.out_buf,con_sock->pdata.out_len))
								{
								case SEND_RESULT_OK:
									sig=SOCKET_SIG_DATA_SENT;
//									con_sock->pdata.out_len=0;
									break;

								}
								sebs_parser_frame(0,sig, &con_sock->pdata);
							}



							switch(con_sock->pdata.out_len)
							{
							case SOCKET_SIG_CONNECT:
								{
									socket_connect_info_t *connect_to=con_sock->pdata.additional_storage;
									switch(connect_to->data_state)
									{
									case CONNECT_DATA_STATE_IPV4:
										abstract_connect_socket(connect_to->remote_ip,connect_to->remote_port);
										break;
									}
								}
								break;

							case SOCKET_SIG_CLOSE:
								abstract_close_socket(con_sock->socket_id);
								con_sock->state=0;
								break;
							case SOCKET_SIG_NO_DATA:
								//Do nothing
								break;

							default:
								//Do nothing
								break;
							}
						}
					}
					while(s>0);
					if(had_data)
						printf("\n");
				}

				con_sock=con_sock->next;
			}


	}
}


