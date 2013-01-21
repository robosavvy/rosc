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
 * general_msg.h
 *
 *  Created on: 19.01.2013
 *      Author: Christian Holl
 */

#ifndef GENERAL_MSG_H_
#define GENERAL_MSG_H_

//TODO MOVE TO MAKEFILE STUFF....
#define __ROS_C_VERSION__MAJOR__ 0
#define __ROS_C_VERSION__MINOR__ 1


#include <rosc/string_res/msg_strings.h>
#include <rosc/com_xml/msg_gen.h>

/**
 * This macro is for initializing a generator command array for generating the http header
 */
#define XMLRPC_HTTP_HEADER_REQUEST(IP0,IP1,IP2,IP3,PORT)\
	{\
		HTTP_HEADER_GEN_VAL_METHOD_POST,\
		HTTP_HEADER_GEN_DESC_USER_AGENT,\
			HTTP_HEADER_GEN_VAL_XMLRPC_ROSC_NODELIB,\
			HTTP_HEADER_GEN_SINGLE_CHAR+' ',\
			HTTP_HEADER_GEN_SINGLE_CHAR+'V',\
			HTTP_HEADER_GEN_VAL_UINT_NUMBER+__ROS_C_VERSION__MAJOR__,\
			HTTP_HEADER_GEN_SINGLE_CHAR+'.',\
			HTTP_HEADER_GEN_VAL_UINT_NUMBER+__ROS_C_VERSION__MINOR__,\
			\
		HTTP_HEADER_GEN_DESC_HOST,\
			HTTP_HEADER_GEN_VAL_UINT_NUMBER+IP0,\
			HTTP_HEADER_GEN_SINGLE_CHAR+'.',\
			HTTP_HEADER_GEN_VAL_UINT_NUMBER+IP1,\
			HTTP_HEADER_GEN_SINGLE_CHAR+'.',\
			HTTP_HEADER_GEN_VAL_UINT_NUMBER+IP2,\
			HTTP_HEADER_GEN_SINGLE_CHAR+'.',\
			HTTP_HEADER_GEN_VAL_UINT_NUMBER+IP3,\
			HTTP_HEADER_GEN_SINGLE_CHAR+':',\
			HTTP_HEADER_GEN_VAL_UINT_NUMBER+PORT,\
			\
		HTTP_HEADER_GEN_DESC_ACCEPTED_ENCODING,\
			HTTP_HEADER_GEN_VAL_TEXT_XML,\
			\
		HTTP_HEADER_GEN_DESC_CONTENT_TYPE,\
			HTTP_HEADER_GEN_VAL_TEXT_XML,\
			\
		HTTP_HEADER_GEN_END\
	}

/**
 * The XML message defined by this macro sets keepalive option at the master
 */
#define XMLRPC_MSG_KEEPALIVE()\
{\
	RPC_STDTXT_XML_DEF,\
	RPC_TAG_METHODCALL,\
		RPC_TAG_METHODNAME,\
			RPC_STDTXT_HASPARAM,\
		RPC_CT RPC_TAG_METHODNAME,\
		RPC_TAG_PARAMS,\
			RPC_TAG_PARAM,\
				RPC_STDTXT_TCP_KEEPALIVE,\
			RPC_CT RPC_TAG_PARAM ,\
		RPC_CT RPC_TAG_PARAMS,\
	RPC_CT RPC_TAG_METHODCALL,\
	RPC_GENERATOR_FINISH\
}



/**
 * This initializes a custom string array to be used with XMLRPC_MSG_REGISTERPUBLISHER
 * and XMLRPC_MSG_REGISTERSUBSCRIBER
 */
#define XMLRPC_MSGSTR_REGISTERPUBSUB_CUSTOM_STRING_ARRAY(TOPICNAME,MESSAGETYPE)\
{\
	 node_name,\
	 TOPICNAME,\
	 MESSAGETYPE,\
}



/**
 * This will initialize a rpc command array for the "registerPublisher" message.
 * It requires to use the XMLRPC_MSGSTR_REGISTERPUBSUB_CUSTOM_STRING_ARRAY for the string array
 */
