/*
 * ports.h
 *
 *  Created on: 03.02.2013
 *      Author: cyborg-x1
 */

#ifndef PORTS_H_
#define PORTS_H_

#include <rosc/system/eth.h>


typedef enum
{
	PORT_USE_UNUSED,		//!<Marks currently available port slots
	PORT_USE_ROSTCP_PUB,    //!<Outgoing port for ROSTCP
	PORT_USE_XMLRPC_CLIENT, //!<Outgoing port for Requests to ROS Master/other Node
	PORT_USE_XMLRPC_SERVER, //!<Incoming port for XMLRPC requests
}port_user_type_t;

typedef struct
{
	port_user_type_t port_usage;
    uint32_t user_number;
	port_id_t port_id;
	port_t port;
}port_info;

extern unsigned int open_ports_count;


#endif /* PORTS_H_ */
