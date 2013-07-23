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
 *  sebs_parse_copy2buffer.h created by Christian Holl
 */

#ifndef SEBS_PARSE_COPY2BUFFER_H_
#define SEBS_PARSE_COPY2BUFFER_H_

#include <rosc/system/types.h>
#include <rosc/sebs_parse_fw/sebs_parser_frame.h>

#define SEBS_PARSE_COPY2BUFFER_INIT(PARSER_DATA,DATA_STORAGE, BUFFER, MAX_LEN, END_CHARS, BYTE_ORDER_CORRECT,IS_STRING,REPEATS)\
		PARSER_DATA->next_parser.parser_function=(sebs_parse_function_t) &sebs_parse_copy2buffer;\
		PARSER_DATA->next_parser.parser_data=(void *)(&DATA_STORAGE);\
		DATA_STORAGE.buffer=BUFFER;\
		DATA_STORAGE.max_len=MAX_LEN;\
		DATA_STORAGE.endChrs=END_CHARS;\
		DATA_STORAGE.byteOrderCorrection=(int8_t*)BYTE_ORDER_CORRECT;\
		DATA_STORAGE.is_string=IS_STRING;\
		DATA_STORAGE.repeats=REPEATS;\
		return (SEBS_PARSE_RETURN_INIT)

typedef enum
{
	COPY2BUFFER_ENDCHR,
	COPY2BUFFER_MAXLEN,
}sebs_parse_copy2buffer_result_t;

typedef struct
{
	void* buffer; //!< buffer points to the place where the chars have to be stored in memory.
	uint32_t cur_pos; //!< cur_pos stores the amount of already copied chars.
	uint32_t max_len; //!< max_len is the maximum length to be used for the buffer.
	const char* endChrs; //!< values which will mark the end of the string
	int8_t* byteOrderCorrection; //!< if this is set to a non zero value; The numbers in the array will be used to change the byte order. Must be as big as the datatype
	sebs_parse_copy2buffer_result_t result; //!< stores the result of the submode
	bool is_string; //!< if set to one the max length will be reduced by one and a string terminator will be inserted
	uint32_t repeats; //!< if this is greater zero the operation will repeated as many times as given here
}sebs_parse_copy2buffer_data_t;

/**
 * This function copies data from a stream to a buffer.
 * @param buf A pointer to the storage of the buffer
 * @param len The variable pointing to the length variable of the current buffer
 * @param data the function data storage, must be initialized in the beginning!
 * @return true when finished
 */
sebs_parse_return_t sebs_parse_copy2buffer(sebs_parser_data_t* pdata);


#endif /* SEBS_PARSE_COPY2BUFFER_H_ */
