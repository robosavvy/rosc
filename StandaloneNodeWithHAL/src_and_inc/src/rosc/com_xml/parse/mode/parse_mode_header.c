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
	#include <rosc/com_xml/msg_gen.h>
#endif


#ifndef FORCE_INLINE
	void parse_mode_header(char *buf, uint32_t *len_ptr, parse_act_t *pact)
#endif
#ifdef ENABLE_C
{
	#ifndef FORCE_INLINE
			uint32_t len=*len_ptr;
	#endif



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
				if(*buf=='\n') //Header end
				{
					printf("Header End!\n");
					while(1);
				}
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
//			case HTTP_DESC_CONTENT_TYPE:
//				pact->mode_data.http.state=PARSE_HTTP_STATE_CONTENT_TYPE;
//
//				break;
			default:
				PARSE_SUBMODE_INIT_SKIPUNTILCHAR(pact,"\n",true);
				pact->mode_data.http.state=PARSE_HTTP_STATE_DESCRIPTOR;
				break;
			}
		break;


		//Parsing a number
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
					printf("content-length: %i\n", pact->content_length);
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
}
#endif




