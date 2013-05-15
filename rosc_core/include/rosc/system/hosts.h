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
 *  hosts.h created by Christian Holl
 */

#ifndef HOSTS_H_
#define HOSTS_H_

#include <rosc/rosc.h>

typedef unsigned char ip_address_t[4];
typedef int16_t port_id_t;



#ifdef __SYSTEM_HAS_MALLOC__
	typedef char *hostname_t;
	typedef char *nodename_t;
#else
	typedef char hostname_t[__HOSTNAME_MAX_LEN__];
	typedef char nodename_t[__NODENAME_MAX_LEN__];
#endif

	typedef enum
	{
		HOST_TYPE_SLOT_UNUSED,
		HOST_TYPE_LOCALHOST,
		HOST_TYPE_MASTER,
		HOST_TYPE_MACHINE
	}host_type_t;


#define STATIC_HOST_LIST_HEAD\
		host_info_t host_list[]={

#define STATIC_HOST_LIST_FOOT\
		}

#define STATIC_HOST_LIST_ENTRY_MASTER(MASTER_IP, MASTER_HOSTNAME,XMLRPC_PORT)\
		{.host_type=HOST_TYPE_MASTER, .host_ip=MASTER_IP, .host_name=MASTER_HOSTNAME, .xmlrpc_port=XMLRPC_PORT},

#define STATIC_HOST_LIST_ENTRY_LOCALHOST(LOCAL_IP, LOCAL_HOSTNAME)\
		{.host_type=HOST_TYPE_LOCALHOST, .host_ip=LOCAL_IP, .host_name=LOCAL_HOSTNAME},

#define STATIC_HOST_LIST_ENTRY_MACHINE(MACHINE_IP, MACHINE_HOSTNAME)\
		{.host_type=HOST_TYPE_MACHINE, .host_ip=MACHINE_IP, .host_name=MACHINE_HOSTNAME}

typedef struct
{
#ifdef  __SYSTEM_HAS_MALLOC__
	struct node_info_t* next;
#endif
	host_type_t host_type;//!< Type of Host Master, Current System, Another Machine
	ip_address_t host_ip; //!< IP Address
	hostname_t host_name; //!< Hostname
	uint16_t xmlrpc_port;   //!< Port setting xmlrpc ports stores the xmlrpc port...
}host_info_t;

extern host_info_t host_list[];



#endif /* HOSTS_H_ */
