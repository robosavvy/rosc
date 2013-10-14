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
			listen_socket_t *lsock=listen_socket_list_start;
			int i;

			while(lsock)
			{
				socket_t* sock=socket_list_start;
				while(sock)
				{
					if(!sock->is_active && ( sock->reserved == 0 ||  sock->reserved == lsock->interface))
					{
						break;
					}
					sock=sock->next;
				}

				if(sock)
				{
					socket_id_t asock = abstract_socket_accept(lsock->id);
					if(asock >= 0)
					{
						sock->socket_id=asock;
						sock->is_active=true;
						DEBUG_PRINT_STR("New connection ... ");
					}
				}
				lsock=lsock->next;
			}



			char buffer[3];
			int size=3;
			int s;

			socket_t* sock=socket_list_start;
			while(sock)
			{
				if(sock->is_active)
				{
					bool had_data=false;
					do
					{
						s=recv_packet(sock->socket_id,buffer,size);

						if(s>0)
						{
							had_data=true;
							printf("%.*s", s, buffer);
						}
					}
					while(s>0);
					if(had_data)
						printf("\n");
				}

				sock=sock->next;
			}


	}
}


