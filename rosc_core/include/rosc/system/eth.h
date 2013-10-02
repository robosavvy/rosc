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
#include <rosc/system/spec.h>
#include <rosc/sebs_parse_fw/sebs_parser_frame.h>
#include <rosc/com/ros_msg_common.h>

#ifndef SOCKET_ID_TYPE
#define SOCKET_ID_TYPE int32_t
#warning SOCKET_ID_TYPE undefined, now set to int32_t
#endif

typedef uint8_t ip_address_t[4];
typedef uint8_t* ip_address_ptr;
typedef int16_t port_id_t;
typedef SOCKET_ID_TYPE socket_id_t;
typedef uint16_t port_t;

extern char host_name[];
extern char node_name[];

/**
 * This enum contains the different interface states
 */
typedef enum
{
	IFACE_STATE_RPC_INTERFACE,
	IFACE_STATE_UNREGISTERED, //!< IFACE_STATE_UNREGISTERED
	IFACE_STATE_DO_REGISTER,  //!< IFACE_STATE_DO_REGISTER
	IFACE_STATE_WAIT_REGISTERED, //!< IFACE_STATE_WAIT_REGISTERED
	IFACE_STATE_REGISTERED,   //!< IFACE_STATE_REGISTERED
	IFACE_STATE_DO_UNREGISTER,//!< IFACE_STATE_DO_UNREGISTER
	IFACE_STATE_WAIT_UNREGISTERED, //!< IFACE_STATE_WAIT_UNREGISTERED
}iface_state_t;

/**
 * This is the definition of the struct containing various information for
 * an interface
 */
typedef struct iface_t
{
#ifdef __SYSTEM_HAS_MALLOC__
	size_t handler_mem_size;
#endif
	bool isListHub;
	sebs_parse_function_t handler_function;
	void *init_data;
	iface_state_t state;
	struct iface_t *next;
}iface_t;

void rosc_init_interface_list();
void register_interface(iface_t *interface);
void unregister_interface(iface_t *interface);


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
	SOCKET_STATE_UNUSABLE,
	SOCKET_STATE_CLOSED,
	SOCKET_STATE_OUTGOING,
	SOCKET_STATE_INCOMING,
}socket_state_t;

typedef enum
{
	PORT_STATE_CLOSED,
	PORT_STATE_LISTEN,
	PORT_STATE_UNUSED,
}port_state_t;

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

typedef struct listen_socket_t
{
	port_t port;
	socket_id_t id;
	port_state_t state;
	struct iface_t *interface;
}listen_socket_t;

typedef enum
{
	SEND_RESULT_OK=0,
	SEND_RESULT_CONNECTION_CLOSE,
	SEND_RESULT_CONNECTION_ERROR,
	SEND_RESULT_CONNECTION_TIMEOUT,
}send_result_t;

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
 * This function is called by the driver when something is received on a
 * special socket. It then searches for the socket id inside the socket list
 * and calls the parsing function for that socket. It needs to be called
 * by the driver or platform package when there is data for a specific socket.
 * @param socket_id
 * @param buffer
 * @param size
 */
void receive_packet(socket_id_t socket_id, uint8_t* buffer, uint32_t size);




/* ********************************
 **********************************
 * FUNCTIONS TO BE IMPLEMENTED BY *
 * PLATFORM OR HARDWARE PACKAGE   *
 **********************************
 **********************************/

/**
 * This function initializes the hostname
 */
extern void abstract_static_initHostname();

/**
 * This function resolves the IP from a hostname
 * @param hostname The hostname terminated with 0
 * @param ip[o] The storage for the ip address
 * return false if successfull
 */
extern bool abstract_resolveIP(const char* hostname, uint8_t* ip);

/**
 * rosc uses this function to tell the network device to open a port.
 * Normally this function is called by port number 0, for
 * @param[io] port port number
 * @return socket_id or 0 if failed
 */
socket_id_t abstract_start_listening_on_port(port_t* port);

/**
 * rosc uses this function for closing a server port, if the current platform or hardware
 * does not support closing ports, just return PORT_STATUS_UNUSED
 * @param socket the socket info of the port
 * @return PORT_STATUS_CLOSED or if not supported PORT_STATUS_UNUSED
 */
extern port_status_t abstract_stop_listening_on_port(socket_id_t socket_id);

/**
 * Connect to a external server by ip and port
 * @param iface
 * @param ip
 * @param port
 * @return
 */
socket_id_t abstract_connect_socket(ip_address_t ip, port_t port);

/**
 * This function is from rosc to send out data. It needs to be implemented
 * by the driver or platform package.
 * @param socket_id the socket_id
 * @param buffer the output buffer
 * @param size
 * @return result of sending
 */
extern send_result_t abstract_send_packet(socket_id_t socket_id, uint8_t*  buffer, uint32_t size);

extern void abstract_close_socket(socket_id_t socket);


/**
 * This routine is exectuted every time ros spin is called.
 */
extern void abstract_ros_spin_routine();

#endif /* ETH_H_ */
