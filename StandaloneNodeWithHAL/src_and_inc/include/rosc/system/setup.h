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
#include <rosc/system/types.h>
#include <rosc/system/spec.h>
#include <rosc/system/hosts.h>

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

	#ifndef __SYSTEM_HAS_OS__
		#ifndef __DEFAULT_XMLRPC_PORT_OVERRIDE__
			#define XMLRPC_PORT 8284
		#else
			#define XMLRPC_PORT __DEFAULT_XMLRPC_PORT_OVERRIDE__
		#endif
	#else
		#define XMLRPC_PORT 0  //just open an available port
	#endif
#endif

#ifndef __SYSTEM_MEM_ALLOC_BIG_ENDIAN__
	#ifndef __SYSTEM_MEM_ALLOC_LITTLE_ENDIAN__
		#error __SYSTEM_MEM_ALLOC_<TYPE>_ENDIAN  not set!
	#endif
#endif




typedef enum
{
	CONNECTION_TYPE_OUTGOING,
#ifdef __SYSTEM_NEEDS_CON_ACCEPT__
	CONNECTION_TYPE_INCOMING_ACCEPTED,
	CONNECTION_TYPE_INCOMING_ACCEPTING_PORT,
#else
	CONNECTION_TYPE_INCOMING,
#endif
}connection_type_t;


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



#endif /*_SETUP_H_*/
