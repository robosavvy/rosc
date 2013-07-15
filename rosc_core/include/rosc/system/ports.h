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
 *  ports.h created by Christian Holl
 */
#ifndef PORTS_H_
#define PORTS_H_

#include <rosc/system/types.h>
//#include <rosc/sebs_parse_fw/sebs_parser_frame.h>
//#include <rosc/com/xmlrpc_server.h>
//#include <rosc/com/ros_handler.h>
#include <rosc/system/iface.h>

typedef enum
{
	PORT_TYPE_HUB,
	PORT_TYPE_UNUSED,
	PORT_TYPE_INCOMING,
	PORT_TYPE_INCOMING_ACCEPT,
	PORT_TYPE_OUTGOING,
}port_type_t;

typedef enum
{
	PORT_STATE_UNUSABLE,
	PORT_STATE_CLOSED,
	PORT_STATE_LISTEN,
	PORT_STATE_OUTGOING,
	PORT_STATE_INCOMING,
}port_state_t;




typedef struct port_t
{
	uint16_t port_number;
	struct iface_t *interface;  //!< Interface connected to port
	void* data;
	uint32_t socket_id;
	port_type_t type;
	port_state_t state;
	struct port_t *next;
}port_t;


void rosc_ports_init();

/**
 * Opens a port for a specific interface
 * @param iface interface which will use the port
 * @return True if successfull, False if not
 */
bool rosc_open_port(struct iface_t *iface);




#endif /* PORTS_H_ */
