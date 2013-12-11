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

#include <rosc/system/spec.h>
#include <rosc/system/eth.h>
#include <rosc/system/spec.h>
#include <rosc/sebs_parse_fw/sebs_parser_frame.h>
#include <rosc/com/ros_handler.h>
#include <rosc/com/xmlrpc.h>
#include <rosc/system/mem.h>




#ifndef  __SYSTEM_HAS_MALLOC__

	/**
	 * STATIC_SYSTEM_MESSAGE_TYPE_LIST_BEGIN
	 * defines the begin of the list which must contain all
	 * port types on static systems
	 */
	#define ROSC_STATIC_SYSTEM_MESSAGE_TYPE_LIST_BEGIN\
		typedef struct\
		{\
			union\
			{\
						xmlrpc_data_t xml;\
						ros_handler_data_t ros;\
			}handler;\
			union\
			{\
				socket_connect_info_t	connect_info;\
				char connect_string_reservation[offsetof(socket_connect_info_t,url) + __URI_BUFFER_LENGTH__];
				//TODO char topic_name_string_reservation[];

	/**
	 * STATIC_SYSTEM_MESSAGE_TYPE_LIST_END
	 * defines the end of the list which must contain all
	 * port types on static systems, it also sets up
	 * the necessary external variables for the memory size
	 * of all port buffers
	 *
	 * Additional information (rosc developers only):
	 * We need the type and array size variables here because the port memory size type is
	 * not known to rosc functions because it would force the user to have a separate
	 * setup header file which needs to be included by all rosc stuff requiring this
	 * information. To get arround this, the information is passed by "external" variables,
	 * these are setup here.
	 */
	#define ROSC_STATIC_SYSTEM_MESSAGE_TYPE_LIST_END\
			}message_data;\
		}rosc_socket_memory_size_def_t;\
		\
		const size_t rosc_static_socket_mem_size=sizeof(rosc_socket_memory_size_def_t);\
		rosc_socket_memory_size_def_t rosc_static_socket_mem[__SOCKET_MAXIMUM__];\
		const size_t rosc_static_socket_mem_hdata_offset=offsetof(rosc_socket_memory_size_def_t,handler);\
		const size_t rosc_static_socket_mem_message_offset=offsetof(rosc_socket_memory_size_def_t,message_data);\
		const size_t rosc_static_socket_additional_data_size=sizeof(rosc_socket_memory_size_def_t)-offsetof(rosc_socket_memory_size_def_t,message_data);\
		const size_t rosc_static_url_max_size=sizeof(rosc_socket_memory_size_def_t)-offsetof(rosc_socket_memory_size_def_t,message_data)-offsetof(socket_connect_info_t,url);
#endif

/**
 * The output buffer of all XMLRPC and ROSRPC services is in total as big as the biggest message size
 * if more performance is required, it might be an option to increase the buffer size (if there is unused memory left)
 * Additionally must be mentioned, that the memory size can even if it's set to 100, can still be a bit larger because of
 * padding bytes of the created structs. Those bytes will be used as output buffer as well.
 */
#define ROSC_SIZE_LIST_ENTRY_MIN_XMLRPC_OUTPUT_BUFFER(SIZE)\
	char rosc_size_list_min_output_buffer[SIZE];


#include <rosc/com/ros_handler.h>

/**
 * This function initializes a rosc client
 */
void rosc_init();

#endif /* ROSINIT_H_ */
