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
 * setup.h
 *
 * Created on: 12.01.2013
 *     Author: Christian Holl
 */


#ifndef _SETUP_H_
#define _SETUP_H_

#include <rosc/rosc.h>
#include <stdbool.h>
#include <inttypes.h>
#include <system_spec.h>


#ifndef __SYSTEM_HAS_MALLOC__
	#ifndef __HOSTNAME_MAX_LEN__
		#define __HOSTNAME_MAX_LEN__ 50
		#warning __HOSTNAME_MAX_LEN__ undefined, it will be automatically set to 50
	#endif
	#ifndef __NODENAME_MAX_LEN__
		#define __NODENAME_MAX_LEN__ 50
		#warning __NODENAME_MAX_LEN__ undefined, it will be automatically set to 50
	#endif
	#ifndef __PORT_MAXIMUM__
		#define __PORT_MAXIMUM__
		#warning __PORT_MAXIMUM__ undefined, it will be automatically set to 8
	#endif
#endif

#ifndef __SYSTEM_HAS_OS__
	#ifndef __DEFAULT_XMLRPC_PORT_OVERRIDE__
		#define XMLRPC_PORT 8284
	#else
		#define XMLRPC_PORT __DEFAULT_XMLRPC_PORT_OVERRIDE__
	#endif
#else
	#define XMLRPC_PORT 0  //just open an available port
#endif


#ifndef __SYSTEM_MEM_ALLOC_BIG_ENDIAN__
	#ifndef __SYSTEM_MEM_ALLOC_LITTLE_ENDIAN__
		#error __SYSTEM_MEM_ALLOC_<TYPE>_ENDIAN  not set!
	#endif
#endif



typedef unsigned char ip_address_t[4];
typedef int16_t port_id_t;
typedef uint16_t port_t;

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
	HOST_TYPE_SELF,
	HOST_TYPE_MASTER,
	HOST_TYPE_NODE
}host_type_t;

extern hostname_t node_name;
extern ip_address_t node_ip;
extern ip_address_t master_ip;

#define IP_ADDR(IP3,IP2,IP1,IP0)\
		{IP3,IP2,IP1,IP0}

#define ROSC_SYSTEM_SETTING(NODE_NAME,NODE_IP,MASTER_IP,MASTER_PORT)\
	hostname_t node_name=NODE_NAME;\
	ip_address_t node_ip=NODE_IP;\
	ip_address_t master_ip=MASTER_IP;\
	port_t master_port=MASTER_PORT

typedef struct
{
#ifdef  __SYSTEM_HAS_MALLOC__
	struct node_info_t* next;
#endif
	host_type_t host_type;
	ip_address_t host_ip;
	hostname_t host_name;
	port_t xmlrpc_port; //!< Port setting for master xmlrpc or current node xmlrpc port, unused for other node types
}host_info_t;


#endif /*_SETUP_H_*/
