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
 *  parse_mode_header.c created by Christian Holl on 22.03.2013
 */

#ifndef FORCE_INLINE
	#ifndef ENABLE_C
		#define ENABLE_C
	#endif
	#include <rosc/com_xml/parse/mode/parse_mode_header.h>
	#include <rosc/com_xml/parse/handler/xmlrpc_string_id.h>
	#include <rosc/com_xml/parse/sub/subs.h>
#endif

#ifndef FORCE_INLINE
	void parse_mode_header(char **buf_ptr, uint32_t *len_ptr, parse_act_t *pact)
#endif
#ifdef ENABLE_C
{
	#ifndef FORCE_INLINE
		uint32_t len=*len_ptr;
		char *buf=*buf_ptr;
	#endif

		while(len>0 && pact->event  == PARSE_EVENT_NONE &&
					   pact->submode== PARSE_SUBMODE_NONE)
		{



				/*
				 *	Handling substate results
				 */
					switch(pact->mode_data.http.sub_state)
					{

					case PARSE_HTTP_SUB_CHECK_METHOD:
						if(pact->submode_result>=0)
						{
							DEBUG_PRINT_STR("->METHOD found");
							pact->event=PARSE_EVENT_HTTP_METHOD_PARSED;
							pact->mode_data.http.state=PARSE_HTTP_STATE_REQUEST_ACTION;
						}
						else
						{
							DEBUG_PRINT_STR("ERROR! HTTP Method unknown");
							pact->event=PARSE_EVENT_ERROR_HTTP_METHOD_NOT_ALLOWED;
						}
						break;


						case PARSE_HTTP_SUB_CHECK_ACTION:
							if(pact->submode_result>=0)
							{
								DEBUG_PRINT_STR("->ACTION found...");
								pact->event=PARSE_EVENT_HTTP_ACTION_PARSED;
								pact->mode_data.http.state=PARSE_HTTP_STATE_REQUEST_HTTP_VER;
							}
							else
							{
								DEBUG_PRINT_STR("ERROR! HTTP Action unknown");
								pact->event=PARSE_EVENT_ERROR_NOT_FOUND_404;
							}
						break;

						case PARSE_HTTP_SUB_CHECK_VERSION_REQUEST:
							if(pact->submode_result==HTTP_VAL_HTTP1_0 || pact->submode_result==HTTP_VAL_HTTP1_1 )
							{
								DEBUG_PRINT_STR("Version found...");
								pact->mode_data.http.state=PARSE_HTTP_STATE_HEADLINE_WAIT_END;
							}
							else
							{
								DEBUG_PRINT_STR("ERROR! HTTP Version unknown");
								pact->event=PARSE_EVENT_ERROR_VERSION_NOT_SUPPORTED_505;
							}
							break;

						case PARSE_HTTP_SUB_CHECK_DESCRIPTOR_ID:

							if(pact->submode_result>=0)
							{
								DEBUG_PRINT(STR,"Descriptor",http_header_descriptors[pact->submode_result]);
								pact->mode_data.http.descriptor=pact->submode_result;
							}
							else
							{
								DEBUG_PRINT_STR("Unknown Descriptor...");
							}
							pact->mode_data.http.state=PARSE_HTTP_DESCRIPTOR_FIELD_SEPARATOR;
							break;
						case PARSE_HTTP_SUB_CHECK_CONTENT_LENGTH:

							switch(pact->submode_result)
							{
							case NUMBERPARSE_ANOTHER_CHAR:
								if(*buf==' ' || *buf=='\n')
								{
									pact->content_length=pact->submode_data.numberParse.number;
									DEBUG_PRINT(INT,"Content Length is", pact->content_length);
								}
								else
								{
									pact->event=PARSE_EVENT_ERROR_HTTP_BAD_REQUEST;
								}
								break;
							case NUMBERPARSE_ERROR_NONUMBER:
								pact->event=PARSE_EVENT_ERROR_HTTP_BAD_REQUEST;
								break;
							case NUMBERPARSE_MAX_FIGURES:
								pact->event=PARSE_EVENT_ERROR_HTTP_BAD_REQUEST;
								break;

							}

							break;
					default:
						break;
					}

					/*
					 * Check if previous state was a substate
					 */
					if(pact->mode_data.http.sub_state != PARSE_HTTP_SUB_STATE_NONE)
					{
						pact->mode_data.http.sub_state=PARSE_HTTP_SUB_STATE_NONE;
					}
					else
					{
						/*
						 * Parsing
						 */
						switch(*buf)
						{
						case '/':
							switch(pact->mode_data.http.state)
							{
							case PARSE_HTTP_STATE_REQUEST_ACTION:
								PARSE_SUBMODE_INIT_SEEKSTRING(pact,http_available_actions,HTTP_AVAILABLE_ACTIONS_LEN," ");
								pact->mode_data.http.sub_state=PARSE_HTTP_SUB_CHECK_ACTION;
								++buf;
								--len;
								break;

							case PARSE_HTTP_STATE_FIELD:
								break;

							case PARSE_HTTP_DESCRIPTOR_FIELD_SEPARATOR:
							default:
								pact->event=PARSE_EVENT_ERROR_HTTP_BAD_REQUEST;
								break;
							}
							break;

						case ':':
							switch(pact->mode_data.http.state)
							{

							case PARSE_HTTP_STATE_FIELD_CONTENT:
								break;

							case PARSE_HTTP_STATE_FIELD:
								break;

							case PARSE_HTTP_DESCRIPTOR_FIELD_SEPARATOR:
								pact->mode_data.http.state=PARSE_HTTP_STATE_FIELD;
								break;
							default:
								pact->event=PARSE_EVENT_ERROR_HTTP_BAD_REQUEST;
								break;

							}
							break;

						case ' ':
							switch(pact->mode_data.http.state)
							{

							case PARSE_HTTP_STATE_FIELD_CONTENT:
								pact->mode_data.http.state=PARSE_HTTP_STATE_FIELD;
								break;


							default:
								break;
							}
							break;

						case '\n':
							switch(pact->mode_data.http.state)
							{
							case PARSE_HTTP_STATE_FIELD:
							case PARSE_HTTP_STATE_FIELD_CONTENT:
							case PARSE_HTTP_STATE_HEADLINE_WAIT_END:
								pact->mode_data.http.state=PARSE_HTTP_STATE_DESCRIPTOR_OR_HEADER_END;
								break;

							case PARSE_HTTP_STATE_DESCRIPTOR_OR_HEADER_END:
								if(pact->content_length<0)
								{
									pact->event=PARSE_EVENT_ERROR_LENGTH_REQUIRED;
								}
								pact->submode_state=PARSE_SUBMODE_INIT;
								pact->mode=PARSE_MODE_XML;
								pact->mode_data.xml.state=PARSE_XML_INIT;
								break;

							case PARSE_HTTP_DESCRIPTOR_FIELD_SEPARATOR:
								DEBUG_PRINT_STR("ERROR: Line feed between Field and field separator(http parser)");
								pact->event=PARSE_EVENT_ERROR_HTTP_BAD_REQUEST;
								PARSE_SUBMODE_INIT_SKIPWHOLEMESSAGE(pact);
								break;

							default:
								break;
							}
							break;

						//Any other char
						default:
							switch(pact->mode_data.http.state)
							{
								case PARSE_HTTP_STATE_REQUEST_METHOD:
										PARSE_SUBMODE_INIT_SEEKSTRING(pact,http_methods,HTTP_METHODS_LEN," /\n.");
										pact->mode_data.http.sub_state=PARSE_HTTP_SUB_CHECK_METHOD;
								break;

								case PARSE_HTTP_STATE_REQUEST_HTTP_VER:
									PARSE_SUBMODE_INIT_SEEKSTRING(pact ,http_header_stdtext, HTTP_HEADER_STDTEXT_LEN," \n");
									pact->mode_data.http.sub_state=PARSE_HTTP_SUB_CHECK_VERSION_REQUEST;
									break;

								case PARSE_HTTP_STATE_DESCRIPTOR_OR_HEADER_END:
									PARSE_SUBMODE_INIT_SEEKSTRING(pact,http_header_descriptors,HTTP_HEADER_DESCRIPTORS_LEN," :");
									pact->mode_data.http.sub_state=PARSE_HTTP_SUB_CHECK_DESCRIPTOR_ID;
									break;

								case PARSE_HTTP_STATE_FIELD:
									switch(pact->mode_data.http.descriptor)
									{
									case HTTP_DESC_CONTENT_LENGTH:
										PARSE_SUBMODE_INIT_NUMBERPARSE(pact,4);
										pact->mode_data.http.sub_state=PARSE_HTTP_SUB_CHECK_CONTENT_LENGTH;
										break;

									default:
										pact->event=PARSE_EVENT_HTTP_HEADER_FIELD_CONTENT;
										break;
									}
									break;
							}
							break;
						}
						if(pact->submode==PARSE_SUBMODE_NONE && len>0 && pact->event!=PARSE_EVENT_CONTENT_START)
						{
							buf++;
							len--;
						}
					}
				}
/*
	switch(pact->mode_data.http.state)
	{
	//Parse the methodstring
	case PARSE_HTTP_STATE_METHSTR_BEGIN:
		PARSE_SUBMODE_INIT_SEEKSTRING(pact,http_methods,HTTP_METHODS_LEN," ");
		pact->mode_data.http.state=PARSE_HTTP_STATE_METHSTR_METHODSTR;
	break;

	//Call handler to check what to do with the methodstring
	case PARSE_HTTP_STATE_METHSTR_METHODSTR:
			if(pact->submode_result>=0)
			{
				pact->event=PARSE_EVENT_HTTP_METHOD_PARSED;
				pact->mode_data.http.state=PARSE_HTTP_STATE_METHSTR_BCKSLSH0;
				PARSE_SUBMODE_INIT_SKIPUNTILCHAR(pact," ",false);
			}
			else
			{
				pact->event=PARSE_EVENT_ERROR_HTTP_BAD_REQUEST;
			}
		break;

	//Check for the backslash
	case PARSE_HTTP_STATE_METHSTR_BCKSLSH0:
			if(*buf=='/')
			{
				++buf;
				--len;
				PARSE_SUBMODE_INIT_SEEKSTRING(pact,http_available_targets,HTTP_AVAILABLE_TARGETS_LEN," ");
				pact->mode_data.http.state=PARSE_HTTP_STATE_METHSTR_TARGET;
			}
			else
			{
				pact->event=PARSE_EVENT_ERROR_HTTP_BAD_REQUEST;
			}
		break;

	 //Check Target
		case PARSE_HTTP_STATE_METHSTR_TARGET:
			if(pact->submode_result>=0)
			{
				pact->event=PARSE_EVENT_HTTP_TARGET_PARSED;
				pact->mode_data.http.state=PARSE_HTTP_STATE_METHSTR_HTTP;
				PARSE_SUBMODE_INIT_SKIPUNTILCHAR(pact," ",false);
			}
			else
			{
				pact->event=PARSE_EVENT_ERROR_404;
			}
			break;

		//Compare HTTP
		case PARSE_HTTP_STATE_METHSTR_HTTP:
			PARSE_SUBMODE_INIT_SEEKSTRING(pact,http_header_stdtext,HTTP_HEADER_STDTEXT_LEN," \n");
			pact->mode_data.http.state=PARSE_HTTP_STATE_METHSTR_LF;
			break;

		//Check HTTP and separator
		case PARSE_HTTP_STATE_METHSTR_LF:
				if(pact->submode_result==HTTP_VAL_HTTP1_0 || pact->submode_result==HTTP_VAL_HTTP1_1)
				{
					if(*buf=='\n')
					{
						++buf;
						--len;
						pact->mode_data.http.state=PARSE_HTTP_STATE_DESCRIPTOR;
					}
					else
					{
						pact->event=PARSE_EVENT_ERROR_HTTP_BAD_REQUEST;
					}
				}
				else
				{
					pact->event=PARSE_EVENT_ERROR_HTTP_BAD_REQUEST;
				}
			break;


		//Parse the descriptor
		case PARSE_HTTP_STATE_DESCRIPTOR:
			if(*buf=='\n')
			{
				++buf;
				--len;
				pact->submode_state=PARSE_SUBMODE_INIT;
				pact->mode=PARSE_MODE_XML;
				pact->mode_data.xml.state=PARSE_XML_INIT;
				break;
			}
			PARSE_SUBMODE_INIT_SEEKSTRING(pact,http_header_descriptors, HTTP_HEADER_DESCRIPTORS_LEN,":\n");
			pact->mode_data.http.state=PARSE_HTTP_STATE_FIELD;
			break;


		case PARSE_HTTP_STATE_GET_FIELD_VALUE:

			switch(pact->mode_data.http.descriptor)
			{
			case HTTP_DESC_CONTENT_LENGTH:
				pact->mode_data.http.state=PARSE_HTTP_STATE_CONTENT_LENGTH;
				PARSE_SUBMODE_INIT_NUMBERPARSE(pact,4);
				break;
				//TODO Check for Content-Encoding / Content-type //Now we just guess we always get text/html
			default:
				PARSE_SUBMODE_INIT_SKIPUNTILCHAR(pact,"\n",true);
				pact->mode_data.http.state=PARSE_HTTP_STATE_DESCRIPTOR;
				break;
			}
		break;


		//Checking if content type is supported
		case PARSE_HTTP_STATE_CONTENT_TYPE:

		break;

		//Checking the content length parsing result
		case PARSE_HTTP_STATE_CONTENT_LENGTH:
			switch(pact->submode_result)
			{
			case NUMBERPARSE_ANOTHER_CHAR:
				pact->content_length=pact->submode_data.numberParse.number;

				if(*buf!=' ' && *buf!='\n')
				{
					pact->event=PARSE_EVENT_ERROR_CONTENT_LENGTH;
				}
				else
				{
					//printf("content-length: %i\n", pact->content_length);
					PARSE_SUBMODE_INIT_SKIPUNTILCHAR(pact,"\n",true);
					pact->mode_data.http.state=PARSE_HTTP_STATE_DESCRIPTOR;
				}
				break;
			case NUMBERPARSE_MAX_FIGURES:
				pact->event=PARSE_EVENT_ERROR_CONTENT_LENGTH_TOO_LONG;
				break;
			case NUMBERPARSE_ERROR_NONUMBER:
				pact->event=PARSE_EVENT_ERROR_CONTENT_LENGTH;
				break;
			}
		break;

		//Check if we have a field to be parsed
		case PARSE_HTTP_STATE_FIELD:
			if(*buf==':')//check if separator was a :
			{
				++buf;
				--len;
					if(pact->submode_result>=0)
					{
						//Known Field
						pact->mode_data.http.descriptor=pact->submode_result;
						pact->mode_data.http.state=PARSE_HTTP_STATE_GET_FIELD_VALUE;
						pact->event=PARSE_EVENT_HTTP_HEADER_FIELD;
						PARSE_SUBMODE_INIT_SKIPUNTILCHAR(pact," ",false);
					}
					else
					{
						//Skip unknown field
						PARSE_SUBMODE_INIT_SKIPUNTILCHAR(pact,"\n",true);
						pact->mode_data.http.state=PARSE_HTTP_STATE_DESCRIPTOR;
					}
			}
			else
			{
				pact->event=PARSE_EVENT_ERROR_HTTP_BAD_REQUEST;
			}
			break;
			}
*/

	#ifndef FORCE_INLINE
		*len_ptr=len;
		*buf_ptr=buf;
	#endif
}
#endif




