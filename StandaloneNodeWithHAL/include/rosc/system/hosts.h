/*
 * hosts.h
 *
 *  Created on: 03.02.2013
 *      Author: cyborg-x1
 */

#ifndef HOSTS_H_
#define HOSTS_H_


#ifdef __SYSTEM_HAS_MALLOC__
	typedef char *hostname_t;
	typedef char *nodename_t;
#else
	typedef char hostname_t[__HOSTNAME_MAX_LEN__];
	typedef char nodename_t[__NODENAME_MAX_LEN__];
#endif

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

extern host_info_t host_list[];

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


typedef enum
{
	HOST_TYPE_SLOT_UNUSED,
	HOST_TYPE_LOCALHOST,
	HOST_TYPE_MASTER,
	HOST_TYPE_MACHINE
}host_type_t;

#endif /* HOSTS_H_ */
