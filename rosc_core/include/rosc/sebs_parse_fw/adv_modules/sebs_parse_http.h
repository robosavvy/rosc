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
 *  sebs_parse_http.h created by Christian Holl
 */

#ifndef SEBS_PARSE_HTTP_H_
#define SEBS_PARSE_HTTP_H_

#include <rosc/system/spec.h>
#include <rosc/string_res/msg_strings.h>
#include <rosc/sebs_parse_fw/std_modules/sebs_parse_seekstring.h>
#include <rosc/sebs_parse_fw/std_modules/sebs_parse_numberparse.h>
#include <rosc/sebs_parse_fw/sebs_parser_frame.h>

#define SEBS_PARSE_HTTP_REQUEST_INIT SEBS_PARSE_HTTP_STATE_REQUEST_METHOD
#define SEBS_PARSE_HTTP_RESPONSE_INIT SEBS_PARSE_HTTP_STATE_RESPONSE_HTTP_VER


///@todo document SEBS_PARSE_HTTP_INIT
#define SEBS_PARSE_HTTP_INIT(PARSER_DATA, DATA_STORAGE, INIT_STATE, DESCRIPTORS, DESCRIPTORS_LEN, ACTIONS, ACTIONS_LEN, METHODS, METHODS_LEN)\
		PARSER_DATA->next_parser.parser_function=(sebs_parse_function_t) &sebs_parse_http;\
		PARSER_DATA->next_parser.parser_data=(void *)(&DATA_STORAGE);\
		DATA_STORAGE.state=INIT_STATE;\
		DATA_STORAGE.descriptors=DESCRIPTORS;\
		DATA_STORAGE.descriptors_len=DESCRIPTORS_LEN;\
		DATA_STORAGE.actions=ACTIONS;\
		DATA_STORAGE.actions_len=ACTIONS_LEN;\
		DATA_STORAGE.methods=METHODS;\
		DATA_STORAGE.methods_len=METHODS_LEN;\
		return (SEBS_PARSE_RETURN_INIT_ADV)


/**
 * Maximum allowed header length (can be unlimited, technically)
 */
#define SEBS_PARSE_HTTP_MAX_LEN 1024

/**
 * Contains events for the handler for it to know what currently happened
 */
typedef enum
{
	SEBS_PARSE_HTTP_EVENT_ERROR_CONTENT_LENGTH_TOO_LONG=-100,//!< means the content exceeds the specified max lenght
	SEBS_PARSE_HTTP_EVENT_ERROR_CONTENT_LENGTH,              //!< means that the content length is now available
	SEBS_PARSE_HTTP_EVENT_ERROR_ACTION_NOT_FOUND,                   //!< means that the target/action/url was not found in the string array (code: 404)
	SEBS_PARSE_HTTP_EVENT_ERROR_VERSION_NOT_SUPPORTED,       //!< means that the HTTP version is not supported (code: 505)
	SEBS_PARSE_HTTP_EVENT_ERROR_BAD_REQUEST,                 //!< means that something is wrong inside the http header
	SEBS_PARSE_HTTP_EVENT_ERROR_METHOD_NOT_ALLOWED,          //!< means that the given method string did not match any of those in the string array
	SEBS_PARSE_HTTP_EVENT_ERROR_CONTENT_ENCODING,            //!< means that the content encoding is set (which is not supported by a stream parser -> cause of compression...)
	SEBS_PARSE_HTTP_EVENT_ERROR_BAD_RESPONSE,                //!< means that something is wrong in the received response

	SEBS_PARSE_HTTP_EVENT_HEADER_END,						 //!< means that the function reached the end
	SEBS_PARSE_HTTP_EVENT_CONTENT_TYPE,                		 //!< means that a content type was found
	SEBS_PARSE_HTTP_EVENT_METHOD_PARSED,                     //!< means that the method string of the http header was parsed
	SEBS_PARSE_HTTP_EVENT_ACTION_PARSED,                     //!< means that the action (url, uri string) was parsed

	SEBS_PARSE_HTTP_EVENT_HEADER_CONTENT,	                 //!< means that the http parser reached content inside a http field
	SEBS_PARSE_HTTP_EVENT_RESPONSE_CODE,                     //!< means that the response code is now available (client)

}sebs_parse_http_event_t;

/**
 * Definition of the main states of the http parser.
 */
