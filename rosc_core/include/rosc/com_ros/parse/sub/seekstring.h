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
 *  seekstring.h created by Christian Holl
 */

#ifndef SEEKSTRING_H_
#define SEEKSTRING_H_

#include <rosc/system/types.h>
#include <rosc/com_ros/parse/xml_parser_structure.h>

#define STRING_NOT_FOUND  -1


#define PARSE_SUBMODE_INIT_SEEKSTRING(PARSE_STRUCT,STRING_ARRAY,ARRAY_LEN,SEP)\
				PARSE_STRUCT->submode_state=PARSE_SUBMODE_INIT;\
				PARSE_STRUCT->submode=PARSE_SUBMODE_SEEKSTRING;\
				PARSE_STRUCT->submode_data.seekString.stringlist=(char**)STRING_ARRAY;\
				PARSE_STRUCT->submode_data.seekString.stringlist_len=ARRAY_LEN;\
				PARSE_STRUCT->submode_data.seekString.endchrs=SEP




#ifndef FORCE_INLINE
	void seekstring(char **buf_ptr, uint32_t *len_ptr, parse_act_t *pact);
#endif


#endif /* SEEKSTRING_H_ */
