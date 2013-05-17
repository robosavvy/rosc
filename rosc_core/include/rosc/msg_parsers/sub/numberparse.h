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
 *  numberparse.h created by Christian Holl
 */

#ifndef NUMBERPARSE_H_
#define NUMBERPARSE_H_

#include <rosc/system/types.h>

#define PARSE_SUBMODE_INIT_NUMBERPARSE(SUBMODE_PTR,DATA_STORAGE,FIGURE_MAX, NEGATIVE_ALLOWED)\
		SUBMODE_PTR=(submode_t)&numberparse;\
		DATA_STORAGE->negative_allowed=NEGATIVE_ALLOWED;\
		DATA_STORAGE->cur_place=0;\
		DATA_STORAGE->figure_max=FIGURE_MAX;
/**
 * These are the result values of the number parse submode
 */
typedef enum
{
	NUMBERPARSE_MAX_FIGURES,   //!< Means that the maximum amount of figures was reached
	NUMBERPARSE_ANOTHER_CHAR,  //!< Means that the number ended through a char which is not a figure
	NUMBERPARSE_ERROR_NONUMBER,//!< Means that there was no number to parse
}numberparse_result_t;

/**
 * This struct stores the data for the numberParse submode
 */
typedef struct
{
	bool negative_allowed; //!< if true negative numbers are allowed
	bool negative; //!< stores if there was a '-' in the beginning
	uint8_t cur_place; //!< cur_place stores the number of the current figure of the number
	uint8_t figure_max; //!< figure_max stores the maximum of figures to be allowed
	uint32_t number; //!< contains the parsed number after finished
	numberparse_result_t result; //!< contains the result after the function finished
}numberparse_data_t;

/**
 * This function parses a number from a stream
 * @param buf A pointer to the storage of the buffer
 * @param len The variable pointing to the length variable of the current buffer
 * @param data the function data storage, must be initialized in the beginning!
 * @return true when finished
 */
bool numberparse(char **buf, int32_t *len, numberparse_data_t *data);

#endif /* NUMBERPARSE_H_ */
