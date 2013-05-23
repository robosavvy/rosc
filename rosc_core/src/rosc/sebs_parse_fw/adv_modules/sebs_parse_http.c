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

bool sebs_parse_http(char **buf, int32_t *len, sebs_parse_http_data_t *data)
{

	while (*len > 0 && data->parser_data->event == SEBS_PARSE_EVENT_NONE)
	{
		bool is_field_content = false;
		/*
		 *	Handling substate results
		 */
		switch (data->substate)
		{
		case SEBS_PARSE_HTTP_SUBSTATE_CHECK_METHOD:
			if (data->std_func_data.seekstring.result >= 0)
			{
				DEBUG_PRINT_STR("->METHOD found");
				data->parser_data->event = SEBS_PARSE_EVENT_HTTP_METHOD_PARSED;

				data->state = SEBS_PARSE_HTTP_STATE_REQUEST_ACTION;
			}
			else
			{
				DEBUG_PRINT_STR("ERROR! HTTP Method unknown");
				data->parser_data->event =
						SEBS_PARSE_EVENT_HTTP_ERROR_METHOD_NOT_ALLOWED;
			}
			break;

		case SEBS_PARSE_HTTP_SUBSTATE_CHECK_ACTION:
			if (data->std_func_data.seekstring.result >= 0)
			{
				DEBUG_PRINT_STR("->ACTION found...");
				data->parser_data->event = SEBS_PARSE_EVENT_HTTP_ACTION_PARSED;
				data->state = SEBS_PARSE_HTTP_STATE_REQUEST_HTTP_VER;
			}
			else
			{
				DEBUG_PRINT_STR("ERROR! HTTP Action unknown");
				data->parser_data->event =
						SEBS_PARSE_EVENT_HTTP_ERROR_NOT_FOUND;
			}
			break;

		case SEBS_PARSE_HTTP_SUBSTATE_CHECK_REQUEST_HTTP_VER:
			if (data->std_func_data.seekstring.result == HTTP_VAL_HTTP1_0
					|| data->std_func_data.seekstring.result == HTTP_VAL_HTTP1_1)
			{
				DEBUG_PRINT_STR("Version found...");
				data->state = SEBS_PARSE_HTTP_STATE_HEADLINE_WAIT_END;
			}
			else
			{
				DEBUG_PRINT_STR("ERROR! HTTP Version unknown");
				data->parser_data->event =
						SEBS_PARSE_EVENT_HTTP_ERROR_VERSION_NOT_SUPPORTED;
			}
			break;

		case SEBS_PARSE_HTTP_SUBSTATE_CHECK_RESPONSE_HTTP_VER:
			if (data->std_func_data.seekstring.result == HTTP_VAL_HTTP1_0
					|| data->std_func_data.seekstring.result == HTTP_VAL_HTTP1_1)
			{
				DEBUG_PRINT_STR("Version found...");
				data->state = SEBS_PARSE_HTTP_STATE_RESPONSE_CODE;
			}
			else
			{
				DEBUG_PRINT_STR("ERROR! RESPONSE HTTP Version unknown");
				data->parser_data->event =
						SEBS_PARSE_EVENT_HTTP_ERROR_VERSION_NOT_SUPPORTED;
			}
			break;

		case SEBS_PARSE_HTTP_SUBSTATE_CHECK_RESPONSE_CODE:
			switch (data->std_func_data.numberparse.result)
			{
			case NUMBERPARSE_ANOTHER_CHAR:
				if (**buf == ' ')
				{
					DEBUG_PRINT(INT,"Code of HTTP Response", data->std_func_data.numberparse.number);
					data->parser_data->event =
							SEBS_PARSE_EVENT_HTTP_RESPONSE_CODE;
					data->state = SEBS_PARSE_HTTP_STATE_RESPONSE_STRING;
				}
				else
				{
					data->parser_data->event =
							SEBS_PARSE_EVENT_HTTP_ERROR_BAD_RESPONSE;
				}
				break;

			case NUMBERPARSE_ERROR_NONUMBER:
				data->parser_data->event =
						SEBS_PARSE_EVENT_HTTP_ERROR_BAD_RESPONSE;
				break;

			case NUMBERPARSE_MAX_FIGURES:
				data->parser_data->event =
						SEBS_PARSE_EVENT_HTTP_ERROR_BAD_RESPONSE;
				break;
			}
			break;

		case SEBS_PARSE_HTTP_SUBSTATE_CHECK_DESCRIPTOR_ID:
			if (data->std_func_data.seekstring.result > 0)
			{
				DEBUG_PRINT(INT,"RESULT",data->std_func_data.seekstring.result);
				DEBUG_PRINT(STR,"Descriptor", data->std_func_data.seekstring.stringlist[0]);
				data->descriptor = data->std_func_data.seekstring.result;
			}
			else
			{
				DEBUG_PRINT_STR("Unknown Descriptor...");
			}
			data->state = SEBS_PARSE_HTTP_STATE_DESCRIPTOR_FIELD_SEPARATOR;
			break;
		case SEBS_PARSE_HTTP_SUBSTATE_CHECK_CONTENT_LENGTH:

			switch (data->std_func_data.numberparse.result)
			{
			case NUMBERPARSE_ANOTHER_CHAR:
				if (**buf == ' ' || **buf == '\n')
				{
					data->content_length =
							data->std_func_data.numberparse.number;
					DEBUG_PRINT(INT,"Content Length is", data->content_length);
				}
				else
				{
					data->parser_data->event =
							SEBS_PARSE_EVENT_HTTP_ERROR_BAD_REQUEST;
				}
				break;
			case NUMBERPARSE_ERROR_NONUMBER:
				data->parser_data->event =
						SEBS_PARSE_EVENT_HTTP_ERROR_BAD_REQUEST;
				break;
			case NUMBERPARSE_MAX_FIGURES:
				data->parser_data->event =
						SEBS_PARSE_EVENT_HTTP_ERROR_BAD_REQUEST;
				break;

			}

			break;

		case SEBS_PARSE_HTTP_SUBSTATE_CHECK_CONTENT_TYPE:
			if (data->std_func_data.seekstring.result == HTTP_VAL_TEXT_XML)
				data->parser_data->event = SEBS_PARSE_EVENT_HTTP_CONTENT_TYPE;

			//default chars will lead to reengage seekstring
			//so we need to skip them here
			if (**buf == ',' || **buf == ';')
			{
				++*buf;
				--*len;
			}
			break;
		default:
			break;
		}

		/*
		 * Check if previous state was a substate
		 */
		if (data->substate != SEBS_PARSE_HTTP_SUBSTATE_STATE_NONE)
		{
			data->substate = SEBS_PARSE_HTTP_SUBSTATE_STATE_NONE;
		}
		else
		{
			/*
			 * Parsing
			 */
			switch (**buf)
			{
			case '/':
				switch (data->state)
				{
				case SEBS_PARSE_HTTP_STATE_REQUEST_ACTION:
					data->substate = SEBS_PARSE_HTTP_SUBSTATE_CHECK_ACTION;
					++*buf;
					--*len;
					SEBS_PARSE_INIT_SEEKSTRING(data->parser_data->next_parser,
							data->std_func_data.seekstring,
							http_available_actions, HTTP_AVAILABLE_ACTIONS_LEN,
							" ", true)
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
					data->parser_data->event =
							SEBS_PARSE_EVENT_HTTP_ERROR_BAD_REQUEST;
					break;
				}
				break;

			case ':':
				switch (data->state)
				{

				case SEBS_PARSE_HTTP_STATE_FIELD_CONTENT:
					break;

				case SEBS_PARSE_HTTP_STATE_FIELD:
					is_field_content = true;
					break;

				case SEBS_PARSE_HTTP_STATE_DESCRIPTOR_FIELD_SEPARATOR:
					data->state = SEBS_PARSE_HTTP_STATE_FIELD;
					break;

				case SEBS_PARSE_HTTP_STATE_RESPONSE_STRING:
					break;

				default:
					data->parser_data->event =
							SEBS_PARSE_EVENT_HTTP_ERROR_BAD_REQUEST;
					break;

				}
				break;

			case ' ':
				switch (data->state)
				{

				case SEBS_PARSE_HTTP_STATE_FIELD_CONTENT:
					data->state = SEBS_PARSE_HTTP_STATE_FIELD;
					break;

				case SEBS_PARSE_HTTP_STATE_RESPONSE_STRING:
					break;

				default:
					break;
				}
				break;

			case '\n':
				switch (data->state)
				{
				case SEBS_PARSE_HTTP_STATE_FIELD:
				case SEBS_PARSE_HTTP_STATE_FIELD_CONTENT:
				case SEBS_PARSE_HTTP_STATE_HEADLINE_WAIT_END:
				case SEBS_PARSE_HTTP_STATE_RESPONSE_STRING:
					data->state =
							SEBS_PARSE_HTTP_STATE_DESCRIPTOR_OR_HEADER_END;
					break;

				case SEBS_PARSE_HTTP_STATE_DESCRIPTOR_OR_HEADER_END:
					if (data->content_length < 0)
					{
						data->parser_data->event =
								SEBS_PARSE_EVENT_HTTP_ERROR_LENGTH_REQUIRED;
					}
					data->parser_data->event = SEBS_PARSE_EVENT_HTTP_HEADER_END;
					return false;
					break;

				case SEBS_PARSE_HTTP_STATE_RESPONSE_CODE:
				case SEBS_PARSE_HTTP_STATE_DESCRIPTOR_FIELD_SEPARATOR:
					data->parser_data->event =
							SEBS_PARSE_EVENT_HTTP_ERROR_BAD_REQUEST;
					break;

				default: //TODO ^^ merge with above???
					break;
				}
				break;

				//Any other char
			default:
				switch (data->state)
				///@note Here is a lot of potential for saving memory by reducing setup cause it is always almost the same
				{
				case SEBS_PARSE_HTTP_STATE_REQUEST_METHOD:
					data->substate = SEBS_PARSE_HTTP_SUBSTATE_CHECK_METHOD;
					SEBS_PARSE_INIT_SEEKSTRING(data->parser_data->next_parser,
							data->std_func_data.seekstring, http_methods,
							HTTP_METHODS_LEN, " /\n.", true)
					;
					break;

				case SEBS_PARSE_HTTP_STATE_REQUEST_HTTP_VER:
					data->substate =
							SEBS_PARSE_HTTP_SUBSTATE_CHECK_REQUEST_HTTP_VER;
					SEBS_PARSE_INIT_SEEKSTRING(data->parser_data->next_parser,
							data->std_func_data.seekstring, http_header_stdtext,
							HTTP_HEADER_STDTEXT_LEN, " \n", true)
					;
					break;

				case SEBS_PARSE_HTTP_STATE_RESPONSE_HTTP_VER:
					data->substate =
							SEBS_PARSE_HTTP_SUBSTATE_CHECK_RESPONSE_HTTP_VER;
					SEBS_PARSE_INIT_SEEKSTRING(data->parser_data->next_parser,
							data->std_func_data.seekstring, http_header_stdtext,
							HTTP_HEADER_STDTEXT_LEN, " \n", true)
					;
					break;

				case SEBS_PARSE_HTTP_STATE_RESPONSE_CODE:
					data->substate =
							SEBS_PARSE_HTTP_SUBSTATE_CHECK_RESPONSE_CODE;
					SEBS_PARSE_INIT_NUMBERPARSE(
							data->parser_data->next_parser,
							data->std_func_data.numberparse, 3, false);
					break;

				case SEBS_PARSE_HTTP_STATE_DESCRIPTOR_OR_HEADER_END:
					data->substate =
							SEBS_PARSE_HTTP_SUBSTATE_CHECK_DESCRIPTOR_ID;
					SEBS_PARSE_INIT_SEEKSTRING(data->parser_data->next_parser,
							data->std_func_data.seekstring,
							http_header_descriptors,
							HTTP_HEADER_DESCRIPTORS_LEN, " :", false)
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
				data->state = SEBS_PARSE_HTTP_STATE_FIELD_CONTENT;

				switch (data->descriptor)
				{

				case HTTP_DESC_CONTENT_ENCODING:
					/* NO ENCODING IS SUPPORTED->ERROR 415*/
					data->parser_data->event =
							SEBS_PARSE_EVENT_HTTP_ERROR_CONTENT_ENCODING;
					break;

				case HTTP_DESC_CONTENT_LENGTH:
					if (data->content_length == -1)
					{
						data->substate =
								SEBS_PARSE_HTTP_SUBSTATE_CHECK_CONTENT_LENGTH;
						SEBS_PARSE_INIT_NUMBERPARSE(
								data->parser_data->next_parser,
								data->std_func_data.numberparse, 4, 0);
					}
					else
					{
						data->parser_data->event =
								SEBS_PARSE_EVENT_HTTP_ERROR_BAD_REQUEST;
						DEBUG_PRINT_STR("CONTENT LENGTH FIELD ERROR");
					}
					break;
				case HTTP_DESC_CONTENT_TYPE:
					data->substate =
							SEBS_PARSE_HTTP_SUBSTATE_CHECK_CONTENT_TYPE;
					SEBS_PARSE_INIT_SEEKSTRING(data->parser_data->next_parser,
							data->std_func_data.seekstring, http_header_stdtext,
							HTTP_HEADER_STDTEXT_LEN, " ,;\n", true)
					;
					break;

				default:
					data->parser_data->event =
							SEBS_PARSE_EVENT_HTTP_HEADER_CONTENT;
					data->state = SEBS_PARSE_HTTP_STATE_FIELD_CONTENT;
					break;
				}
			}

			if (len > 0)
			{
				++*buf;
				--*len;
			}
			else
			{
				break;
			}
		}
	}
	return (false);
}
