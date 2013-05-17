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
 *  copy2buffer.h created by Christian Holl
 */

#ifndef COPY2BUFFER_H_
#define COPY2BUFFER_H_

#include <rosc/msg_parsers/xml_parser_structure.h>


typedef enum
{
	COPY2BUFFER_ENDCHR,
	COPY2BUFFER_MAXLEN,
}copy2buffer_result_t;

typedef struct
{
	bool init;
	char* buffer; //!< buffer points to the place where the chars have to be stored in memory.
	uint32_t cur_pos; //!< cur_pos stores the amount of already copied chars.
	uint32_t max_len; //!< max_len is the maximum length to be used for the buffer.
	char* endChrs; //!< chrs which will mark the end of the string
	copy2buffer_result_t result; //!< stores the result of the submode
}copy2Buffer_data_t;

/**
 * This function copies data from a stream to a buffer.
 * @param buf A pointer to the storage of the buffer
 * @param len The variable pointing to the length variable of the current buffer
 * @param data the function data storage, must be initialized in the beginning!
 * @return true when finished
 *
 * @TODO Test!
 */
bool copy2buffer(char **buf, int32_t *len, copy2Buffer_data_t *data);


#endif /* COPY2BUFFER_H_ */
