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
	struct iface_t* interface;
	void *data;
	uint32_t socket_id;
}port_t;





#endif /* PORTS_H_ */
