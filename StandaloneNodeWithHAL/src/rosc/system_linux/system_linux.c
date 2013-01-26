/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2013, Synapticon GmbH
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Synapticon GmbH nor the names of its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * system_linux.c
 *
 * Created on: 12.01.2013
 *     Author: Christian Holl
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


uint16_t listenPort(uint16_t port)
{
	int listenfd = 0;
	struct sockaddr_in serv_addr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd<0) return -1;

	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(listenfd, 10);

    return listenfd;
}

uint16_t connectServer(ip_address_t target_ip, uint16_t remote_port, uint16_t *local_port)
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

void closeConnection(port_id_t portID)
{
	close(portID);
}

int32_t receiveFromPort(port_id_t portID, char* buffer, uint32_t buffersize)
{
	return read(portID, buffer, buffersize);
}

int32_t sendToPort(port_id_t portID, char* buffer, uint32_t len)
{
	return write(portID, buffer,len);
}

port_id_t acceptConnectionOnPort(uint16_t portID)
{
	return accept(portID, (struct sockaddr*)NULL, NULL);
}


void auto_aquire_system_ip()
{

}

