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

typedef enum
{
	IFACE_DEFINITION_TYPE_XMLRPC_SERVER,
	IFACE_DEFINITION_TYPE_XMLRPC_CLIENT,

	IFACE_DEFINITON_TYPE_TOPIC_SUBSCRIBER,
	IFACE_DEFINITON_TYPE_TOPIC_PUBLISHER,

	IFACE_DEFINITON_TYPE_SERVICE_SERVER,
	IFACE_DEFINITON_TYPE_SERVICE_CLIENT,


	IFACE_DEFINITION_SUBMESSAGE,
	IFACE_DEFINITION_SUBMESSAGE_END,
	IFACE_DEFINITION_SUBMESSAGE_ARRAY,

	IFACE_DEFINITION_INT8,
	IFACE_DEFINITION_INT16,
	IFACE_DEFINITION_INT32,
	IFACE_DEFINITION_INT64,

	IFACE_DEFINITION_UINT8,
	IFACE_DEFINITION_UINT16,
	IFACE_DEFINITION_UINT32,
	IFACE_DEFINITION_UINT64,

	IFACE_DEFINITION_FLOAT32,
	IFACE_DEFINITION_FLOAT64,

	IFACE_DEFINITION_TIME,
	IFACE_DEFINITION_DURATION,
	IFACE_DEFINITION_BOOL,
	IFACE_DEFINITION_STRING,

	IFACE_DEFINITION_MSG_END,
	IFACE_DEFINITION_RESULT_END,
}iface_definition_t;

typedef enum
{
	IFACE_TYPE_LIST_HUB,
	IFACE_TYPE_SERVICE_SERVER,
	IFACE_TYPE_SERVICE_CLIENT,
	IFACE_TYPE_TOPIC_PUBLISHER,
	IFACE_TYPE_TOPIC_SUBSCRIBER,
}iface_type_t;

typedef enum
{
	IFACE_STATE_UNREGISTERED,
	IFACE_STATE_DO_REGISTER,
	IFACE_STATE_REGISTERED,
	IFACE_STATE_DO_UNREGISTER,
}iface_state_t;


typedef struct iface_t
{
	iface_type_t type;
	char* name;
	struct iface_t* next;
	void (*init_mem_function) (void* data);
}iface_t;


void initInterfaceList();
void registerInterface_static(iface_t *interface, const char *interfacename, const iface_definition_t* iface_def);
void unregisterInterface(iface_t *interface);



#endif /* IFACE_H_ */
