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
		int i;
		pdata->function_init = false;
		fdata->cur_pos = 0;
		fdata->cur_fig = 0;
		for (i = 0; i < 8; ++i)
			fdata->ip.v6[i] = 0;
		fdata->dot_cnt = 0;

		//Save current return function
		fdata->caller.parser_function = pdata->next_parser.parser_function;
		fdata->caller.parser_data = pdata->next_parser.parser_data;
		fdata->called_by_handler = pdata->return_to_handler;
		pdata->return_to_handler = 0;
		fdata->result = SEBS_PARSE_URL_RESULT_IPV6;
		fdata->state = SEBS_PARSE_URL_STATE_START;
	}
	bool finished = false;
	bool grabChar = false;
	while (*pdata->len > 0)
	{
		bool malformed = false;
		switch(fdata->state)
		{
			case SEBS_PARSE_URL_STATE_START:
				DEBUG_PRINT_STR("PARSE URL...");
				fdata->state = SEBS_PARSE_URL_STATE_CHECK_SCHEME;
				SEBS_PARSE_SEEKSTRING_INIT(pdata, fdata->seekstring,
						fdata->scheme_list, fdata->scheme_list_len,
						"!*'();:@$=+$,/?%#[]", false, fdata->url_length)
;				break;
				case SEBS_PARSE_URL_STATE_CHECK_SCHEME:
				fdata->url_scheme=fdata->seekstring.result;
				if(fdata->url_scheme>=0)
				{
					fdata->state=SEBS_PARSE_URL_STATE_CHECK_SCHEME_END;
				}
				else
				{
					finished=true;
					fdata->result=SEBS_PARSE_URL_RESULT_ERROR_URL_SCHEME;
				}
				break;
				case SEBS_PARSE_URL_STATE_PARSE_PORT:
				fdata->state=SEBS_PARSE_URL_STATE_CHECK_PORT;
				SEBS_PARSE_NUMBERPARSE_INIT(pdata, fdata->numberparse, 5, false, 10);
				break;
				case SEBS_PARSE_URL_STATE_CHECK_PORT:
				switch(fdata->numberparse.result)
				{
					case SEBS_PARSE_NUMBERPARSE_ANOTHER_CHAR:
					if(fdata->numberparse.number>1 && fdata->numberparse.number<0xFFFF)
					{
						DEBUG_PRINT_STR("NUMBER!");
						fdata->port=fdata->numberparse.number;
						fdata->state=SEBS_PARSE_URL_STATE_PARSE_WAIT_DASH;
					}
					else
					{
						malformed=true;
					}
					break;
					case SEBS_PARSE_NUMBERPARSE_MAX_FIGURES:
					malformed=true;
					break;

					case SEBS_PARSE_NUMBERPARSE_ERROR_NONUMBER:
					malformed=true;
					break;

				}
				break;

				case SEBS_PARSE_URL_STATE_PARSE_IPV6:
				fdata->state=SEBS_PARSE_URL_STATE_CHECK_IPV6;
				SEBS_PARSE_NUMBERPARSE_INIT(pdata, fdata->numberparse, 4, false, 16);
				break;

				default: //If non of the states above
				{

					switch(**pdata->buf)
					{
						case ':':
						switch(fdata->state)
						{
							case SEBS_PARSE_URL_STATE_ANALYSE_URI_TYPE:
							malformed=true;
							break;
							case SEBS_PARSE_URL_STATE_CHECK_SCHEME_END:
							fdata->state=SEBS_PARSE_URL_STATE_ANALYSE_URI_TYPE;
							break;

							case SEBS_PARSE_URL_STATE_PARSE_IPV4_HOSTNAME:
							case SEBS_PARSE_URL_STATE_PARSE_HOSTNAME:
							fdata->state=fdata->state=SEBS_PARSE_URL_STATE_PARSE_PORT;
							break;

							case SEBS_PARSE_URL_STATE_CHECK_IPV6:
							switch(fdata->numberparse.result)
							{
								case SEBS_PARSE_NUMBERPARSE_ANOTHER_CHAR:
									//FIXME - management said: implement that later. ok ok... I 'll do that later.
									//Notes to implement
									//Problem: IPv6 has the :: feature for multiple zeros (only allowed once!)
									//thats no problem but ipv6 has a resolv address feature adding a ipv4 address in the back
									//unfortunately this is separated through a doublepoint, so we know about if it's a ipv4 resolving part
									//when the first . is reached... DAMN!
									//remove numberparse stuff and add it directly here doing Base 16 and Base 10 conversion the same time...
									//or add that to numberparse...
									malformed=true;

								case SEBS_PARSE_NUMBERPARSE_ERROR_NONUMBER:

									break;
								case SEBS_PARSE_NUMBERPARSE_MAX_FIGURES:
									break;
							}

							break;
						}
						break;

						case '.':
						switch(fdata->state)
						{

							case SEBS_PARSE_URL_STATE_PARSE_IPV4_HOSTNAME:
							if(fdata->dot_cnt < 3)
							{
								++fdata->dot_cnt;
								fdata->cur_fig=0;

							}
							else //If we have more than three dots... it's a hostname
							{
								fdata->result=SEBS_PARSE_URL_RESULT_HOSTNAME;
								fdata->state=fdata->state=SEBS_PARSE_URL_STATE_PARSE_HOSTNAME;
							}
							grabChar=true;
							break;

							case SEBS_PARSE_URL_STATE_PARSE_HOSTNAME:
							grabChar=true;
							break;

							case SEBS_PARSE_URL_STATE_ANALYSE_URI_TYPE:
							malformed=true;
							break;
						}
						break;

						case '[':
						switch(fdata->state)
						{
							case SEBS_PARSE_URL_STATE_ANALYSE_URI_TYPE:
//							fdata->result=SEBS_PARSE_URL_RESULT_IPV6;
//							fdata->state=SEBS_PARSE_URL_STATE_PARSE_IPV6;
								malformed=true; //TODO REMOVE when implemented
							break;
						}
						break;

						case '/':
						switch(fdata->state)
						{
							case SEBS_PARSE_URL_STATE_ANALYSE_URI_TYPE:
							break;

							case SEBS_PARSE_URL_STATE_PARSE_IPV4_HOSTNAME:
							case SEBS_PARSE_URL_STATE_PARSE_HOSTNAME:
							case SEBS_PARSE_URL_STATE_PARSE_WAIT_DASH:
							finished=true;
							break;
						}
						break;
						default:
						switch(fdata->state)
						{
							case SEBS_PARSE_URL_STATE_ANALYSE_URI_TYPE:
							case SEBS_PARSE_URL_STATE_PARSE_IPV4_HOSTNAME:

							if(**pdata->buf>='0' && **pdata->buf<='9') //It's an IP while there are only numbers...
							{
								fdata->result=SEBS_PARSE_URL_RESULT_IPV4;
								fdata->state=fdata->state=SEBS_PARSE_URL_STATE_PARSE_IPV4_HOSTNAME;
								if(fdata->cur_fig<3)
								{
									fdata->ip.v4[fdata->dot_cnt]*=10;
									fdata->ip.v4[fdata->dot_cnt]+=**pdata->buf-'0'; //convert to integer
									++fdata->cur_fig;
								}
								else //If there are more than 3 figures, it's a hostname
								{
									fdata->result=SEBS_PARSE_URL_RESULT_HOSTNAME;
									fdata->state=fdata->state=SEBS_PARSE_URL_STATE_PARSE_HOSTNAME;
								}
							}
							else //If there is another char than 0-9 it's a hostname
							{
								fdata->result=SEBS_PARSE_URL_RESULT_HOSTNAME;
								fdata->state=fdata->state=SEBS_PARSE_URL_STATE_PARSE_HOSTNAME;
							}
							grabChar=true;
							break;

							case SEBS_PARSE_URL_STATE_PARSE_HOSTNAME:
							grabChar=true;
							break;
							default:
							malformed=true;
							break;
						}

					}

					if(grabChar)
					{
						if(fdata->cur_pos<(__HOSTNAME_MAX_LEN__+20))
						{
							fdata->hostname_buf[fdata->cur_pos]=**pdata->buf;
							fdata->cur_pos++;
						}
						else
						{
							finished=true;
							fdata->result=SEBS_PARSE_URL_RESULT_ERROR_URL_MALFORMED;
						}
					}

					if(malformed)
					{
						fdata->result=SEBS_PARSE_URL_RESULT_ERROR_URL_MALFORMED;
						finished=true;
					}
					else if(!finished)
					{

						++*pdata->buf;
						--*pdata->len;
						++fdata->url_length;
					}
				}
				break;

			}
		if (finished)
		{
			//Restore calling function and return
			pdata->next_parser.parser_function = fdata->caller.parser_function;
			pdata->next_parser.parser_data = fdata->caller.parser_data;
			pdata->return_to_handler = fdata->called_by_handler;
			fdata->hostname_buf[fdata->cur_pos] = '\0';
			return (SEBS_PARSE_RETURN_FINISHED);
		}
	}
	return (SEBS_PARSE_RETURN_GO_AHEAD);
}

