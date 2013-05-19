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
 *  copy2buffer.c created by Christian Holl
 */


#include <rosc/msg_parsers/sub/copy2buffer.h>

#define COPY2BUFFER_INIT(DATA)\
				PARSE_STRUCT->submode=PARSE_SUBMODE_NUMBERPARSE;\
				PARSE_STRUCT->submode_state=PARSE_SUBMODE_INIT;\
				PARSE_STRUCT->submode_data.numberParse.figure_max=MAX_FIGURES\
				return false;


bool copy2buffer(char **buf, int32_t *len, copy2buffer_data_t *data)
{
	const char *sep=data->endChrs;
	while(*len > 0)
	{
		bool isEndChar=false;
		while(*sep!='\0')
		{
			if(**buf==*sep)
			{
				isEndChar=true;
				break;
			}
			++sep;
		}

		if((data->cur_pos<data->max_len) && !isEndChar )
		{
			data->buffer[data->cur_pos]=**buf;
			data->cur_pos++;
			++*buf;
			--*len;
		}
		else
		{
			if(isEndChar)
			{
				data->result=COPY2BUFFER_ENDCHR;
			}
			else
			{
				data->result=COPY2BUFFER_MAXLEN;
			}
			return true; //Finished!
		}
	}
	return false; //Not finished yet
}


