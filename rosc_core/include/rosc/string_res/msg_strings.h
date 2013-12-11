
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
 * Length of the string array http_header_stdtext
 */
#define HTTP_HEADER_STDTEXT_LEN 	17

/**
 * Macro for inserting some standard values for http headers string numbers into any enum
 */
#define HTTP_HEADER_STDTEXT(ENUM) \
	 ENUM ## _VAL_MINUS_ONE, \
	 ENUM ## _VAL_ZERO, \
	 ENUM ## _VAL_ONE, \
	 ENUM ## _VAL_200_OK, \
	 ENUM ## _VAL_404_NOT_FOUND, \
	 ENUM ## _VAL_501_METHOD_NOT_IMPLEMENTED, \
	 ENUM ## _VAL_BASEHTTP_ROSC_NODELIB, \
	 ENUM ## _VAL_HTTP1_0, \
	 ENUM ## _VAL_HTTP1_1, \
	 ENUM ## _VAL_METHOD_POST, \
	 ENUM ## _VAL_XMLRPC_ROSC_NODELIB, \
	 ENUM ## _VAL_HTTP_URL_HEAD, \
	 ENUM ## _VAL_PUBLICATIONS, \
	 ENUM ## _VAL_SUBSCRIBTIONS, \
	 ENUM ## _VAL_TEXT_HTML, \
	 ENUM ## _VAL_ENCODING_PLAIN, \
	 ENUM ## _VAL_TEXT_XML

/**
 * This array contains the strings for some standard values for http headers
 */
extern const char *http_header_stdtext[];


/**
 * Length of the string array xmlrpc_stdtext
 */
#define XMLRPC_STDTEXT_LEN 	9

/**
 * Macro for inserting standard XMLRPC text pieces string numbers into any enum
 */
#define XMLRPC_STDTEXT(ENUM) \
	 ENUM ## _STDTXT_TCP_KEEPALIVE, \
	 ENUM ## _STDTXT_XML_DEF, \
	 ENUM ## _STDTXT_ERROR_CODE, \
	 ENUM ## _STDTXT_ROS, \
	 ENUM ## _STDTXT_TCPROS, \
	 ENUM ## _STDTXT_FAULTCODE, \
	 ENUM ## _STDTXT_FAULTSTRING, \
	 ENUM ## _STDTXT_HTTP_URL_HEAD, \
	 ENUM ## _STDTXT_ROSRPC_URL_HEAD

/**
 * This array contains the strings for standard XMLRPC text pieces
 */
extern const char *xmlrpc_stdtext[];


/**
 * Length of the string array xmlrpc_attribute_strings
 */
#define XMLRPC_ATTRIBUTE_STRINGS_LEN 	1

/**
 * Macro for inserting standard attributes for XMLRPC string numbers into any enum
 */
#define XMLRPC_ATTRIBUTE_STRINGS(ENUM) \
	 ENUM ## _ATTRIBUTE_VERSION

/**
 * This array contains the strings for standard attributes for XMLRPC
 */
extern const char *xmlrpc_attribute_strings[];


/**
 * Length of the string array xmlrpc_http_methods
 */
#define XMLRPC_HTTP_METHODS_LEN 	1

/**
 * Macro for inserting method strings string numbers into any enum
 */
#define XMLRPC_HTTP_METHODS(ENUM) \
	 ENUM ## _METHOD_POST

/**
 * This array contains the strings for method strings
 */
extern const char *xmlrpc_http_methods[];


/**
 * Length of the string array xmlrpc_tag_strings
 */
#define XMLRPC_TAG_STRINGS_LEN 	22

/**
 * Macro for inserting standard XMLRPC tags string numbers into any enum
 */
#define XMLRPC_TAG_STRINGS(ENUM) \
	 ENUM ## _TAG_ARRAY, \
	 ENUM ## _TAG_BODY, \
	 ENUM ## _TAG_BOOLEAN, \
	 ENUM ## _TAG_DATA, \
	 ENUM ## _TAG_DOUBLE, \
	 ENUM ## _TAG_FAULT, \
	 ENUM ## _TAG_HTML, \
	 ENUM ## _TAG_I4, \
	 ENUM ## _TAG_INT, \
	 ENUM ## _TAG_MEMBER, \
	 ENUM ## _TAG_METHODCALL, \
	 ENUM ## _TAG_METHODNAME, \
	 ENUM ## _TAG_METHODRESPONSE, \
	 ENUM ## _TAG_NAME, \
	 ENUM ## _TAG_P, \
	 ENUM ## _TAG_PARAM, \
	 ENUM ## _TAG_PARAMS, \
	 ENUM ## _TAG_STRING, \
	 ENUM ## _TAG_STRUCT, \
	 ENUM ## _TAG_TITLE_HEAD, \
	 ENUM ## _TAG_VALUE, \
	 ENUM ## _TAG_XMLDEF

/**
 * This array contains the strings for standard XMLRPC tags
 */
extern const char *xmlrpc_tag_strings[];


/**
 * Length of the string array xmlrpc_http_descriptors
 */
#define XMLRPC_HTTP_DESCRIPTORS_LEN 	8

/**
 * Macro for inserting standard http header descriptors string numbers into any enum
 */
#define XMLRPC_HTTP_DESCRIPTORS(ENUM) \
	 ENUM ## _DESCRIPTOR_ACCEPT_ENCODING, \
	 ENUM ## _DESCRIPTOR_CONTENT_ENCODING, \
	 ENUM ## _DESCRIPTOR_CONTENT_TYPE, \
	 ENUM ## _DESCRIPTOR_CONTENT_LENGTH, \
	 ENUM ## _DESCRIPTOR_DATE, \
	 ENUM ## _DESCRIPTOR_HOST, \
	 ENUM ## _DESCRIPTOR_SERVER, \
	 ENUM ## _DESCRIPTOR_USER_AGENT

