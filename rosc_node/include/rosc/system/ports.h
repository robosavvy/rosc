/*
 * ports.h
 *
 *  Created on: 03.05.2013
 *      Author: choll
 */

#ifndef PORTS_H_
#define PORTS_H_

#include <inttypes.h>
#include <rosc/com_ifaces/iface.h>
#include <rosc/com_xml/parse/parser_structure.h>

#define STATIC_SYSTEM_MESSAGE_TYPE_LIST_BEGIN\
	typedef struct\
	{\
		union\
		{\
			parse_act_t parser_act_size_placeholder;\


#define STATIC_SYSTEM_MESSAGE_TYPE_LIST_END\
		};\
	}static_system_general_port_handler_storage_t;



typedef enum
{
	PORT_TYPE_UNUSED,
	PORT_TYPE_INCOMING_CONNECTED,
	PORT_TYPE_INCOMING_ACCEPT,
	PORT_TYPE_OUTGOING,
}port_type_t;

typedef struct
{
	uint16_t port_number;
	iface_t interface;
	void *data;
	uint32_t socket_id;
}port_t;





#endif /* PORTS_H_ */
