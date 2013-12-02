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
#include <rosc/system/spec.h>
#include <rosc/system/spec.h>
#include <netinet/tcp.h>
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

listen_socket_id_t abstract_start_listening_on_port(port_t* port)
{
	//Do not bind to a specific port, or bind to port 0, e.g. sock.bind(('', 0)).
	//The OS will then pick an available port for you. You can get the port
	//that was chosen using sock.getsockname()[1], and pass it on to the slaves so
	//that they can connect back.
	int listenfd = 0;
	struct sockaddr_in serv_addr;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if(listenfd<0) return (-1);

	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(*port);
    bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    listen(listenfd, 10);
	fcntl(listenfd, F_SETFL, O_NONBLOCK);

	int size=sizeof(serv_addr);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (getsockname(listenfd, (struct sockaddr *)&serv_addr, &size) == -1)
	    perror("getsockname");

	*port=ntohs(serv_addr.sin_port);

    return (listenfd);
}

bool abstract_resolveIP(const char* hostname, size_t size, ip_address_ptr ip)
{
	struct hostent *he;
	struct in_addr **addr_list;
	int i;
	char hn[__HOSTNAME_MAX_LEN__+1];


	if(size>__HOSTNAME_MAX_LEN__)
	{
		return (true);
	}

	for (i = 0; i < __HOSTNAME_MAX_LEN__ && i<size; ++i) {
		hn[i]=hostname[i];
	}
	hn[i]='\0';

	if ( (he = gethostbyname( hn ) ) == NULL)
	{
		return (true);
	}
	addr_list = (struct in_addr **) he->h_addr_list;


	//Return only the first one;
	char * add=(char *)addr_list[0];

	for(i=0;i<4;i++)
		ip[i]=add[i];

	return(false);
}

bool abstract_stop_listening_on_port(socket_id_t socket_id)
{
	close(socket_id);
	return (1);
}

socket_id_t abstract_connect_socket(ip_address_ptr ip, port_t port)
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
	struct hostent *server;

	server=gethostbyaddr(ip,4,AF_INET);
    if (server == NULL) {
    	return (-1);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        return (-1);


    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
        bcopy((char *)server->h_addr,
             (char *)&serv_addr.sin_addr.s_addr,
             server->h_length);
        serv_addr.sin_port = htons(port);

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0)
            return (-1);
        {
        int flag = 1;
        int result = setsockopt(sockfd,            /* socket affected */
                                IPPROTO_TCP,     /* set option at TCP level */
                                TCP_NODELAY,     /* name of option */
                                (char *) &flag,  /* the cast is historical cruft */
                                sizeof(int));    /* length of option value */
        if (result < 0)
        	 return (-1);
        }

        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        {
        	close(sockfd);
        	return (-1);
        }
        fcntl(sockfd, F_SETFL, O_NONBLOCK);

    	printf("-----------------socket: %i \n",sockfd);
        return (sockfd);
}

send_result_t abstract_send_packet(socket_id_t socket_id, uint8_t*  buffer, uint32_t size)
{
	if(write(socket_id,buffer,size) == size)
	{
		return(SEND_RESULT_OK);
	}
	else
	{
		return(SEND_RESULT_CONNECTION_ERROR);
	}
}

void abstract_close_socket(socket_id_t socket_id)
{
	close(socket_id);
}

socket_id_t abstract_socket_accept(listen_socket_id_t socket_id)
{
	struct sockaddr cli_addr;
	int clilen = sizeof(cli_addr);
    int newsockfd = accept(socket_id,
                (struct sockaddr *) &cli_addr,
                &clilen);

    //NONBLOCK!!!
        int flag = 1;
        int result = setsockopt(newsockfd,            /* socket affected */
                                IPPROTO_TCP,     /* set option at TCP level */
                                TCP_NODELAY,     /* name of option */
                                (char *) &flag,  /* the cast is historical cruft */
                                sizeof(int));    /* length of option value */
        if (result < 0)
        {
        	close(newsockfd);
        	 return (-1);
        }

        fcntl(newsockfd, F_SETFL, O_NONBLOCK);

    if(newsockfd<0)
    	return(-1);
    else
    	return (newsockfd);
}

int32_t recv_packet(socket_id_t socket_id, uint8_t* buffer, uint32_t size)
{
	int n = read(socket_id,buffer,size);
	if(n<=0)
		switch(n)
		{
		case -1:
			return(SOCKET_SIG_NO_DATA);
			break;
		case 0:
			return(SOCKET_SIG_CLOSE);
			break;
		}
	return(n);
}

bool abstract_get_hostname(char * hostname, size_t maxlength)
{
	return (gethostname(hostname,maxlength));
}
