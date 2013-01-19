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
 * msg_parse.h
 *
 * Created on: 11.09.2012
 *     Author: Christian Holl
 */

#ifndef MSG_PARSE_H_
#define MSG_PARSE_H_

#include <stdbool.h>
#include "../rosc_string_res/msg_strings.h"

#define TAG_BUFFER_SIZE 25


typedef enum
{
	__PARSE_SEPARATORS(PARSE_METHOD)
}parseSeparators_t;


typedef enum
{
	HTTP_METHOD_PARSE_UNKNOWN, //!< Unknown/Unsupported Method
	HTTP_METHOD_PARSE_INVALID, //!< Invalid Method string

	__HTTP_METHODS(HTTP_METHOD_PARS),	//!< Inserts available methods into the enum

	 __HTTP_HEADER_DESCRIPTORS(HTTP_HEADER_PARSE), //!< Inserts available header descriptors into the enum


	__XML_PARSE_EVENT_NO_EVENT, //!< This is just for internal processing and should never occur

	XML_PARSE_EVENT_DOCUMENT_START, //!< Initialize event for result computing function

	__XML_PARSE_EVENT_TAG_OPEN_START,
	__RPC_XML_TAG_STRINGS(XML_PARSE_EVENT_OPEN), //!< one of these values will be transmitted if a known tag is found
	XML_PARSE_EVENT_TAG_OPEN_UNKNOWN, //!< If this is transmitted there is a new open

	__XML_PARSE_EVENT_TAG_CLOSE_START,
	__RPC_XML_TAG_STRINGS(XML_PARSE_EVENT_CLOSE), //!< one of these values will be transmitted if a known close tag is found
	XML_PARSE_EVENT_TAG_CLOSE_UNKNOWN,

	__XML_PARSE_EVENT_TAG_EMPTY,
	__RPC_XML_TAG_STRINGS(XML_PARSE_EVENT_EMPTY), //!< one of these values will be transmitted if a known empty tag (<tag/>) is found
	XML_PARSE_EVENT_TAG_EMPTY_UNKNOWN,

	__XML_PARSE_EVENT_KNOWN_ATTRIBUTES_START,
	__RPC_XML_ATTRIBUTE_STRINGS(XML_PARSE_EVENT), //!< one of these values will be transmitted if a known attribute ist found
	__XML_PARSE_EVENT_KNOWN_ATTRIBUTES_END,
	XML_PARSE_EVENT_UNKNOWN_ATTRIBUTE,

	XML_PARSE_EVENT_STRING, //!< This is sent when a string was found inside a tag

}parseEvents;




/**
 * This enum holds negative return values for the seekString function.
 * Every time the output of this function is negative its in that list
 */
typedef enum
{
	SEEKSTRING_NOT_IN_LIST=-100,//!< The parsed string is not in the specified list
	SEEKSTRING_STRINGEND=-200,  //!< The function reached a string terminator
}seekStringStatus_t;

/**
 * This enum holds the possible error values which can be returned by
 * the function parseStringInt. If the returned value is positive
 * it successfully parsed a number otherwise it should be one of these.
 */
typedef enum
{
	PARSE_INT_STR_END=-100, //!< This means the string ended with a terminator.
	PARSE_INT_NO_NUMBER  //!< This means the string started without a char specifiying a number
}parseStringIntStatus_t;

typedef enum
{
	__HTTP_METHODS(METHOD)
}parseMethodStringIDs_t;


/**
 * This function parses a string on in a buffer by increasing a buffer pointer.
 * @param buffer This is the pointer to a string pointer which will be increased.
 * @param goAhead This value is used to go ahead with parsing in another string if the last parse reached a terminator.
 * @return If the number is positive it's a parsed number, if not its a value from parseStringIntStatus_t
 */
int parseStringUInt(const char **buffer, bool goAhead);

/**
 * This function skips spaces inside a string by increasing the pointer as long
 * as the starting char is no space
 * @param buffer the pointer to the char pointer
 */
inline void skipSpace(const char** buffer);

/**
 * This function will skip all chars till a specified seperator is found.
 * @param buffer The pointer to the pointer which will be increased
 * @param seperatorList The list of seperators
 */
void skipAllTillSeperators(const char** buffer, char *seperatorList);


/**
 * This function seeks for a string inside a string list
 * @param stringptr The pointer to the start pointer of the string (will be increased!)
 * @param stringlist The list of the known words
 * @param stringlist_len the length of the string list
 * @param separator_list the list of seperators which end seeking
 * @param start If the previous search ended with no seperator and
 * @return Number of string inside the list or -1
 */
unsigned int seekString(const char** stringptr,
						 const char** stringlist,
						 unsigned int stringlist_len,
						 const char* separator_list,
						 bool start);


/**
 * This function checks if the char where the pointer is currently pointing on
 * is the specified one. It also will increase the value of the buffer!
 *
 * @param buffer String position to search for chr
 * @param chr The char which is expected to be there.
 */
inline bool checkforSpecialChr(const char **buffer, char chr);


/**
 * This function parses the method string in a XMLRPC Message, to check if the method is supported.
 * XMLRPC only supports HTTP POST. So only a correct header will be accepted.
 * TODO REWORK THIS DOCUMENTATION
 * @param buffer the string buffer containing the function
 * @return If the method string is valid and the method supported,
 *         it returns HTTP_METHOD_<method> or HTTP_METHOD_PARSE_INVALID or HTTP_METHOD_PARSE_UNKNOWN if not.
 */
inline unsigned int parseHttpMethod(const char **buffer, const char **methods);






#endif /* MSG_PARSE_H_ */