/**
 * This array contains the strings for standard http header descriptors
 */
extern const char *xmlrpc_http_descriptors[];


/**
 * Length of the string array xmlrpc_slave_methodnames
 */
#define XMLRPC_SLAVE_METHODNAMES_LEN 	23

/**
 * Macro for inserting Strings for known methods of the slave api string numbers into any enum
 */
#define XMLRPC_SLAVE_METHODNAMES(ENUM) \
	 ENUM ## _METHODNAME_DELETEPARAM, \
	 ENUM ## _METHODNAME_GETBUSINFO, \
	 ENUM ## _METHODNAME_GETBUSSTATS, \
	 ENUM ## _METHODNAME_GETMASTERURI, \
	 ENUM ## _METHODNAME_GETPARAMNAMES, \
	 ENUM ## _METHODNAME_GETPID, \
	 ENUM ## _METHODNAME_GETPUBLICATIONS, \
	 ENUM ## _METHODNAME_GETSUBSCRIPTIONS, \
	 ENUM ## _METHODNAME_HASPARAM, \
	 ENUM ## _METHODNAME_PARAMUPDATE, \
	 ENUM ## _METHODNAME_PUBLISHERUPDATE, \
	 ENUM ## _METHODNAME_REGISTERPUBLISHER, \
	 ENUM ## _METHODNAME_REGISTERSERVICE, \
	 ENUM ## _METHODNAME_REGISTERSUBSCRIBER, \
	 ENUM ## _METHODNAME_REQUESTTOPIC, \
	 ENUM ## _METHODNAME_SEARCHPARAM, \
	 ENUM ## _METHODNAME_SETPARAM, \
	 ENUM ## _METHODNAME_SHUTDOWN, \
	 ENUM ## _METHODNAME_SUBSCRIBEPARAM, \
	 ENUM ## _METHODNAME_UNREGISTERPUBLISHER, \
	 ENUM ## _METHODNAME_UNREGISTERSERVICE, \
	 ENUM ## _METHODNAME_UNREGISTERSUBSCRIBER, \
	 ENUM ## _METHODNAME_UNSUBSCRIBEPARAM

/**
 * This array contains the strings for Strings for known methods of the slave api
 */
extern const char *xmlrpc_slave_methodnames[];


/**
 * Length of the string array ros_field_strings
 */
#define ROS_FIELD_STRINGS_LEN 	13

/**
 * Macro for inserting field id strings for the ros protocol string numbers into any enum
 */
#define ROS_FIELD_STRINGS(ENUM) \
	 ENUM ## _ROS_FIELD_CALLERID, \
	 ENUM ## _ROS_FIELD_ERROR, \
	 ENUM ## _ROS_FIELD_LATCHING, \
	 ENUM ## _ROS_FIELD_MD5SUM, \
	 ENUM ## _ROS_FIELD_MESSAGE_DEFINITION, \
	 ENUM ## _ROS_FIELD_PERSISTENT, \
	 ENUM ## _ROS_FIELD_PROBE, \
	 ENUM ## _ROS_FIELD_REQUEST_TYPE, \
	 ENUM ## _ROS_FIELD_RESPONSE_TYPE, \
	 ENUM ## _ROS_FIELD_SERVICE, \
	 ENUM ## _ROS_FIELD_TCP_NODELAY, \
	 ENUM ## _ROS_FIELD_TOPIC, \
	 ENUM ## _ROS_FIELD_TYPE

/**
 * This array contains the strings for field id strings for the ros protocol
 */
extern const char *ros_field_strings[];


/**
 * Length of the string array xmlrpc_url_scheme_string
 */
#define XMLRPC_URL_SCHEME_STRING_LEN 	2

/**
 * Macro for inserting strings for xmlrpc url schemes string numbers into any enum
 */
#define XMLRPC_URL_SCHEME_STRING(ENUM) \
	 ENUM ## _URL_SCHEMES_HTTP, \
	 ENUM ## _URL_SCHEMES_ROSRPC

/**
 * This array contains the strings for strings for xmlrpc url schemes
 */
extern const char *xmlrpc_url_scheme_string[];


/**
 * Length of the string array xmlrpc_http_actions
 */
#define XMLRPC_HTTP_ACTIONS_LEN 	2

/**
 * Macro for inserting contains available actions for http requests string numbers into any enum
 */
#define XMLRPC_HTTP_ACTIONS(ENUM) \
	 ENUM ## _ACTION_ROOT, \
	 ENUM ## _ACTION_RPC2

/**
 * This array contains the strings for contains available actions for http requests
 */
extern const char *xmlrpc_http_actions[];


/**
 * Length of the string array xmlrpc_xml_boolean_strings
 */
#define XMLRPC_XML_BOOLEAN_STRINGS_LEN 	4

/**
 * Macro for inserting possible strings for representing boolean in xml rpc string numbers into any enum
 */
#define XMLRPC_XML_BOOLEAN_STRINGS(ENUM) \
	 ENUM ## _BOOLEAN_ZERO, \
	 ENUM ## _BOOLEAN_ONE, \
	 ENUM ## _BOOLEAN_FALSE, \
	 ENUM ## _BOOLEAN_TRUE

/**
 * This array contains the strings for possible strings for representing boolean in xml rpc
 */
extern const char *xmlrpc_xml_boolean_strings[];


#endif /* MSG_STRINGS_H_ */