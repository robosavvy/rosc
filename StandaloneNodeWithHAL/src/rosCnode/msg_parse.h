/*
 *  Copyright by Synapticon GmbH (www.synapticon.com)  ©2012
 *
 *  msg_parse.h
 *
 *  This file is part of ROScNode Library.
 *
 *  ROScNode Library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ROScNode Library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ROScNode Library. If not, see <http://www.gnu.org/licenses/>.
 *
 *	File created by Christian Holl on 11.09.2012
 */

#ifndef MSG_PARSE_H_
#define MSG_PARSE_H_

#include <stdbool.h>
#include "msg_strings.h"

#define TAG_BUFFER_SIZE 25


typedef enum
{
	__PARSE_SEPARATORS(PARSE_METHOD)
}parseSeparators_t;

typedef enum
{
	__HTTP_METHODS(HTTP_METHOD)
}http_methods_t;



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
void skipSpace(const char** buffer);

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
 *
 * @param buffer the string buffer containing the function
 * @return True if it's a HTTP POST Header.
 */
inline bool parseHTTPMethod(const char **buffer);





#endif /* MSG_PARSE_H_ */
