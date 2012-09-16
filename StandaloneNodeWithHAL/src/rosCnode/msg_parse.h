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





/**
 * List for XML and Header separators
 * The first entry contains seperators for HTTP headers and the second for XML.
 */
extern const char *separators[];


unsigned int seekWord(const char** wordptr,
						 const char** wordlist,
						 unsigned int wordlist_len,
						 const char* separator_list,
						 bool start);

//returns list match number, 0 for stringend, -1 for no match of current wordlist
unsigned int stringParse(const char* str, unsigned int len,  int (*handler)(int event), parserMode mode);

#endif /* MSG_PARSE_H_ */
