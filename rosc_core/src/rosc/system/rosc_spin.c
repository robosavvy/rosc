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

void rosc_spin()
{
	int i;


	while(1)
	{

			//Check for new connections
			listen_socket_t *listen_sock=listen_socket_list_start;
			int i;

			while(listen_sock)
			{
				socket_t* con_sock=socket_list_start;
				while(con_sock)
				{
					if(!con_sock->is_active && ( con_sock->reserved == 0 ||  con_sock->reserved == listen_sock->interface))
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
						con_sock->is_active=true;
						con_sock->iface=listen_sock->interface;
						con_sock->pdata.handler_init=true;
						con_sock->pdata.handler_function=listen_sock->interface->handler_function;
						con_sock->pdata.init_data=listen_sock->interface->init_data;


						DEBUG_PRINT_STR("New connection ... ");
					}
				}
				listen_sock=listen_sock->next;
			}



			char buffer[3];
			int size=3;
			int s;

			socket_t* con_sock=socket_list_start;
			while(con_sock)
			{
				if(con_sock->is_active)
				{
					bool had_data=false;
					do
					{
						s=recv_packet(con_sock->socket_id,buffer,size);

						if(s>0)
						{
							had_data=true;
							printf("%.*s", s, buffer);


						}
						if(s!=SOCKET_SIG_NO_DATA)
						{
							sebs_parser_frame(buffer,s, &con_sock->pdata);

							while(con_sock->pdata.out_len>0)
							{
								switch(abstract_send_packet(con_sock->socket_id,con_sock->pdata.out_buf,con_sock->pdata.out_len))
								{
								case SEND_RESULT_OK:
									sebs_parser_frame(0,SOCKET_SIG_DATA_SENT, &con_sock->pdata);

								}
							}



							switch(con_sock->pdata.out_len)
							{
							case SOCKET_SIG_CLOSE:
								abstract_close_socket(con_sock->socket_id);
								con_sock->is_active=0;
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


