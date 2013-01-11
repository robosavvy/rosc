/*
 *  Copyright by Synapticon GmbH (www.synapticon.com)  ©2013
 *
 *  eth.c
 *
 *  This file is part of InterfacePorts.
 *
 *  InterfacePorts is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  InterfacePorts is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY
{

}
 without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with InterfacePorts. If not, see <http://www.gnu.org/licenses/>.
 *
 *	File created by choll on 10.01.2013
 */


#include "eth.h"
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

uint16_t connectServer(ip_address_t target_ip, uint16_t remote_port, uint16_t local_port)
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
    return sockfd;
}

void closeConnection(uint16_t portID)
{
	close(portID);
}

uint32_t receiveFromPort(uint16_t portID, char* buffer, uint32_t buffersize)
{
	return read(portID, buffer, buffersize);
}

uint32_t sendToPort(uint16_t portID, char* buffer, uint32_t len)
{
	return write(portID, buffer,len);
}
