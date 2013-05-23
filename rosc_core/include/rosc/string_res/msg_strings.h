
/*
 *    WARNING WARNING WARNING WARNING
 *     FILE GENERATED AUTOMATICALLY!
 *    WARNING WARNING WARNING WARNING
 *     TO CHANGE ANYTHING GO INTO
 *             strHCgen.py
 *
 *    Copyright (c) 2013, Synapticon GmbH
 *    All rights reserved.
 *
 *    Redistribution and use in source and binary forms, with or without
 *    modification, are permitted provided that the following conditions are met:
 *
 *    1. Redistributions of source code must retain the above copyright notice, this
 *       list of conditions and the following disclaimer.
 *    2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *    ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *    ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *    The views and conclusions contained in the software and documentation are those
 *    of the authors and should not be interpreted as representing official policies,
 *    either expressed or implied, of the FreeBSD Project.
 */           
#ifndef MSG_STRINGS_H_
#define MSG_STRINGS_H_ 


/**
 * Length of the string array rpc_xml_attribute_strings
 */
#define RPC_XML_ATTRIBUTE_STRINGS_LEN 	1

/**
 * Macro for inserting standard attributes for XMLRPC string numbers into any enum
 */
#define __RPC_XML_ATTRIBUTE_STRINGS(ENUM) \
	 ENUM ## _ATTRIBUTE_VERSION

/**
 * This array contains the strings for standard attributes for XMLRPC
 */
extern const char *rpc_xml_attribute_strings[];


/**
 * Length of the string array http_header_stdtext
 */
#define HTTP_HEADER_STDTEXT_LEN 	8

/**
 * Macro for inserting some standard values for http headers string numbers into any enum
 */
#define __HTTP_HEADER_STDTEXT(ENUM) \
	 ENUM ## _VAL_BASEHTTP_ROSC_NODELIB, \
	 ENUM ## _VAL_HTTP1_0, \
	 ENUM ## _VAL_HTTP1_1, \
	 ENUM ## _VAL_OK, \
	 ENUM ## _VAL_METHOD_POST, \
	 ENUM ## _VAL_XMLRPC_ROSC_NODELIB, \
	 ENUM ## _VAL_HTTP_URL_HEAD, \
	 ENUM ## _VAL_TEXT_XML

/**
 * This array contains the strings for some standard values for http headers
 */
extern const char *http_header_stdtext[];


/**
 * Length of the string array rpc_xml_slave_methodnames
 */
#define RPC_XML_SLAVE_METHODNAMES_LEN 	10

/**
 * Macro for inserting Strings for known methods of the slave api string numbers into any enum
 */
#define __RPC_XML_SLAVE_METHODNAMES(ENUM) \
	 ENUM ## _METHOD_NAME_GETBUSINFO, \
	 ENUM ## _METHOD_NAME_GETBUSSTATS, \
	 ENUM ## _METHOD_NAME_GETMASTERURI, \
	 ENUM ## _METHOD_NAME_GETPID, \
	 ENUM ## _METHOD_NAME_GETPUBLICATIONS, \
	 ENUM ## _METHOD_NAME_GETSUBSCRIPTIONS, \
	 ENUM ## _METHOD_NAME_PARAMUPDATE, \
	 ENUM ## _METHOD_NAME_PUBLISHERUPDATE, \
	 ENUM ## _METHOD_NAME_REQUESTTOPIC, \
	 ENUM ## _METHOD_NAME_SHUTDOWN

/**
 * This array contains the strings for Strings for known methods of the slave api
 */
extern const char *rpc_xml_slave_methodnames[];


/**
 * Length of the string array xmlrpc_xml_boolean_strings
 */
#define XMLRPC_XML_BOOLEAN_STRINGS_LEN 	4

/**
 * Macro for inserting possible strings for representing boolean in xml rpc string numbers into any enum
 */
#define __XMLRPC_XML_BOOLEAN_STRINGS(ENUM) \
	 ENUM ## _BOOLEAN_ZERO, \
	 ENUM ## _BOOLEAN_ONE, \
	 ENUM ## _BOOLEAN_FALSE, \
	 ENUM ## _BOOLEAN_TRUE

/**
 * This array contains the strings for possible strings for representing boolean in xml rpc
 */
extern const char *xmlrpc_xml_boolean_strings[];


/**
 * Length of the string array xmlrpc_http_methods
 */
#define XMLRPC_HTTP_METHODS_LEN 	2

/**
 * Macro for inserting method strings string numbers into any enum
 */
#define __XMLRPC_HTTP_METHODS(ENUM) \
	 ENUM ## _METHOD_GET, \
	 ENUM ## _METHOD_POST

/**
 * This array contains the strings for method strings
 */
extern const char *xmlrpc_http_methods[];


/**
 * Length of the string array xmlrpc_http_descriptors
 */
#define XMLRPC_HTTP_DESCRIPTORS_LEN 	8

/**
 * Macro for inserting standard http header descriptors string numbers into any enum
 */
