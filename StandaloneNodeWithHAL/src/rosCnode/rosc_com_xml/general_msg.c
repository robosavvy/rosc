/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2013, Synapticon GmbH
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Synapticon GmbH nor the names of its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * general_msg.c
 *
 *  Created on: 19.01.2013
 *      Author: Christian Holl
 */

#include "general_msg.h"
#include "../rosc_com_base/eth.h"



http_head_gen_command xmlrpc_http_std_header[]=
{
	HTTP_HEADER_GEN_DESC_USER_AGENT,
		HTTP_HEADER_GEN_VAL_XMLRPC_ROSC_NODELIB,

	HTTP_HEADER_GEN_DESC_HOST,
		HTTP_HEADER_GEN_VAL_HTTP_URL_HEAD,
		HTTP_HEADER_GEN_VAL_UINT_NUMBER,
		HTTP_HEADER_GEN_SINGLE_CHAR+'.',
		HTTP_HEADER_GEN_VAL_UINT_NUMBER,
		HTTP_HEADER_GEN_SINGLE_CHAR+'.',
		HTTP_HEADER_GEN_VAL_UINT_NUMBER,
		HTTP_HEADER_GEN_SINGLE_CHAR+'.',
		HTTP_HEADER_GEN_VAL_UINT_NUMBER,
		HTTP_HEADER_GEN_SINGLE_CHAR+':',
		HTTP_HEADER_GEN_VAL_UINT_NUMBER +11311,

	HTTP_HEADER_GEN_DESC_CONTENT_TYPE,
		HTTP_HEADER_GEN_VAL_TEXT_XML,

	HTTP_HEADER_GEN_END
};


ros_rpc_gen_command xmlrpc_master_keepalive_msg[]=
{
RPC_STDTXT_XML_DEF,
RPC_TAG_METHODCALL,
	RPC_TAG_METHODNAME,
		RPC_STDTXT_HASPARAM,
	RPC_CT RPC_TAG_METHODNAME,
	RPC_TAG_PARAMS,
		RPC_TAG_PARAM,
			RPC_STDTXT_TCP_KEEPALIVE,
		RPC_CT RPC_TAG_PARAM ,
	RPC_CT RPC_TAG_PARAMS,
RPC_CT RPC_TAG_METHODCALL,
RPC_GENERATOR_FINISH
};

