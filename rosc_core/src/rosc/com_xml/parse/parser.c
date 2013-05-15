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
 *  parser.c created by Christian Holl
 */

#include <stdio.h>
#include <rosc/com_xml/parse/parser.h>
#include <rosc/com_xml/parse/mode/parse_mode_xml.h>
#include <rosc/com_xml/parse/mode/parse_mode_header.h>
#include <rosc/com_xml/parse/sub/subs.h>


void xmlrpc_parse_act_init(parse_act_t *pact, xmlrpc_parser_type_t type, void * handler_data_storage)
{
	pact->handler_data_storage=handler_data_storage;
	pact->mode=PARSE_MODE_HEADER;

	pact->event=PARSE_EVENT_NONE;

	pact->submode=PARSE_SUBMODE_NONE;
	pact->submode_by_handler=false;
	pact->event=PARSE_EVENT_HANDLER_INIT;
	pact->content_length=-1;

	pact->mode_data.http.sub_state=PARSE_HTTP_SUB_STATE_NONE;

	switch (type) {
		case  XMLRPC_SERVER:
			pact->mode_data.http.state=PARSE_HTTP_STATE_REQUEST_METHOD;
			pact->handler_fkt=(parse_handler_fct_cast) &xmlrpc_server_handler;
			break;
		case XMLRPC_CLIENT:
			pact->mode_data.http.state=PARSE_HTTP_STATE_RESPONSE_HTTP_VER;
			pact->handler_fkt=(parse_handler_fct_cast) &xmlrpc_client_handler;
			break;
		default:
			break;
	}

}

void xmlrpc_parse(char *buf, uint32_t len, parse_act_t* pact)
{
	bool chunk_processed=false;
	if(pact->mode == PARSE_MODE_XML)
	{
		pact->mode_data.xml.processed_bytes+=len;
	}
	while(!chunk_processed)
	{
		if(pact->mode==PARSE_MODE_XML && pact->mode_data.xml.state == PARSE_XML_INIT)
		{
			pact->mode_data.xml.processed_bytes=len;
		}

		if((pact->submode==PARSE_SUBMODE_NONE) || (pact->submode_state==PARSE_SUBMODE_FINISHED))
		{
			if(!pact->submode_by_handler)
			{
				switch(pact->mode)
				{
				case PARSE_MODE_HEADER:
						#ifdef FORCE_INLINE
							#define ENABLE_C
								#include "mode/parse_mode_header.c"
							#undef ENABLE_C
						#else
							parse_mode_header(&buf, &len, pact);
						#endif
					break;
				case PARSE_MODE_XML:
						#ifdef FORCE_INLINE
							#define ENABLE_C
								#include "mode/parse_mode_xml.c"
							#undef ENABLE_C
						#else
							parse_mode_xml(&buf, &len, pact);
						#endif
					break;
				default:
					//TODO Error
					break;
				}
			}
		}
		else
		{
			switch (pact->submode)
			{
				case PARSE_SUBMODE_COPY2BUFFER:
						#ifdef FORCE_INLINE
							#define ENABLE_C
								#include "sub/copy2buffer.c"
							#undef ENABLE_C
						#else
							copy2buffer(&buf, &len, pact);
						#endif
					break;

				case PARSE_SUBMODE_NUMBERPARSE:
						#ifdef FORCE_INLINE
							#define ENABLE_C
								#include "sub/numberparse.c"
							#undef ENABLE_C
						#else
							numberparse(&buf, &len, pact);
						#endif
					break;

				case PARSE_SUBMODE_SEEKSTRING:
						#ifdef FORCE_INLINE
							#define ENABLE_C
								#include "sub/seekstring.c"
							#undef ENABLE_C
						#else
							seekstring(&buf, &len, pact);
						#endif
					break;

				case PARSE_SUBMODE_SKIPWHOLEMESSAGE:
						#ifdef FORCE_INLINE
							#define ENABLE_C
								#include "sub/skipwholemessage.c"
							#undef ENABLE_C
						#else
							skipwholemessage(&buf, &len, pact);
						#endif
					break;

				default:
					break;
			}
		}

		if(pact->submode_by_handler)
		{
			if(pact->submode_state==PARSE_SUBMODE_FINISHED)
			{
				pact->event=PARSE_EVENT_XML_HANDLER_CALLED_SUBMODE_FINISHED;
				pact->submode_by_handler=false;
				pact->submode=PARSE_SUBMODE_NONE;
			}
		}
		if(pact->event!=PARSE_EVENT_NONE)
		{
			if(pact->event<0)
			{//On ERROR skip the whole message, notifiy handler
				PARSE_SUBMODE_INIT_SKIPWHOLEMESSAGE(pact);
			}
			pact->handler_fkt(pact);
			pact->event=PARSE_EVENT_NONE;
		}
		if((len == 0) )// && pact->submode_state!=PARSE_SUBMODE_FINISHED)
		{
			chunk_processed=true;

			if(pact->mode==PARSE_MODE_XML && pact->mode_data.xml.processed_bytes == pact->content_length)
			{
				DEBUG_PRINT_STR("FINISHED!");
//				while(1);
				return;
			}
		}
		else if(len<0)
		{
			DEBUG_PRINT(INT,"ERROR in LEN",len);
		}
	}
}
