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

#include <rosc/system/setup.h>
#include <rosc/sebs_parse_fw/sebs_parser_frame.h>
#include <rosc/system/spec.h>
#include <rosc/com/ros_msg_common.h>
#include <rosc/system/status.h>
#include <rosc/debug/debug_out.h>

typedef uint8_t ip_address_t[4];
typedef uint8_t* ip_address_ptr;
typedef int16_t port_id_t;

typedef __SOCKET_ID_TYPE__ socket_id_t;
typedef __SOCKET_ID_TYPE__ listen_socket_id_t;

typedef uint16_t port_t;

extern char hostname[];
extern char node_name[];




/**
 * This enum contains the different interface states
 */
typedef enum
{
	IFACE_STATE_RPC_INTERFACE, //!< interface is a rpc interface, no handling required
	IFACE_STATE_UNREGISTERED, //!< interface is not registered
	IFACE_STATE_DO_REGISTER,  //!< interface is should be registered
	IFACE_STATE_STATE_OPERATION_PENDING, //!< interface registering operation in progress
	IFACE_STATE_REGISTERED,   //!< interface is registered
	IFACE_STATE_DO_UNREGISTER,//!< interface should be unregistered
}iface_state_t;

/**
 * This is the definition of the struct containing various information for
 * an interface
 */
typedef struct iface_t
{
	sebs_parse_function_t handler_function;
	void *init_data;
	iface_state_t state;
	struct iface_t *next;
}iface_t;

bool iface_list_insert(iface_t *interface);
bool unregister_interface(iface_t *interface);
bool register_interface(iface_t *interface);


typedef enum
{
	LISTEN_SOCKET_STATE_CLOSED,
	LISTEN_SOCKET_STATE_UNUSED,
	LISTEN_SOCKET_STATE_ACTIVE,
}listen_socket_state_t;

typedef enum
{
	SOCKET_STATE_INACTIVE, 				/*!< socket can be obtained for usage */
	SOCKET_STATE_SETUP,					/*!< socket is reserved already and being setup (means it's not called by ros_spin) */
	SOCKET_STATE_NOT_CONNECTED,			/*!< socket is not connected */
	SOCKET_STATE_WAITING_FOR_CONNECTION,	/*!< socket is currently waiting for a connection */
	SOCKET_STATE_CONNECTED,				/*!< socket is connected and able to receive or send data */
}socket_state_t;


typedef enum
{
	CONNECT_DATA_STATE_URL,	/*!< currently there is only a url which needs to be parsed*/
	CONNECT_DATA_STATE_RESOLVE, /*!< the hostname is available */
	CONNECT_DATA_STATE_IPV4, /*!< IPv4 and port are ready */
	//TODO support IPv6 CONNECT_DATA_STATE_IPV6, /*!< IPv6 and port ready*/
	CONNECT_DATA_STATE_ERROR,
}connect_data_state_t;


typedef struct socket_connect_info_t
{
	connect_data_state_t data_state; /*!< contains the state of the data*/
	ip_address_t remote_ip;	/*!< The ip of the remote system*/
	port_t remote_port;		/*!< The port of the remote system*/
	uint32_t hostname_size;	/*!< The length of the hostname*/
	char *hostname;	/*!< points to the start of the hostname in connect_string*/
	char url[__URI_MAX_LENGTH__];		/*!< storage for urls */
}socket_connect_info_t;

/**
 * This defines the state of the data parsing
 */
typedef enum
{
	SOCKET_CONNECT_STATE_URL_SCHEME,       //!< State when processing is at URL scheme
	SOCKET_CONNECT_STATE_URL_IPV4_HOSTNAME,//!< State when processing got an IP or till the current char it can be a IP address
	SOCKET_CONNECT_STATE_URI_HOSTNAME,     //!< State when processing found out its a hostname
	SOCKET_CONNECT_STATE_URL_IPV6,         //!< UNUSED state for IPV6 support ...
	SOCKET_CONNECT_STATE_URL_PORT,         //!< State when processing the port information
	SOCKET_CONNECT_STATE_CONNECT,          //!< State when information can be used to connect
	SOCKET_CONNECT_STATE_ERROR,            //!< State when something was not correct
}socket_connect_state_t;

typedef struct
{
	socket_connect_state_t state;
	socket_connect_info_t* connect_data;
}socket_connect_data_t;

typedef struct lookup_table_entry_t
{
	char hostname[__HOSTNAME_BUFFER_LEN__];
	ip_address_t ip;
}lookup_table_entry_t;

#define ROSC_STATIC_LOOKUP_TABLE_HEAD(MIN_SIZE)\
lookup_table_entry_t __rosc_static_lookup_table[MIN_SIZE]=\
{\

#define ROSC_STATIC_LOOKUP_TABLE_END \
};\
lookup_table_entry_t* rosc_static_lookup_table=(&__rosc_static_lookup_table[0]);\
size_t rosc_static_lookup_table_size=sizeof(__rosc_static_lookup_table)/sizeof(lookup_table_entry_t);

