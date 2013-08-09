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
 *  buffer_fill.c created by Christian Holl
 */

#include <rosc/sebs_parse_fw/gen_modules/buffer_fill.h>
#include <rosc/system/status.h>



sebs_parse_return_t buffer_fill(sebs_parser_data_t* pdata)
{
	buffer_fill_data_t *fdata =
			(buffer_fill_data_t *) pdata->current_parser.parser_data;
	if (pdata->function_init)
	{
		pdata->function_init = false;
		fdata->cur_byte=0;
	}
	int8_t const *correct;
	switch(fdata->correct_size)
	{
		case 0:
		case 1:
			correct=0;
			break;
		case 2:
			correct=g_byte_order_correction_to_system->SIZE_2_B; //to_system because we catch it from a systems variable with the correction
			break;												 //if I am wrong change to network :-)
		case 4:
			correct=g_byte_order_correction_to_system->SIZE_4_B;
			break;
		case 8:
			correct=g_byte_order_correction_to_system->SIZE_8_B;
			break;

		default:
			ROSC_FATAL("BUFFER_FILL: NO CORRECTION FOR THAT BYTE SIZE!!!!");
			break;
	}

	while(*pdata->len>0)
	{
		if(correct && !fdata->is_string)
		{
			if(fdata->cur_byte<fdata->size)
			{
				**pdata->buf=*((char *)(fdata->data
						               +(fdata->cur_byte/fdata->correct_size)
						               +correct[fdata->cur_byte%fdata->correct_size]));
				fdata->data++;
				--*pdata->len;
				++*pdata->buf;
				++fdata->cur_byte;
			}
			else
			{
				pdata->event=BUFFER_FILL_FINISHED;
				return (SEBS_PARSE_RETURN_FINISHED);
			}
		}
		else
		{
			if(fdata->cur_byte<fdata->size || (fdata->is_string && (*((char *)fdata->data) != '\0')) )
			{
				**pdata->buf=*((char *)fdata->data);
				fdata->data++;
				--*pdata->len;
				++*pdata->buf;
				++fdata->cur_byte;
			}
			else
			{
				pdata->event=BUFFER_FILL_FINISHED;
				return (SEBS_PARSE_RETURN_FINISHED);
			}
		}

		if(*(*pdata->buf-1) > ' ' && *(*pdata->buf-1)< '~')
		{
			printf("%c", *(*pdata->buf-1));
		}
		else
		{
			printf("[%x]", (unsigned int) *(*pdata->buf-1));
		}
	}
	pdata->event=BUFFER_FILL_FULL;
	return (SEBS_PARSE_RETURN_GO_AHEAD);
}
