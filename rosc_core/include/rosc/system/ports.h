/*
 * ports.h
 *
 *  Created on: 03.02.2013
 *      Author: cyborg-x1
 */

#ifndef PORTS_H_
#define PORTS_H_

#include <rosc/system/eth.h>
#include <rosc/system/spec.h>

typedef enum
{
	PORT_USER_TYPE_UNUSED,		  //!<Marks currently available port slots
	PORT_USER_TYPE_ROSTCP_PUB,    //!<Outgoing port for ROSTCP Publisher
	PORT_USER_TYPE_ROSTCP_SUB,	  //!<Incoming port for ROSTCP Subscriber
	PORT_USER_TYPE_XMLRPC_CLIENT, //!<Outgoing port for Requests to ROS Master/other Node
	PORT_USER_TYPE_XMLRPC_SERVER, //!<Incoming port for XMLRPC requests
}port_user_type_t;

typedef uint32_t port_uin_t;

typedef struct
{
	port_user_type_t port_user_type;
    port_uin_t user_number;
	port_t port;
}port_info;

extern port_info ports_info[__PORT_MAXIMUM__];
extern unsigned int open_ports_count;


//inline void ports_info_init();

port_id_t listenPort(port_t port, port_user_type_t port_user_type, port_uin_t port_user_number);
void closePort(port_id_t);


#endif /* PORTS_H_ */
