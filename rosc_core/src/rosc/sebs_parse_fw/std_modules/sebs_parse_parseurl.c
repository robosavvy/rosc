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
 *  sebs_parse_parseurl.c created by Christian Holl
 */

#include <rosc/sebs_parse_fw/std_modules/sebs_parse_parseurl.h>

sebs_parse_return_t sebs_parse_url(sebs_parser_data_t *pdata)
{
	sebs_parse_url_data_t* fdata =
			(sebs_parse_url_data_t*) pdata->current_parser.parser_data;

	if (pdata->function_init)
	{
		pdata->function_init = false;
		fdata->cur_pos = 0;
		//Save current return function
		fdata->caller.parser_function = pdata->next_parser.parser_function;
		fdata->caller.parser_data = pdata->next_parser.parser_data;
		fdata->called_by_handler = pdata->return_to_handler;
		pdata->return_to_handler = 0;
		fdata->state = SEBS_PARSE_URL_STATE_START;
	}
	bool finished = false;
	while (pdata->len > 0)
	{
		switch (fdata->state)
		{
		case SEBS_PARSE_URL_STATE_START:
			DEBUG_PRINT_STR("PARSE URL...");
			fdata->state = SEBS_PARSE_URL_STATE_CHECK_SCHEME;
			SEBS_PARSE_SEEKSTRING_INIT(pdata, fdata->seekstring,
					fdata->scheme_list, fdata->scheme_list_len, "!*'();:@$=+$,/?%#[]",
					false)
;			break;
			case SEBS_PARSE_URL_STATE_CHECK_SCHEME:
			if(fdata->seekstring.result<0)
			{
				fdata->result=SEBS_PARSE_URL_RESULT_ERROR_URL_SCHEME;
				finished=true;
			}
			else
			{
				fdata->state=SEBS_PARSE_URL_STATE_CHECK_DOUBLE_POINT;
			}
			break;
			case SEBS_PARSE_URL_STATE_CHECK_DOUBLE_POINT:
			case SEBS_PARSE_URL_STATE_CHECK_DASH0:
			case SEBS_PARSE_URL_STATE_CHECK_DASH1:
			{
				char chr_to_check;
				sebs_parse_url_state_t state_when_found;
				switch(fdata->state)
				{
					case SEBS_PARSE_URL_STATE_CHECK_DOUBLE_POINT:
					chr_to_check=':';
					state_when_found=SEBS_PARSE_URL_STATE_CHECK_DASH0;
					break;
					case SEBS_PARSE_URL_STATE_CHECK_DASH0:
					chr_to_check='/';
					state_when_found=SEBS_PARSE_URL_STATE_CHECK_DASH1;
					break;
					case SEBS_PARSE_URL_STATE_CHECK_DASH1:
					chr_to_check='/';
					state_when_found=SEBS_PARSE_URL_STATE_CHECK_ANALYSE_TYPE;
					break;
				}
				if(len > 0)
				{
					if(**pdata->buf==chr_to_check)
					{
						fdata->state=state_when_found;
					}
					else
					{
						fdata->result=SEBS_PARSE_URL_RESULT_ERROR_URL_SCHEME;
						finished=true;
					}
				}


			}
			break;
			case SEBS_PARSE_URL_STATE_CHECK_ANALYSE_TYPE:
				if(**pdata->buf == '[')
				{
					fdata->state=SEBS_PARSE_URL_STATE_PARSE_IPV6;
				}
				else if(**pdata->buf>='0' && **pdata->buf>='9')
				{
					fdata->state=SEBS_PARSE_URL_STATE_PARSE_IPV4_HOSTNAME;
				}
				else if(**pdata->buf>48 && **pdata->buf>57)
				{
					fdata->state=SEBS_PARSE_URL_STATE_PARSE_HOSTNAME;
				}
				else
				{
					fdata->result=SEBS_PARSE_URL_RESULT_ERROR_URL_MALFORMED;
				}
				break;

			default:
			break;
		}

	}
	if (finished)
	{
		//Restore calling function and return
		pdata->next_parser.parser_function = fdata->caller.parser_function;
		pdata->next_parser.parser_data = fdata->caller.parser_data;
		pdata->return_to_handler = fdata->called_by_handler;
		return (SEBS_PARSE_RETURN_FINISHED);
	}
	else
	{
		return (SEBS_PARSE_RETURN_GO_AHEAD);
	}
}

