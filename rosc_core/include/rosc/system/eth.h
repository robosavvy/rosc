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
 *  eth.h created by Christian Holl
 */

#ifndef ETH_H_
#define ETH_H_

#include <rosc/system/types.h>
#include <rosc/system/setup.h>
#include <rosc/system/hosts.h>


typedef int16_t socket_id_t;
typedef uint16_t port_t;

typedef enum
{
	PORT_TYPE_HUB,
	PORT_TYPE_UNUSED,
	PORT_TYPE_INCOMING,
	PORT_TYPE_INCOMING_ACCEPT,
	PORT_TYPE_OUTGOING,
}socket_type_t;

typedef enum
{
	PORT_STATE_UNUSABLE,
	PORT_STATE_CLOSED,
	PORT_STATE_LISTEN,
	PORT_STATE_OUTGOING,
	PORT_STATE_INCOMING,
}socket_state_t;


typedef struct socket_t
{
	port_t port_number;
	sebs_parser_data_t pdata;
	struct iface_t *interface;
	void* data;
	socket_id_t socket_id;
	socket_type_t type;
	socket_state_t state;
	struct socket_t *next;
}socket_t;




void rosc_sockets_init();

/**
 * Opens a port for a specific interface
 * @param iface interface which will use the port
 * @return True if successfull, False if not
 */
bool rosc_use_socket( iface_t *iface, uint16_t port_number);



typedef enum
{
	PORT_STATUS_OPENING_FAILED,
	PORT_STATUS_OPENED,
	PORT_STATUS_CLOSED,
	PORT_STATUS_UNUSED,
}port_status_t;

/**
 * rosc uses this function to tell the network device to open a port.
 * Normally this function is called by port number 0, for
 * @param port port number
 * @return port number or 0 if failed
 */
extern port_t start_listening_on_port(port_t port);

extern port_status_t stop_listening_on_port(port_t port);

/**
 * This function is from rosc to send out data. It needs to be implemented
 * by the driver or platform package.
 * @param socket_id
 * @param buffer
 * @param size
 */
extern void send_packet(socket_id_t socket_id, uint8_t*  buffer, uint32_t size);

/**
 * This function is called by the driver when something is received on a
 * special socket. It then searches for the socket id inside the socket list
 * and calls the parsing function for that socket. It needs to be called
 * by the driver or platform package when there is data for a specific socket.
 * @param socket_id
 * @param buffer
 * @param size
 * @return
 */
uint32_t receive_packet(socket_id_t socket_id, uint8_t* buffer, uint32_t size);

extern socket_t* connect_socket(iface_t *iface, ip_address_t ip, port_t port);

extern void close_socket(socket_id_t socket);

#endif /* ETH_H_ */