#define XMLRPC_MSG_REGISTERPUBLISHER(PORT)\
{\
	RPC_STDTXT_XML_DEF,\
	\
	RPC_TAG_METHODCALL,\
		RPC_TAG_METHODNAME,\
			RPC_STDTXT_REGISTERPUBLISHER,\
		RPC_CT RPC_TAG_METHODNAME,\
		\
		RPC_TAG_PARAMS,\
		\
			RPC_TAG_PARAM,\
				RPC_TAG_VALUE,\
					RPC_CUSTOM_TEXT+0,	/*Node Name*/\
				RPC_CT RPC_TAG_VALUE,\
			RPC_CT RPC_TAG_PARAM ,\
			\
			RPC_TAG_PARAM,\
				RPC_TAG_VALUE,\
					RPC_CUSTOM_TEXT+1,	/*Topic Name*/\
				RPC_CT RPC_TAG_VALUE,\
			\
			RPC_CT RPC_TAG_PARAM ,\
			\
			RPC_TAG_PARAM,\
				RPC_TAG_VALUE,\
					RPC_CUSTOM_TEXT+2,	/*Message Type*/\
				RPC_CT RPC_TAG_VALUE,\
			RPC_CT RPC_TAG_PARAM ,\
			\
			RPC_TAG_PARAM,\
				RPC_TAG_VALUE,\
					RPC_STDTXT_HTTP_URL_HEAD,\
					RPC_UINT_NUMBER +master_ip[0],\
					RPC_SINGLE_CHAR+'.',\
					RPC_UINT_NUMBER +master_ip[1],\
					RPC_SINGLE_CHAR+'.',\
					RPC_UINT_NUMBER +master_ip[2],\
					RPC_SINGLE_CHAR+'.',\
					RPC_UINT_NUMBER +master_ip[3],\
					RPC_SINGLE_CHAR+':',\
					RPC_UINT_NUMBER +PORT,\
				RPC_CT RPC_TAG_VALUE,\
			RPC_CT RPC_TAG_PARAM ,\
			\
		RPC_CT RPC_TAG_PARAMS,\
	RPC_CT RPC_TAG_METHODCALL,\
	RPC_GENERATOR_FINISH\
}


/**
 * This will initialize a rpc command array for the "registerSubscriber" message.
 * It requires to use the XMLRPC_MSGSTR_REGISTERPUBSUB_CUSTOM_STRING_ARRAY for the string array
 */
#define XMLRPC_MSG_REGISTERSUBSCRIBER(PORT)\
{\
	RPC_STDTXT_XML_DEF,\
	\
	RPC_TAG_METHODCALL,\
		RPC_TAG_METHODNAME,\
			RPC_STDTXT_REGISTERSUBSCRIBER,\
		RPC_CT RPC_TAG_METHODNAME,\
		\
		RPC_TAG_PARAMS,\
		\
			RPC_TAG_PARAM,\
				RPC_TAG_VALUE,\
					RPC_CUSTOM_TEXT+0,	/*Node Name*/\
				RPC_CT RPC_TAG_VALUE,\
			RPC_CT RPC_TAG_PARAM ,\
			\
			RPC_TAG_PARAM,\
				RPC_TAG_VALUE,\
					RPC_CUSTOM_TEXT+1,	/*Topic Name*/\
				RPC_CT RPC_TAG_VALUE,\
			\
			RPC_CT RPC_TAG_PARAM ,\
			\
			RPC_TAG_PARAM,\
				RPC_TAG_VALUE,\
					RPC_CUSTOM_TEXT+2,	/*Message Type*/\
				RPC_CT RPC_TAG_VALUE,\
			RPC_CT RPC_TAG_PARAM ,\
			\
			RPC_TAG_PARAM,\
				RPC_TAG_VALUE,\
					RPC_STDTXT_HTTP_URL_HEAD,\
					RPC_UINT_NUMBER +master_ip[0],\
					RPC_SINGLE_CHAR+'.',\
					RPC_UINT_NUMBER +master_ip[1],\
					RPC_SINGLE_CHAR+'.',\
					RPC_UINT_NUMBER +master_ip[2],\
					RPC_SINGLE_CHAR+'.',\
					RPC_UINT_NUMBER +master_ip[3],\
					RPC_SINGLE_CHAR+':',\
					RPC_UINT_NUMBER +PORT,\
				RPC_CT RPC_TAG_VALUE,\
			RPC_CT RPC_TAG_PARAM ,\
			\
		RPC_CT RPC_TAG_PARAMS,\
	RPC_CT RPC_TAG_METHODCALL,\
	RPC_GENERATOR_FINISH\
}


#endif /* GENERAL_MSG_H_ */
