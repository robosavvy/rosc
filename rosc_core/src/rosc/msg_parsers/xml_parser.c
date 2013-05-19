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

#include <rosc/system/types.h>
#include <rosc/msg_parsers/xml_parser.h>
#include <rosc/msg_parsers/xml_mode/parse_mode_xml.h>
#include <rosc/msg_parsers/xml_mode/parse_mode_header.h>
#include <rosc/msg_parsers/sub/subs.h>


void xmlrpc_parse_act_init(parse_act_t *pact, xmlrpc_parser_type_t type, void * handler_data_storage)
{
	pact->handler_data_storage=handler_data_storage;
	pact->mode=PARSE_MODE_HEADER;

	pact->submode=0;
	pact->event=PARSE_EVENT_NONE;

	//pact->submode=PARSE_SUBMODE_NONE;
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

bool xmlrpc_parse(char *buf, int32_t len, parse_act_t* pact)
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

		if((pact->submode==0))
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
			if(pact->submode)
			{
				if(pact->submode(&buf, &len, (void *)&pact->submode_data))
				{
					pact->submode=0;
					if(pact->submode_by_handler)
					{
						pact->event=PARSE_EVENT_XML_HANDLER_CALLED_SUBMODE_FINISHED;
					}
				}

			}
		}

		if(pact->event!=PARSE_EVENT_NONE)
		{
			if(pact->event<0)
			{//On ERROR skip the whole message, notifiy handler
				PARSE_SUBMODE_INIT_SKIPWHOLEMESSAGE(pact->submode);
			}
			pact->handler_fkt(pact);
			pact->event=PARSE_EVENT_NONE;
			pact->submode_by_handler=false;
		}
		if(len == 0)
		{
			chunk_processed=true;

			if(pact->mode==PARSE_MODE_XML && pact->mode_data.xml.processed_bytes == pact->content_length)
			{
				DEBUG_PRINT_STR("FINISHED!");
//				while(1);
				return false;
			}
		}
	}
}
