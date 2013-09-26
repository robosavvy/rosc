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
 *  system_linux.c created by Christian Holl
 */

#include <rosc/system/eth.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <inttypes.h>



//
//bool start_listening_on_port(port_t port)
//{
//	//Do not bind to a specific port, or bind to port 0, e.g. sock.bind(('', 0)).
//	//The OS will then pick an available port for you. You can get the port
//	//that was chosen using sock.getsockname()[1], and pass it on to the slaves so
//	//that they can connect back.
//
//	int listenfd = 0;
//	struct sockaddr_in serv_addr;
//
//	listenfd = socket(AF_INET, SOCK_STREAM, 0);
//	if(listenfd<0) return -1;
//
//	memset(&serv_addr, '0', sizeof(serv_addr));
//	serv_addr.sin_family = AF_INET;
//	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//	serv_addr.sin_port = htons(port);
//    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
//    listen(listenfd, 10);
//	fcntl(listenfd, F_SETFL, O_NONBLOCK);
//    return listenfd-3;
//}

extern port_status_t stop_listening_on_port(port_t port);


extern void send_packet(socket_id_t socket_id, uint8_t*  buffer, uint32_t size);

extern socket_t* connect_socket(iface_t *iface, ip_address_t ip, port_t port);

extern void close_socket(socket_id_t socket);









port_id_t __connectServer(ip_address_t target_ip, uint16_t remote_port, uint16_t *local_port)
{
    int sockfd = 0;
    struct sockaddr_in serv_addr;


    serv_addr.sin_addr.s_addr= (int)(target_ip[3]<<24 | target_ip[2]<<16 | target_ip[1]<<8 | target_ip[0]<<0);


    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        return -1;
    }
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(remote_port);
    //inet_pton(AF_INET, "192.168.101.1", &serv_addr.sin_addr);

    serv_addr.sin_addr.s_addr= (int)(target_ip[3]<<24 | target_ip[2]<<16 | target_ip[1]<<8 | target_ip[0]<<0);
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return -1;
    }

    struct sockaddr_in sin;
    unsigned int addrlen = sizeof(sin);
    if(getsockname(sockfd, (struct sockaddr *)&sin, &addrlen) == 0 &&
       sin.sin_family == AF_INET &&
       addrlen == sizeof(sin))
    {
        *local_port = ntohs(sin.sin_port);
    }

    return sockfd;
}

void __closeConnection(port_id_t portID)
{
	close(portID+3);
}

int32_t __receiveFromPort(port_id_t portID, char* buffer, uint32_t buffersize)
{
	return read(portID+3, buffer, buffersize);
}

int32_t __sendToPort(port_id_t portID, char* buffer, uint32_t len)
{
	return write(portID+3, buffer,len);
}

port_id_t __acceptConnectionOnPort(uint16_t portID)
{
	int id=accept(portID, (struct sockaddr*)NULL, NULL);
		//printf("id %i\n",id);
	return id;
}



