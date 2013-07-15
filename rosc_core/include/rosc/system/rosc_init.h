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
 *  rosc_init.h created by Christian Holl
 */

#ifndef ROSINIT_H_
#define ROSINIT_H_

#include <rosc/system/types.h>
#include <rosc/system/ports.h>
#include <rosc/system/spec.h>
#include <rosc/sebs_parse_fw/sebs_parser_frame.h>
#include <rosc/com/ros_handler.h>
#include <rosc/com/xmlrpc_server.h>


#ifndef  __SYSTEM_HAS_MALLOC__

	/**
	 * STATIC_SYSTEM_MESSAGE_TYPE_LIST_BEGIN
	 * defines the begin of the list which must contain all
	 * port types on static systems
	 */
	#define ROSC_STATIC_SYSTEM_MESSAGE_TYPE_LIST_BEGIN\
		typedef struct\
		{\
					sebs_parser_data_t pdata;\
					union\
					{\
						xmlrpc_data_t xml;\
						struct\
						{\
							ros_handler_data_t hdata;\
							union\
							{

	/**
	 * STATIC_SYSTEM_MESSAGE_TYPE_LIST_END
	 * defines the end of the list which must contain all
	 * port types on static systems, it also sets up
	 * the necessary external variables for the memory size
	 * of all port buffers
	 */
	#define ROSC_STATIC_SYSTEM_MESSAGE_TYPE_LIST_END\
							}message_data;\
						}ros;\
					}handler;\
		}rosc_port_memory_size_def_t;\
		const size_t rosc_static_port_mem_size=sizeof(rosc_port_memory_size_def_t);\
		rosc_port_memory_size_def_t rosc_static_port_mem[PORTS_STATIC_MAX_NUMBER];
#endif


#include <rosc/com/ros_handler.h>

/**
 * This function initializes a rosc client
 */
void rosc_init();

#endif /* ROSINIT_H_ */
