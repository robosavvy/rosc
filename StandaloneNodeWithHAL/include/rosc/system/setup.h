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

#include <rosc/rosc.h>
#include <stdbool.h>
#include <inttypes.h>

#ifdef _TEST_DEFINE_
#error testdefine
#endif

#ifndef _SETUP_H_
#define _SETUP_H_

typedef unsigned char ip_address_t[4];
typedef uint16_t port_id_t;
typedef uint16_t port_t;
typedef char nodename_t[50];
typedef enum
{
	NODETYPE_UNUSED,
	NODETYPE_SELF,
	NODETYPE_MASTER,
	NODETYPE_NODE
}node_type_t;


extern nodename_t node_name;
extern ip_address_t node_ip;
extern ip_address_t master_ip;
extern port_t master_port;

#define IP_ADDR(IP3,IP2,IP1,IP0)\
		{IP3,IP2,IP1,IP0}

#define ROSC_SYSTEM_SETTING(NODENAME,NODE_IP,MASTER_IP,MASTER_PORT)\
	nodename_t node_name=NODENAME;\
	ip_address_t node_ip=NODE_IP;\
	ip_address_t master_ip=MASTER_IP;\
	port_t master_port=MASTER_PORT


typedef struct
{
#ifdef  __SYSTEM_HAS_MALLOC__
	struct node_info_t* next;
#endif
	node_type_t node_type;
	ip_address_t node_ip;
	nodename_t nodename;
}node_info_t;


#endif /*_SETUP_H_*/