typedef enum
{
	/*Special states for parsing a request (server)*/
	SEBS_PARSE_HTTP_STATE_REQUEST_METHOD,          //!< in this state the parser expects the method string of a request
	SEBS_PARSE_HTTP_STATE_REQUEST_ACTION,          //!< in this state the parser expects the action string of a request
	SEBS_PARSE_HTTP_STATE_REQUEST_HTTP_VER,        //!< in this state the parser expects the http version of a request

	/*Special states for parsing a response (client)*/
	SEBS_PARSE_HTTP_STATE_RESPONSE_HTTP_VER,       //!< in this state the parser expects the http version of a response
	SEBS_PARSE_HTTP_STATE_RESPONSE_CODE,           //!< in this state the parser expects the http response code
	SEBS_PARSE_HTTP_STATE_RESPONSE_STRING,         //!< in this state the parser waits for a line feed

	/*Common states*/
	SEBS_PARSE_HTTP_STATE_HEADLINE_WAIT_END,       //!< in this state the parser waits for a line feed at the end of a requests headline
	SEBS_PARSE_HTTP_STATE_DESCRIPTOR_OR_HEADER_END,//!< in this state the parser expects the start of a new field or a linefeed for the header end
	SEBS_PARSE_HTTP_STATE_DESCRIPTOR_FIELD_SEPARATOR,    //!< in this state the parser waits for a double point and only accepts spaces
	SEBS_PARSE_HTTP_STATE_FIELD,                   //!< this state is reached after the parser finds a double point after the field name
	SEBS_PARSE_HTTP_STATE_FIELD_CONTENT,           //!< this state is entered by the parser if it finds content inside a header field
}sebs_parse_http_state_t;

/**
 * Definition of
 */
typedef enum
{
	SEBS_PARSE_HTTP_SUBSTATE_STATE_NONE,             //!< SEBS_PARSE_HTTP_SUBSTATE_STATE_NONE
	SEBS_PARSE_HTTP_SUBSTATE_CHECK_METHOD,           //!< SEBS_PARSE_HTTP_SUBSTATE_CHECK_METHOD
	SEBS_PARSE_HTTP_SUBSTATE_CHECK_ACTION,           //!< SEBS_PARSE_HTTP_SUBSTATE_CHECK_ACTION
	SEBS_PARSE_HTTP_SUBSTATE_CHECK_REQUEST_HTTP_VER, //!< SEBS_PARSE_HTTP_SUBSTATE_CHECK_REQUEST_HTTP_VER
	SEBS_PARSE_HTTP_SUBSTATE_CHECK_DESCRIPTOR_ID,    //!< SEBS_PARSE_HTTP_SUBSTATE_CHECK_DESCRIPTOR_ID
	SEBS_PARSE_HTTP_SUBSTATE_CHECK_RESPONSE_HTTP_VER,//!< SEBS_PARSE_HTTP_SUBSTATE_CHECK_RESPONSE_HTTP_VER
	SEBS_PARSE_HTTP_SUBSTATE_CHECK_RESPONSE_CODE,    //!< SEBS_PARSE_HTTP_SUBSTATE_CHECK_RESPONSE_CODE
	SEBS_PARSE_HTTP_SUBSTATE_CHECK_RESPONSE_STATE,   //!< SEBS_PARSE_HTTP_SUBSTATE_CHECK_RESPONSE_STATE
}sebs_parse_http_substate_t;


typedef enum
{
	HTTP_HEADER_STDTEXT(HTTP)
}xmlrpc_server_stdtxt_t;



typedef struct
{
	/**
	 * Pointer for access to parser frame
	 */

	sebs_parse_http_state_t state; //!< state contains the current state of the http parser
	sebs_parse_http_substate_t substate; //!< contains the substate of the http parser (analyzing results of submodes)
	uint16_t descriptor; //!< descriptor contains the current known descriptor
	int32_t content_length;

	const char **methods; //!< contains available methods (POST,GET ...) (sorted!)
	uint16_t	 methods_len; //!< amount of method strings

	const char **descriptors; //!< contains all descriptors for interesting field (sorted!)
	uint16_t	 descriptors_len; //!< amount of descriptor strings

	const char **actions; //!< contains all available actions (bspw. / /index.html) (sorted!)
	uint16_t	 actions_len; //!< amount of target strings

	union
	{
		sebs_parse_numberparse_data_t numberparse;
		sebs_parse_seekstring_data_t seekstring;
	};
}sebs_parse_http_data_t;


sebs_parse_return_t sebs_parse_http(sebs_parser_data_t* pdata);



#endif /* SEBS_PARSE_HTTP_H_ */