#define __XMLRPC_HTTP_DESCRIPTORS(ENUM) \
	 ENUM ## _DESC_ACCEPT_ENCODING, \
	 ENUM ## _DESC_CONTENT_ENCODING, \
	 ENUM ## _DESC_CONTENT_TYPE, \
	 ENUM ## _DESC_CONTENT_LENGTH, \
	 ENUM ## _DESC_DATE, \
	 ENUM ## _DESC_HOST, \
	 ENUM ## _DESC_SERVER, \
	 ENUM ## _DESC_USER_AGENT

/**
 * This array contains the strings for standard http header descriptors
 */
extern const char *xmlrpc_http_descriptors[];


/**
 * Length of the string array ros_field_strings
 */
#define ROS_FIELD_STRINGS_LEN 	10

/**
 * Macro for inserting field id strings for the ros protocol string numbers into any enum
 */
#define __ROS_FIELD_STRINGS(ENUM) \
	 ENUM ## _ROS_FIELD_CALLERID, \
	 ENUM ## _ROS_FIELD_ERROR, \
	 ENUM ## _ROS_FIELD_LATCHING, \
	 ENUM ## _ROS_FIELD_MD5SUM, \
	 ENUM ## _ROS_FIELD_MESSAGE_DEFINITION, \
	 ENUM ## _ROS_FIELD_PERSISTENT, \
	 ENUM ## _ROS_FIELD_SERVICE, \
	 ENUM ## _ROS_FIELD_TCP_NODELAY, \
	 ENUM ## _ROS_FIELD_TOPIC, \
	 ENUM ## _ROS_FIELD_TYPE

/**
 * This array contains the strings for field id strings for the ros protocol
 */
extern const char *ros_field_strings[];


/**
 * Length of the string array rpc_xml_tag_strings
 */
#define RPC_XML_TAG_STRINGS_LEN 	18

/**
 * Macro for inserting standard XMLRPC tags string numbers into any enum
 */
#define __RPC_XML_TAG_STRINGS(ENUM) \
	 ENUM ## _TAG_COMMENT, \
	 ENUM ## _TAG_CDATA, \
	 ENUM ## _TAG_ARRAY, \
	 ENUM ## _TAG_BOOLEAN, \
	 ENUM ## _TAG_DATA, \
	 ENUM ## _TAG_FAULT, \
	 ENUM ## _TAG_I4, \
	 ENUM ## _TAG_INT, \
	 ENUM ## _TAG_MEMBER, \
	 ENUM ## _TAG_METHODCALL, \
	 ENUM ## _TAG_METHODNAME, \
	 ENUM ## _TAG_METHODRESPONSE, \
	 ENUM ## _TAG_PARAM, \
	 ENUM ## _TAG_PARAMS, \
	 ENUM ## _TAG_STRING, \
	 ENUM ## _TAG_STRUCT, \
	 ENUM ## _TAG_VALUE, \
	 ENUM ## _TAG_XMLDEF

/**
 * This array contains the strings for standard XMLRPC tags
 */
extern const char *rpc_xml_tag_strings[];


/**
 * Length of the string array rpc_xml_stdtext
 */
#define RPC_XML_STDTEXT_LEN 	14

/**
 * Macro for inserting standard XMLRPC text pieces string numbers into any enum
 */
#define __RPC_XML_STDTEXT(ENUM) \
	 ENUM ## _STDTXT_TCP_KEEPALIVE, \
	 ENUM ## _STDTXT_XML_DEF, \
	 ENUM ## _STDTXT_ERROR_CODE, \
	 ENUM ## _STDTXT_TCPROS, \
	 ENUM ## _STDTXT_HASPARAM, \
	 ENUM ## _STDTXT_HTTP_URL_HEAD, \
	 ENUM ## _STDTXT_REGISTERPUBLISHER, \
	 ENUM ## _STDTXT_REGISTERSERVICE, \
	 ENUM ## _STDTXT_REGISTERSUBSCRIBER, \
	 ENUM ## _STDTXT_REQUESTTOPIC, \
	 ENUM ## _STDTXT_ROSRPC_URL_HEAD, \
	 ENUM ## _STDTXT_UNREGISTERPUBLISHER, \
	 ENUM ## _STDTXT_UNREGISTERSERVICE, \
	 ENUM ## _STDTXT_UNREGISTERSUBSCRIBER

/**
 * This array contains the strings for standard XMLRPC text pieces
 */
extern const char *rpc_xml_stdtext[];


/**
 * Length of the string array xmlrpc_http_actions
 */
#define XMLRPC_HTTP_ACTIONS_LEN 	2

/**
 * Macro for inserting contains available actions for http requests string numbers into any enum
 */
#define __XMLRPC_HTTP_ACTIONS(ENUM) \
	 ENUM ## _ACTION_ROOT, \
	 ENUM ## _ACTION_TEST

/**
 * This array contains the strings for contains available actions for http requests
 */
extern const char *xmlrpc_http_actions[];


#endif /* MSG_STRINGS_H_ */