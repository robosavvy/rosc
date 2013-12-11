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
 *  sebs_parse_parseurl.h created by Christian Holl
 */

#ifndef SEBS_PARSE_PARSEURL_H_
#define SEBS_PARSE_PARSEURL_H_

#include <rosc/system/setup.h>
#include <rosc/sebs_parse_fw/sebs_parser_frame.h>
#include <rosc/sebs_parse_fw/std_modules/sebs_parse_seekstring.h>
#include <rosc/sebs_parse_fw/std_modules/sebs_parse_numberparse.h>
#include <rosc/sebs_parse_fw/std_modules/sebs_parse_copy2buffer.h>



#define SEBS_PARSE_URL_INIT(PARSER_DATA, DATA_STORAGE, URL_LENGTH,SCHEME_LIST,SCHEME_LIST_LEN)\
		PARSER_DATA->next_parser.parser_function=(sebs_parse_function_t) &sebs_parse_url;\
		PARSER_DATA->next_parser.parser_data=(void *)(&DATA_STORAGE);\
		DATA_STORAGE.scheme_list=SCHEME_LIST;\
		DATA_STORAGE.scheme_list_len=SCHEME_LIST_LEN;\
		DATA_STORAGE.url_length=URL_LENGTH;\
		return (SEBS_PARSE_RETURN_INIT);

/**
 *	Contains the result states parse url submode
 */
typedef enum
{
	SEBS_PARSE_URL_RESULT_ERROR_URL_SCHEME, //!< means that there was no or an unknown url scheme given
	SEBS_PARSE_URL_RESULT_ERROR_URL_MALFORMED, //!< means that the is not correct
	SEBS_PARSE_URL_RESULT_ERROR_HOSTNAME_TOO_LONG, //!< means that the buffers length was insufficient
	SEBS_PARSE_URL_RESULT_HOSTNAME,    //!< SEBS_PARSE_URL__MATCH_HOSTNAME - means that the current content inside the buffer will only match a hostname
	SEBS_PARSE_URL_RESULT_IPV4,        //!< SEBS_PARSE_URL__MATCH_IPv4 - means that the content is a IPv4 address
	SEBS_PARSE_URL_RESULT_IPV6,   	 //!< SEBS_PARSE_URL__MATCH_IPv6 - means that the content is a IPv6 address
	SEBS_PARSE_URL_RESULT_IPv6_RESOLV, //!< SEBS_PARSE_URL__MATCH_IPv6_RESOLV - means that the content is a IPv6 address with a IPv4 network resolving addition
}sebs_parse_url_result_t;


typedef enum
{
	SEBS_PARSE_URL_STATE_START,
	SEBS_PARSE_URL_STATE_CHECK_SCHEME,
	SEBS_PARSE_URL_STATE_CHECK_SCHEME_END,
	SEBS_PARSE_URL_STATE_ANALYSE_URI_TYPE,
	SEBS_PARSE_URL_STATE_IPV6_GET_BLOCK,
	SEBS_PARSE_URL_STATE_PARSE_PORT,
	SEBS_PARSE_URL_STATE_CHECK_PORT,
	SEBS_PARSE_URL_STATE_PARSE_RESOLV,
	SEBS_PARSE_URL_STATE_PARSE_IPV6,
	SEBS_PARSE_URL_STATE_CHECK_IPV6,
	SEBS_PARSE_URL_STATE_PARSE_IPV6_RESOLV,
	SEBS_PARSE_URL_STATE_PARSE_IPV4_HOSTNAME,
	SEBS_PARSE_URL_STATE_PARSE_HOSTNAME,
	SEBS_PARSE_URL_STATE_PARSE_WAIT_DASH
}sebs_parse_url_state_t;

/**
 * This struct stores the data for the parseurl submode
 */
typedef struct
{
	//Storage for previous next variable
	sebs_parser_call_t caller; //!< this will store the original function when calling seekstring or other functions
	bool called_by_handler; //!< this will store the original value of the return to handler function

	//General
	sebs_parse_url_state_t state; //!contains the current state of the xml parser


	//Scheme
	const char **scheme_list; //!< The scheme list for the url scheme (http, rostcp)
	uint16_t scheme_list_len; //!< The scheme list length
	uint16_t url_scheme; //!<contains the urlscheme http, mailto ...
	uint16_t url_length;//!<contains the length of the url when it is known before (usage: binary protocols like rostcp)


	//Hostname
	char hostname_buf[__HOSTNAME_BUFFER_LEN__+20]; //!< The text form of the hostname/IP
	uint16_t cur_pos; //!< curLen The current size of the copied chars

	//IP Address
	uint8_t cur_fig; //!current figure of the current ip address part
	uint8_t dot_cnt; //!variable for counting the dots inside ipv4 and double dots in ipv6 addresses

	union
	{
		uint16_t v6[8];//!< storage for an IPv6 address
		uint8_t v4[4]; //!< storage for an IPv4 address or the resolving end of IPv6
	}ip;

	//Port
	uint16_t port; //!< storage for a port number

	//Result
	sebs_parse_url_result_t result; //!< what specifies what kind of address is given

	//Subfunctions
	union
	{
		sebs_parse_numberparse_data_t numberparse;
		sebs_parse_seekstring_data_t seekstring;
	};
}sebs_parse_url_data_t;



/**
 * This function parses a URL from a stream, it requires a URL with a scheme specifier!
 * @param pdata The parsers data structure
 */
sebs_parse_return_t sebs_parse_url(sebs_parser_data_t *pdata);
#endif /* SEBS_PARSE_PARSEURL_H_ */
