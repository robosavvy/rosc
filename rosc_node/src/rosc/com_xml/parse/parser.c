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
	pact->mode_data.http.state=PARSE_HTTP_STATE_METHSTR_BEGIN;
	pact->submode=PARSE_SUBMODE_NONE;


	switch (type) {
		case  XMLRPC_SERVER:
			pact->handler_fkt=(parse_handler_fct_cast) &xmlrpc_server_handler;
			break;
		case XMLRPC_CLIENT:
			DEBUG_PRINT(STR, "TODO:",  "IMPLEMENT!");
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
		if(pact->mode_data.xml.state == PARSE_XML_INIT)
		{
			pact->mode_data.xml.processed_bytes=len;
		}

		if((pact->submode==PARSE_SUBMODE_NONE) || (pact->submode_state==PARSE_SUBMODE_FINISHED))
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

				case PARSE_SUBMODE_SKIPUNTILCHAR:
						#ifdef FORCE_INLINE
							#define ENABLE_C
								#include "sub/skipuntilchar.c"
							#undef ENABLE_C
						#else
							skipuntilchar(&buf, &len, pact);
						#endif
					break;
				default:
					break;
			}
		}
		if(pact->event!=PARSE_EVENT_NONE)
		{
			if(pact->event<0)
			{//ERROR skip this message
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
