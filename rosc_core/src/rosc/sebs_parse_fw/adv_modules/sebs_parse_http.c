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
 *  sebs_parse_http.c created by Christian Holl
 */

#include <rosc/sebs_parse_fw/adv_modules/sebs_parse_http.h>

sebs_parse_return_t sebs_parse_http(sebs_parser_data_t* pdata)
{
	sebs_parse_http_data_t *fdata=(sebs_parse_http_data_t *)pdata->current_parser.parser_data;
	if(pdata->function_init)
	{
		pdata->function_init=false;
		fdata->content_length=SEBS_PARSE_UNKNOWN;
		fdata->descriptor=SEBS_PARSE_UNKNOWN;
		fdata->substate=SEBS_PARSE_HTTP_SUBSTATE_STATE_NONE;
	}
	while (*pdata->len > 0 && pdata->event == SEBS_PARSE_EVENT_NONE)
	{
		bool is_field_content = false;
		/*
		 *	Handling substate results
		 */
		switch (fdata->substate)
		{
		case SEBS_PARSE_HTTP_SUBSTATE_CHECK_METHOD:
			if (fdata->seekstring.result >= 0)
			{
				DEBUG_PRINT_STR("->METHOD found");
				pdata->event = SEBS_PARSE_HTTP_EVENT_METHOD_PARSED;
				fdata->state = SEBS_PARSE_HTTP_STATE_REQUEST_ACTION;
			}
			else
			{
				DEBUG_PRINT_STR("ERROR! HTTP Method unknown");
				pdata->event =
						SEBS_PARSE_HTTP_EVENT_ERROR_METHOD_NOT_ALLOWED;
			}
			break;

		case SEBS_PARSE_HTTP_SUBSTATE_CHECK_ACTION:
			if (fdata->seekstring.result >= 0)
			{
				DEBUG_PRINT_STR("->ACTION found...");
				pdata->event = SEBS_PARSE_HTTP_EVENT_ACTION_PARSED;
				fdata->state = SEBS_PARSE_HTTP_STATE_REQUEST_HTTP_VER;
			}
			else
			{
				DEBUG_PRINT_STR("ERROR! HTTP Action unknown");
				pdata->event =
						SEBS_PARSE_HTTP_EVENT_ERROR_ACTION_NOT_FOUND;
			}
			break;

		case SEBS_PARSE_HTTP_SUBSTATE_CHECK_REQUEST_HTTP_VER:
			if (fdata->seekstring.result == HTTP_VAL_HTTP1_0
					|| fdata->seekstring.result == HTTP_VAL_HTTP1_1)
			{
				DEBUG_PRINT_STR("Version found...");
				fdata->state = SEBS_PARSE_HTTP_STATE_HEADLINE_WAIT_END;
			}
			else
			{
				DEBUG_PRINT_STR("ERROR! HTTP Version unknown");
				pdata->event =
						SEBS_PARSE_HTTP_EVENT_ERROR_VERSION_NOT_SUPPORTED;
			}
			break;

		case SEBS_PARSE_HTTP_SUBSTATE_CHECK_RESPONSE_HTTP_VER:
			if (fdata->seekstring.result == HTTP_VAL_HTTP1_0
					|| fdata->seekstring.result == HTTP_VAL_HTTP1_1)
			{
				DEBUG_PRINT_STR("Version found...");
				fdata->state = SEBS_PARSE_HTTP_STATE_RESPONSE_CODE;
			}
			else
			{
				DEBUG_PRINT_STR("ERROR! RESPONSE HTTP Version unknown");
				pdata->event =
						SEBS_PARSE_HTTP_EVENT_ERROR_VERSION_NOT_SUPPORTED;
			}
			break;

		case SEBS_PARSE_HTTP_SUBSTATE_CHECK_RESPONSE_CODE:
			switch (fdata->numberparse.result)
			{
			case SEBS_PARSE_NUMBERPARSE_ANOTHER_CHAR:
				if (**pdata->buf == ' ')
				{
					DEBUG_PRINT(INT,"Code of HTTP Response", fdata->numberparse.number);
					pdata->event =
							SEBS_PARSE_HTTP_EVENT_RESPONSE_CODE;
					fdata->state = SEBS_PARSE_HTTP_STATE_RESPONSE_STRING;
				}
				else
				{
					pdata->event =
							SEBS_PARSE_HTTP_EVENT_ERROR_BAD_RESPONSE;
				}
				break;

			case SEBS_PARSE_NUMBERPARSE_ERROR_NONUMBER:
				pdata->event =
						SEBS_PARSE_HTTP_EVENT_ERROR_BAD_RESPONSE;
				break;

			case SEBS_PARSE_NUMBERPARSE_MAX_FIGURES:
				pdata->event =
						SEBS_PARSE_HTTP_EVENT_ERROR_BAD_RESPONSE;
				break;
			}
			break;

		case SEBS_PARSE_HTTP_SUBSTATE_CHECK_DESCRIPTOR_ID:
			if (fdata->seekstring.result > 0)
			{
				DEBUG_PRINT(INT,"RESULT",fdata->seekstring.result);
				DEBUG_PRINT(STR,"Descriptor", fdata->seekstring.stringlist[0]);
				fdata->descriptor = fdata->seekstring.result;
			}
			else
			{
				DEBUG_PRINT_STR("Unknown Descriptor...");
			}
			fdata->state = SEBS_PARSE_HTTP_STATE_DESCRIPTOR_FIELD_SEPARATOR;
			break;

		default:
			break;
		}

		/*
		 * Check if previous state was a substate
		 */
		if (fdata->substate != SEBS_PARSE_HTTP_SUBSTATE_STATE_NONE)
		{
			fdata->substate = SEBS_PARSE_HTTP_SUBSTATE_STATE_NONE;
		}
		else
		{
			/*
			 * Parsing
			 */
			switch (**pdata->buf)
			{
			case '/':
				switch (fdata->state)
				{
				case SEBS_PARSE_HTTP_STATE_REQUEST_ACTION:
					fdata->substate = SEBS_PARSE_HTTP_SUBSTATE_CHECK_ACTION;
					++*pdata->buf;
					--*pdata->len;
					SEBS_PARSE_SEEKSTRING_INIT(pdata,
							fdata->seekstring,
							fdata->actions, fdata->actions_len,
							" ", true,0)
					;
					break;

				case SEBS_PARSE_HTTP_STATE_FIELD:
					is_field_content = true;
					break;

				case SEBS_PARSE_HTTP_STATE_FIELD_CONTENT:
					break;

				case SEBS_PARSE_HTTP_STATE_RESPONSE_STRING:
					break;

				default:
					pdata->event =
							SEBS_PARSE_HTTP_EVENT_ERROR_BAD_REQUEST;
					break;
				}
				break;

			case ':':
				switch (fdata->state)
				{

				case SEBS_PARSE_HTTP_STATE_FIELD_CONTENT:
					break;

				case SEBS_PARSE_HTTP_STATE_FIELD:
					is_field_content = true;
					break;

				case SEBS_PARSE_HTTP_STATE_DESCRIPTOR_FIELD_SEPARATOR:
					fdata->state = SEBS_PARSE_HTTP_STATE_FIELD;
					break;

				case SEBS_PARSE_HTTP_STATE_RESPONSE_STRING:
					break;

				default:
					pdata->event =
							SEBS_PARSE_HTTP_EVENT_ERROR_BAD_REQUEST;
					break;

				}
				break;

			case ' ':
				switch (fdata->state)
				{

				case SEBS_PARSE_HTTP_STATE_FIELD_CONTENT:
					fdata->state = SEBS_PARSE_HTTP_STATE_FIELD;
					break;

				case SEBS_PARSE_HTTP_STATE_RESPONSE_STRING:
					break;

				default:
					break;
				}
				break;

			case '\n':
				switch (fdata->state)
				{
				case SEBS_PARSE_HTTP_STATE_FIELD:
				case SEBS_PARSE_HTTP_STATE_FIELD_CONTENT:
				case SEBS_PARSE_HTTP_STATE_HEADLINE_WAIT_END:
				case SEBS_PARSE_HTTP_STATE_RESPONSE_STRING:
					fdata->state =
							SEBS_PARSE_HTTP_STATE_DESCRIPTOR_OR_HEADER_END;
					break;

				case SEBS_PARSE_HTTP_STATE_DESCRIPTOR_OR_HEADER_END:
					pdata->event = SEBS_PARSE_HTTP_EVENT_HEADER_END;
					return (SEBS_PARSE_RETURN_GO_AHEAD);
					break;

				case SEBS_PARSE_HTTP_STATE_RESPONSE_CODE:
				case SEBS_PARSE_HTTP_STATE_DESCRIPTOR_FIELD_SEPARATOR:
				default:
					pdata->event =
							SEBS_PARSE_HTTP_EVENT_ERROR_BAD_REQUEST;
					break;
				}
				break;

				//Any other char
			default:
				switch (fdata->state)
				///@note Here is a lot of potential for saving memory by reducing setup cause it is always almost the same
				{
				case SEBS_PARSE_HTTP_STATE_REQUEST_METHOD:
					fdata->substate = SEBS_PARSE_HTTP_SUBSTATE_CHECK_METHOD;
					SEBS_PARSE_SEEKSTRING_INIT(pdata,
							fdata->seekstring, fdata->methods,
							fdata->methods_len, " /\n.", true,0)
					;
					break;

				case SEBS_PARSE_HTTP_STATE_REQUEST_HTTP_VER:
					fdata->substate =
							SEBS_PARSE_HTTP_SUBSTATE_CHECK_REQUEST_HTTP_VER;
					SEBS_PARSE_SEEKSTRING_INIT(pdata,
							fdata->seekstring, http_header_stdtext,
							HTTP_HEADER_STDTEXT_LEN, " \n", true,0)
					;
					break;

				case SEBS_PARSE_HTTP_STATE_RESPONSE_HTTP_VER:
					fdata->substate =
							SEBS_PARSE_HTTP_SUBSTATE_CHECK_RESPONSE_HTTP_VER;
					SEBS_PARSE_SEEKSTRING_INIT(pdata,
							fdata->seekstring, http_header_stdtext,
							HTTP_HEADER_STDTEXT_LEN, " \n", true,0)
					;
					break;

				case SEBS_PARSE_HTTP_STATE_RESPONSE_CODE:
					fdata->substate =
							SEBS_PARSE_HTTP_SUBSTATE_CHECK_RESPONSE_CODE;
					SEBS_PARSE_NUMBERPARSE_INIT(
							pdata,
							fdata->numberparse, 3, false, 10);
					break;

				case SEBS_PARSE_HTTP_STATE_DESCRIPTOR_OR_HEADER_END:
					fdata->substate =
							SEBS_PARSE_HTTP_SUBSTATE_CHECK_DESCRIPTOR_ID;
					SEBS_PARSE_SEEKSTRING_INIT(pdata,
							fdata->seekstring,
							fdata->descriptors,
							fdata->descriptors_len, " :", false,0)
					;
					break;

				case SEBS_PARSE_HTTP_STATE_FIELD:
					is_field_content = true;
					break;

				default:
					break;
				}
				break;
			}

			//special external state for field content
			//must be external due to it's size and need to be placed in multiple positions
			if (is_field_content)
			{
				DEBUG_PRINT_STR("CONTENT_START");
				fdata->state = SEBS_PARSE_HTTP_STATE_FIELD_CONTENT;
				pdata->event =
						SEBS_PARSE_HTTP_EVENT_HEADER_CONTENT;
				fdata->state = SEBS_PARSE_HTTP_STATE_FIELD_CONTENT;
			}

			//We need to exclude header content event here because the first byte of the content
			//would be skipped for a subfunction called by the handler!
			if (pdata->len > 0 && pdata->event != SEBS_PARSE_HTTP_EVENT_HEADER_CONTENT)
			{
				++*pdata->buf;
				--*pdata->len;
			}
			else
			{
				break;
			}
		}
	}
	return (SEBS_PARSE_RETURN_GO_AHEAD);
}
