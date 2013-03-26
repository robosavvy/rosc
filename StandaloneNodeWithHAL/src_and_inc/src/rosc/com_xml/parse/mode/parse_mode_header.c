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
				PARSE_SUBMODE_INIT_SKIPWHOLEMESSAGE(pact);
			}


		break;

	//Check for the backslash
	case PARSE_HTTP_STATE_METHSTR_BCKSLSH0:
			if(*buf=='/')
			{
				buf++;
				len--;
				PARSE_SUBMODE_INIT_SEEKSTRING(pact,http_available_targets,HTTP_AVAILABLE_TARGETS_LEN," ");
				pact->mode_data.http.state=PARSE_HTTP_STATE_METHSTR_TARGET;
			}
			else
			{
				pact->event=PARSE_EVENT_ERROR_HTTP_BAD_REQUEST;
				PARSE_SUBMODE_INIT_SKIPWHOLEMESSAGE(pact);
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
				PARSE_SUBMODE_INIT_SKIPWHOLEMESSAGE(pact);
			}
			break;

		//Compare HTTP
		case PARSE_HTTP_STATE_METHSTR_HTTP:
			{
				char *http = "HTTP";
				PARSE_SUBMODE_INIT_SEEKSTRING(pact,&http,1,"/");
				pact->mode_data.http.state=PARSE_HTTP_STATE_METHSTR_BCKSLSH1;
			}
			break;


		//Check for the backslash and check if HTTP was found.
		case PARSE_HTTP_STATE_METHSTR_BCKSLSH1:
			if(pact->submode_result>=0
					&& (*buf=='/'))
			{
				buf++;
				len--;
			}
			else
			{
				pact->event=PARSE_EVENT_ERROR_HTTP_BAD_REQUEST;
				PARSE_SUBMODE_INIT_SKIPWHOLEMESSAGE(pact);
			}
			break;
	}
}
#endif




