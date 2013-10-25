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
 *  setup.h created by Christian Holl
 */


#ifndef _SETUP_H_
#define _SETUP_H_

//#include <rosc/rosc.h>
#include <rosc/system/spec.h>

#define HOST_NAME(INITIAL_NAME)\
	char host_name[__HOSTNAME_MAX_LEN__]=INITIAL_NAME;

#define NODE_NAME(INITIAL_NAME)\
	char node_name[__NODENAME_MAX_LEN__]=INITIAL_NAME;


#define MASTER_HOST_NAME(INITIAL_NAME)\
	hostname_t master_hostname=INITIAL_NAME;

#define MASTER_PORT(INITIAL_PORT)\
	 master_port=INITIAL_NAME;


#ifndef __SYSTEM_HAS_MALLOC__

	#ifndef __HOSTNAME_MAX_LEN__
		#define __HOSTNAME_MAX_LEN__ 50
		#warning __HOSTNAME_MAX_LEN__ undefined, it will be automatically set to 50
	#endif

	#define __HOSTNAME_BUFFER_LEN__ __HOSTNAME_MAX_LEN__+1

    #define __URI_BUFFER_LENGTH__ 9 /*rosrpc://*/ + __HOSTNAME_BUFFER_LEN__ + 6 /*:PORTNUMBER*/




	#ifndef __NODENAME_MAX_LEN__
		#define __NODENAME_MAX_LEN__ 50
		#warning __NODENAME_MAX_LEN__ undefined, it will be automatically set to 50
	#endif

	#ifndef __SOCKET_MAXIMUM__
		#define __SOCKET_MAXIMUM__ 8
		#warning __SOCKET_MAXIMUM__ undefined, it will be automatically set to 8
	#endif

	#ifndef __LISTENING_SOCKET_MAXIMUM__
		#define	__LISTENING_SOCKET_MAXIMUM__ 8
		#warning __LISTENING_SOCKET_MAXIMUM__ undefined, it will be automatically set to 8
	#endif

	#ifndef __SOCKET_ID_TYPE__
		#define __SOCKET_ID_TYPE__ int32_t
		#warning __SOCKET_ID_TYPE__ undefined, now set to int32_t
	#endif


	#ifndef __SYSTEM_HAS_OS__
		#ifndef __DEFAULT_XMLRPC_PORT_OVERRIDE__
			#define XMLRPC_PORT 8284
		#else
			#define XMLRPC_PORT __DEFAULT_XMLRPC_PORT_OVERRIDE__
		#endif
	#else
		#define XMLRPC_PORT 0  //just open an available port
	#endif
#endif

#ifndef __SYSTEM_MEM_ALLOC_BIG_ENDIAN__
	#ifndef __SYSTEM_MEM_ALLOC_LITTLE_ENDIAN__
		#error __SYSTEM_MEM_ALLOC_<TYPE>_ENDIAN  not set!
	#endif
#endif


#define IP(IP3,IP2,IP1,IP0)\
		{IP3,IP2,IP1,IP0}




#endif /*_SETUP_H_*/
