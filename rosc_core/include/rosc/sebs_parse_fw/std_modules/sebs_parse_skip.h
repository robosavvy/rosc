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
 *  sebs_parse_skip.h created by Christian Holl
 */

#ifndef SEBS_PARSE_SKIP_H_
#define SEBS_PARSE_SKIP_H_

#include <rosc/system/spec.h>
#include <rosc/debug/debug_out.h>
#include <rosc/sebs_parse_fw/sebs_parser_frame.h>

#define SEBS_PARSE_SKIP_INIT(PARSER_DATA, DATA_STORAGE, LEN)\
				PARSER_DATA->next_parser.parser_function=(sebs_parse_function_t) &sebs_parse_skip;\
				PARSER_DATA->next_parser.parser_data=(void *)(&DATA_STORAGE);\
				DATA_STORAGE.len=LEN;\
				return (SEBS_PARSE_RETURN_INIT)


typedef struct
{
	uint32_t len;
	uint32_t curPos;
}sebs_parse_skip_data_t;


/**
 * This function skips a given amount of bytes
 *
 * @param buf A pointer to the storage of the buffer
 * @param len The variable pointing to the length variable of the current buffer
 * @param data the function data storage, must be initialized in the beginning!
 * @return true when finished
 */
sebs_parse_return_t sebs_parse_skip(sebs_parser_data_t* pdata);

#endif /* SEBS_PARSE_SKIP_H_ */