#define ROSC_STATIC_LOOKUP_ENTRY(HOSTNAME, IP)\
{#HOSTNAME, IP},



typedef struct socket_t
{
	socket_state_t state;	/*!< If the this socket is used, this value is set to true*/

	socket_connect_info_t connect_info;

	socket_id_t socket_id;/*!< This stores the socket id of the connection on the target system*/
	struct iface_t *reserved;/*!< If this is not 0 the socket is reserved for a special interface*/

	struct iface_t *iface;/*!< This stores the current interface */

	sebs_parser_data_t pdata;/*!< This is state information of the parser of the interface */
	void* data; /*!< This is the data which is reserved for the interface */
	struct socket_t *next;/*!< If not 0 it points to the next entry of the interface list*/

}socket_t;

typedef struct listen_socket_t
{
	port_t port;
	listen_socket_id_t id;
	listen_socket_state_t state;
	struct iface_t *interface;
	struct listen_socket_t *next;
}listen_socket_t;


extern socket_t* socket_list_start;
extern listen_socket_t* listen_socket_list_start;
extern iface_t* interface_list_start;

typedef enum
{
	SEND_RESULT_OK=0,
	SEND_RESULT_CONNECTION_CLOSE,
	SEND_RESULT_CONNECTION_ERROR,
	SEND_RESULT_CONNECTION_TIMEOUT,
}send_result_t;


#define MASTER_URI_STATIC(URI)\
		size_t master_uri_size=sizeof(URI)-1;\
		char * const master_uri=URI

#define MASTER_URI_STATIC_MEMRES(URI,MEMSIZE)\
		size_t master_uri_size=MEMSIZE;\
		char __master_uri__mem[MEMSIZE+1]=URI;\
		char * const master_uri=&__master_uri__mem

/**
 * Pointer to zero terminated master URI
 */
extern char * const master_uri;
extern port_t xmlrpc_port;




void rosc_lists_init();

/**
 * Opens a port for a specific interface
 * @param iface interface which will use the port
 * @return True if successfull, False if not
 */
bool rosc_iface_listen( iface_t *iface, uint16_t port_number);

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


/**
 * This function resolves a hostname, to do this it first uses the built in host list
 * and if the host can not be found there, it uses the abstract resolve command
 * @param hostname [in] A pointer to the hostname
 * @param size the size of the hostname
 * @param ip [out] The ip address output
 * @return false if the ip could be obtained
 */
bool rosc_hostlist_resolve(const char* hostname, size_t size, ip_address_ptr ip);

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
 * @param hostname The hostname (NOT TERMINATED BY ZERO!)
 * @param size The hostname length
 * @param ip[o] The storage for the ip address
 * return false if successfull
 */
extern bool abstract_resolveIP(const char* hostname, size_t size, ip_address_ptr ip);

/**
 * rosc uses this function to tell the network device to open a port.
 * Normally this function is called by port number 0, for
 * @param[io] port port number
 * @return socket_id or -1 if failed
 */
socket_id_t abstract_start_listening_on_port(port_t* port);

/**
 * rosc uses this function for closing a server port, if the current platform or hardware
 * does not support closing ports, just return PORT_STATUS_UNUSED
 * @param socket the socket info of the port
 * @return PORT_STATUS_CLOSED or if not supported PORT_STATUS_UNUSED
 */
extern bool abstract_stop_listening_on_port(socket_id_t socket_id);

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

/**
 * These signals are used to receive information about socket information or to control the socket,
 * or both.
 *
 * They are used instead of an input or output size.
 */
typedef enum
{
	SOCKET_SIG_COULD_NOT_CONNECT=-9, /*!< (In) Could not connect to ip*/
	SOCKET_SIG_COULD_NOT_RESOLVE_HOST =-8, /*!< (In) Could not resolve hostname*/
	SOCKET_SIG_RELEASE = -7, /*! (Out) Close socket, release memory<*/
	SOCKET_SIG_TIMEOUT = -6,/*! (In) Timeout occurred <*/
	SOCKET_SIG_CONNECT = -5, /*! (Out) Connect socket to connection stored inside additional memory<*/
	SOCKET_SIG_NO_CONNECTION = -4, /*! (In) Socket not connected <*/
	SOCKET_SIG_CONNECTED = -3,     /*!(In) Socket connection established <*/
	SOCKET_SIG_DATA_SENT = -2, /*! (In) Data was sent to remote host <*/
	SOCKET_SIG_NO_DATA = -1, /*! (In/Out) No data on connection <*/
	SOCKET_SIG_CLOSE = 0, /*! (In/Out) Close Socket / Socket closed <*/
}socket_sig_t;

extern int32_t recv_packet(socket_id_t socket_id, uint8_t* buffer, uint32_t size);


extern void abstract_close_socket(socket_id_t socket);


/**
 * This routine is exectuted every time ros spin is called.
 */
extern void abstract_ros_spin_routine();

/**
 * Accept sockets for polling
 */
extern socket_id_t abstract_socket_accept(listen_socket_id_t socket_id);

extern const size_t rosc_static_socket_additional_data_size;

extern bool abstract_get_hostname(char * hostname, size_t maxlength);

#endif /* ETH_H_ */
