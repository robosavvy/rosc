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

#define TAG_BUFFER_SIZE 25

extern const char *methods[];


typedef enum
{
	PARSER_BEGIN,	//!< This initializes the Parser
	PARSER_CONTINUE,//!< This continues to parse the current string after a find at the last position
	PARSER_APPEND   //!< This continues the current word on the next string
}parserMode;

typedef struct
{
	const char** searchstrings;		 //!< The strings which the parser searches for
	const unsigned int searchstrcnt; 	 //!< The amount of the search strings present in the list
	const char* seperators;    		 //!< String seperators which end a matching cycle
	char * wordbuffer;					 //!< The buffer for unknown
	const int wordlen;					 //!< The length of the word in the current buffer
	unsigned int first;				 //!< The first word being a possible match in the list
	unsigned int last;					 //!< The second word being a possible match in the list
	unsigned int pos;					 //!< The current position in the target string
}parserSetup;

//returns list match number, 0 for stringend, -1 for no match of current wordlist
unsigned int stringSeek(const char* str, unsigned int len, parserSetup *setup, parserMode mode);

#endif /* MSG_PARSE_H_ */
