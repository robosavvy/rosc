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


#include <rosc/sebs_parse_fw/std_modules/sebs_parse_copy2buffer.h>

bool sebs_parse_copy2buffer(sebs_parser_data_t* pdata)
{



	sebs_parse_copy2buffer_data_t *fdata=(sebs_parse_copy2buffer_data_t *)pdata->current_parser.parser_data;

	const char *sep=fdata->endChrs;
	while(*pdata->len > 0)
	{
		bool isEndChar=false;
		while(*sep!='\0')
		{
			if(**pdata->buf==*sep)
			{
				isEndChar=true;
				break;
			}
			++sep;
		}

		if((fdata->cur_pos<(fdata->max_len-1)) && !isEndChar )
		{
			fdata->buffer[fdata->cur_pos]=**pdata->buf;
			fdata->cur_pos++;
			++*pdata->buf;
			--*pdata->len;
		}
		else
		{
			if(isEndChar)
			{
				fdata->result=COPY2BUFFER_ENDCHR;
			}
			else
			{
				fdata->result=COPY2BUFFER_MAXLEN;
			}
			fdata->buffer[fdata->cur_pos+1]='\0';
			return SEBS_PARSE_RETURN_FINISHED; //Finished!
		}
	}
	return (SEBS_PARSE_RETURN_STREAMEND); //Not finished yet
}


