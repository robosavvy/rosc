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
 *  sebs_parse_seekstring.h created by Christian Holl
 */

#ifndef SEBS_PARSE_SEEKSTRING_H_
#define SEBS_PARSE_SEEKSTRING_H_

#include <rosc/system/types.h>
#include <rosc/debug/debug_out.h>
#include <rosc/sebs_parse_fw/sebs_parser_frame.h>

/**
 * This macro defines the result number if no string was found
 */
#define SEBS_PARSE_SEEKSTRING_NOT_FOUND  -1

#define SEBS_PARSE_SEEKSTRING_INIT(PARSER_DATA, DATA_STORAGE, STRINGLIST, STRINGLIST_LEN, ENDCHRS, CASE_SENSITIVE)\
				PARSER_DATA->next_parser.parser_function=(sebs_parse_function_t) &sebs_parse_seekstring;\
				PARSER_DATA->next_parser.parser_data=(void *)(&DATA_STORAGE);\
				DATA_STORAGE.stringlist=STRINGLIST;\
				DATA_STORAGE.stringlist_len=STRINGLIST_LEN;\
				DATA_STORAGE.endchrs=ENDCHRS;\
				DATA_STORAGE.case_sensitive=CASE_SENSITIVE;\
				return (SEBS_PARSE_RETURN_INIT)

typedef struct
{
	const char **stringlist;	//!< The stringlist.
	bool case_sensitive; //!< Defines if the string must be case sensitive.
	char * endchrs;		//!< endchrs is a list of characters (string) that will end the seek like "<" when reading inside tags
	uint16_t stringlist_len; //!< Length of the stringlist to be checked for the string
	uint16_t curChrPos;	//!< The char number since the start of seekString
	int16_t result;	//!< At finish this contains the number of the found string or SEEKSTRING_STRING_NOT_FOUND
}sebs_parse_seekstring_data_t;

/**
 * This function seeks for a string from a stream inside a char array.
 *
 * @param buf A pointer to the storage of the buffer
 * @param len The variable pointing to the length variable of the current buffer
 * @param data the function data storage, must be initialized in the beginning!
 * @return true when finished
 */
sebs_parse_return_t sebs_parse_seekstring(sebs_parser_data_t* pdata);

#endif /* SEBS_PARSE_SEEKSTRING_H_ */
