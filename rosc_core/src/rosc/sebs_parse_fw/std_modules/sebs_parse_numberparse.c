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
#include <rosc/debug/debug_out.h>

sebs_parse_return_t sebs_parse_numberparse(sebs_parser_data_t *pdata)
	 //work around for inlining the function
{
	sebs_parse_numberparse_data_t *fdata=(sebs_parse_numberparse_data_t *)pdata->current_parser.parser_data;
	if(pdata->function_init)
	{
		pdata->function_init=false;
		fdata->cur_place=0;
		fdata->number=0;
#ifdef __DEBUG__PRINTS__
		if(fdata->base>36)
		{
			DEBUG_PRINT_STR("NUMBERPARSE_BASE_ERROR! BASE MUST BE SMALLER THAN 36!");
			while(1);
		}else if(fdata->base>16)
		{
			DEBUG_PRINT_STR("WARNING NUMBERPARSE: You are using a base greater than 16! What the hell are you up for? Is that intended!?!?");
		}
#endif
	}
	while(*pdata->len > 0)
	{
		bool isFigure=false;
		uint8_t figureConvertValue;
		 //Check if char is a figure ...
		if(**pdata->buf>='0' && **pdata->buf<=('9'+fdata->base-10))
		{
			isFigure=true;
			figureConvertValue='0';
		}
		else if(**pdata->buf>='a' && **pdata->buf<=('a'+fdata->base-10) && (fdata->base>10))
		{
			isFigure=true;
			figureConvertValue='a'-10;
		}
		else if(**pdata->buf>='A' && **pdata->buf<=('A'+fdata->base-10) && fdata->base>10)
		{
			isFigure=true;
			figureConvertValue='A'-10;
		}

		if(isFigure)
		{
			if(fdata->cur_place < fdata->figure_max)//Is the length still acceptable?
			{
				if(fdata->cur_place>0)
					fdata->number*=fdata->base;//multiply current number by 10 to shift it left

				fdata->number+=**pdata->buf-figureConvertValue; //convert char to integer
				++fdata->cur_place;
				++*pdata->buf;
				--*pdata->len;
			}
			else
			{
				fdata->result=SEBS_PARSE_NUMBERPARSE_MAX_FIGURES;
				return(SEBS_PARSE_RETURN_FINISHED);
			}
		}
		else
		{
			if(fdata->cur_place == 0) //Are we still at the beginning?
			{
				if(**pdata->buf=='-' && fdata->negative_allowed)//Is it a negative number?
				{
					fdata->negative=true;
					++*pdata->buf;
					--*pdata->len;
					break;
				}
				else
				{
					fdata->result=SEBS_PARSE_NUMBERPARSE_ERROR_NONUMBER;
					return(SEBS_PARSE_RETURN_FINISHED);
				}
			}
			else
			{
				fdata->result=SEBS_PARSE_NUMBERPARSE_ANOTHER_CHAR;
				fdata->last_byte=**pdata->buf;
				return(SEBS_PARSE_RETURN_FINISHED);
			}
			break;
		}
	}
	return(SEBS_PARSE_RETURN_GO_AHEAD);
}


