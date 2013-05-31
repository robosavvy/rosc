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
 *  sebs_parse_numberparse.h created by Christian Holl
 */

#ifndef SEBS_PARSE_NUMBERPARSE_H_
#define SEBS_PARSE_NUMBERPARSE_H_

#include <rosc/system/types.h>
#include <rosc/sebs_parse_fw/sebs_parser_frame.h>

#define SEBS_PARSE_NUMBERPARSE_INIT(PARSER_DATA, DATA_STORAGE,FIGURE_MAX, NEGATIVE_ALLOWED, BASE)\
		PARSER_DATA->next_parser.parser_function=(sebs_parse_function_t) &sebs_parse_numberparse;\
		PARSER_DATA->next_parser.parser_data=(void *)(&DATA_STORAGE);\
		DATA_STORAGE.negative_allowed=NEGATIVE_ALLOWED;\
		DATA_STORAGE.figure_max=FIGURE_MAX;\
		DATA_STORAGE.base=BASE;\
		return (SEBS_PARSE_RETURN_INIT)

/**
 * These are the result values of the number parse submode
 */
typedef enum
{
	SEBS_PARSE_NUMBERPARSE_MAX_FIGURES,   //!< Means that the maximum amount of figures was reached
	SEBS_PARSE_NUMBERPARSE_ANOTHER_CHAR,  //!< Means that the number ended through a char which is not a figure
	SEBS_PARSE_NUMBERPARSE_ERROR_NONUMBER,//!< Means that there was no number to parse
}sebs_parse_numberparse_result_t;

/**
 * This struct stores the data for the numberParse submode
 */
typedef struct
{
	uint8_t base; //!if true the parsed number is expected to be in hexadecimal form
	bool negative_allowed; //!< if true negative numbers are allowed
	bool negative; //!< stores if there was a '-' in the beginning
	uint8_t cur_place; //!< cur_place stores the number of the current figure of the number
	uint8_t figure_max; //!< figure_max stores the maximum of figures to be allowed
	uint32_t number; //!< contains the parsed number after finished
	uint8_t last_byte; //!< contains the byte if result is ANOTHER_CHAR
	sebs_parse_numberparse_result_t result; //!< contains the result after the function finished
}sebs_parse_numberparse_data_t;

/**
 * This function parses a number from a stream
 * @param buf A pointer to the storage of the buffer
 * @param len The variable pointing to the length variable of the current buffer
 * @param data the function data storage, must be initialized in the beginning!
 * @return true when finished
 */
sebs_parse_return_t sebs_parse_numberparse(sebs_parser_data_t *pdata);

#endif /* SEBS_PARSE_NUMBERPARSE_H_ */
