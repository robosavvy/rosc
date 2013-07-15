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
 *  iface.h created by Christian Holl
 */


#ifndef IFACE_H_
#define IFACE_H_

#include <rosc/system/ports.h>
#include <rosc/system/spec.h>
#include <rosc/com/ros_msg_common.h>


/**
 * This enum specifies the available interfaces types
 */
typedef enum
{
	IFACE_TYPE_LIST_HUB,
	IFACE_TYPE_SERVICE_SERVER,
	IFACE_TYPE_SERVICE_CLIENT,
	IFACE_TYPE_TOPIC_PUBLISHER,
	IFACE_TYPE_TOPIC_SUBSCRIBER,
	IFACE_TYPE_XMLRPC_CLIENT,
	IFACE_TYPE_XMLRPC_SERVER,
	IFACE_TYPE_ROSRPC_CLIENT,
	IFACE_TYPE_ROSRPC_SERVER,
}iface_type_t;

/**
 * This enum contains the different interface states
 */
typedef enum
{
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
	iface_type_t type;
	char *name;	//!< This is the topic / server name of the interface
	iface_state_t state;
	ros_msg_buildup_t *buildup;
	size_t *submessage_sizes;
	size_t *array_lengths;
	size_t *memory_offsets;
	struct iface_t *next;
}iface_t;





void rosc_init_interface_list();
void register_interface(iface_t *interface);
void unregister_interface(iface_t *interface);



#endif /* IFACE_H_ */
