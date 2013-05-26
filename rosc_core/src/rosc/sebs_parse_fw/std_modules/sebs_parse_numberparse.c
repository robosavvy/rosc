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
 *  sebs_parse_numberparse.c created by Christian Holl
 */

#include <rosc/sebs_parse_fw/std_modules/sebs_parse_numberparse.h>

bool sebs_parse_numberparse(char **buf, int32_t *len, sebs_parse_numberparse_data_t *data)
	 //work around for inlining the function
{
	while(*len > 0)
	{
		if(data->cur_place == 0)
		{
			data->negative=false;
			data->number=0;
		}
		if(**buf>=48 && **buf<=57) //Check if char is a figure
		{
			if(data->cur_place < data->figure_max)//Is the length still acceptable?
			{
				if(data->cur_place>0)
					data->number*=10;//multiply current number by 10 to shift it left

				data->number+=**buf-48; //convert char to integer
				++data->cur_place;
				++*buf;
				--*len;
			}
			else
			{
				data->result=SEBS_PARSE_NUMBERPARSE_MAX_FIGURES;
				return true;
			}
		}
		else
		{
			if(data->cur_place == 0) //Are we still at the beginning?
			{
				if(**buf=='-' && data->negative_allowed)//Is it a negative number?
				{
					data->negative=true;
					++*buf;
					--*len;
					break;
				}
				else
				{
					data->result=SEBS_PARSE_NUMBERPARSE_ERROR_NONUMBER;
					return true;
				}
			}
			else
			{
				data->result=SEBS_PARSE_NUMBERPARSE_ANOTHER_CHAR;
				data->last_byte=**buf;
				return true;
			}
			break;
		}
	}
	return false;
}


