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
 *  parse_mode_xml.c created by Christian Holl on 28.03.2013
 */


#ifndef FORCE_INLINE
	#ifndef ENABLE_C
		#define ENABLE_C
	#endif

	#include <rosc/com_xml/parse/parser_structure.h>
	#include <rosc/com_xml/parse/parser.h>
	#include <rosc/string_res/msg_strings.h>
	#include <rosc/com_xml/parse/mode/parse_mode_xml.h>

#endif

#ifndef FORCE_INLINE
	void parse_mode_xml(char **buf_ptr, uint32_t *len_ptr, parse_act_t *pact)
#endif
#ifdef ENABLE_C
{
	#ifndef FORCE_INLINE
		uint32_t len=*len_ptr;
		char *buf=*buf_ptr;
	#endif
// ///////////////////////////////////////

		if(pact->mode_data.xml.state==PARSE_XML_INIT)
		{
			pact->mode_data.xml.depth=0;
			pact->mode_data.xml.current_tag=XML_TAG_NONE;
			pact->mode_data.xml.state=PARSE_XML_ROOT;
			pact->mode_data.xml.sub_state=PARSE_XML_SUB_STATE_NONE;
		}

		while(len>=0 && pact->event == PARSE_EVENT_NONE &&
				(pact->submode==PARSE_SUBMODE_NONE || pact->submode_state==PARSE_SUBMODE_FINISHED))
		{
			if(pact->submode!=PARSE_SUBMODE_NONE)
			{
				if(pact->submode_state==PARSE_SUBMODE_FINISHED)
				{

				}
				else
				{
					break;
				}
			}


			switch(pact->mode_data.xml.sub_state)
			{
			case PARSE_XML_SUB_TAG_ID:
				pact->submode=PARSE_SUBMODE_NONE;
				if(pact->submode_result<0)
				{
					pact->mode_data.xml.current_tag=XML_TAG_UNKNOWN;
				}
				else
				{
					pact->mode_data.xml.current_tag=pact->submode_result;
				}
				break;




				default:

				break;
			}


			//Check for different chars...
			switch(*buf)
			{
			case ' ':
					switch(pact->mode_data.xml.state)
					{
						case PARSE_XML_QMTAG_EXPECT_CLOSE:
							pact->event=PARSE_EVENT_MALFORMED_XML;
						break;
						default:
							break;
					}
				break;

			case '<':
					if(pact->mode_data.xml.state == PARSE_XML_INNER ||
						pact->mode_data.xml.state == PARSE_XML_ROOT)
					{
						pact->mode_data.xml.state=PARSE_XML_TAG_START;
					}
					else
					{
						pact->event=PARSE_EVENT_MALFORMED_XML;
						break;
					}
				break;
			case '>':
					switch(pact->mode_data.xml.state)
					{
						case PARSE_XML_QMTAG_EXPECT_CLOSE:
							pact->event=PARSE_EVENT_MALFORMED_XML;
						break;
						default:
							break;
					}
				break;
			case '/':
				switch(pact->mode_data.xml.state)
				{
					case PARSE_XML_TAG_START:
						pact->mode_data.xml.state=PARSE_XML_CLOSE_TAG_START;
					break;
					case PARSE_XML_INNER:
					case PARSE_XML_ROOT:
						break;
					default:
						pact->event=PARSE_EVENT_MALFORMED_XML;
					break;
				}
				break;
			case '?':
				switch(pact->mode_data.xml.state)
				{
					case PARSE_XML_TAG_START:
						pact->mode_data.xml.tag_type=XML_TAG_TYPE_QUESTION_MARK;
					break;

					case PARSE_XML_INNER:
						break;

					case PARSE_XML_TAG:

					break;


					default:
						pact->event=PARSE_EVENT_MALFORMED_XML;
						break;
					break;
				}
				break;
			case '!':
				switch(pact->mode_data.xml.state)
				{
					case PARSE_XML_TAG_START:
						pact->mode_data.xml.tag_type=XML_TAG_TYPE_EXCLAMATION_MARK;
					break;

					case PARSE_XML_INNER:
						break;

					default:
						pact->event=PARSE_EVENT_MALFORMED_XML;
						break;
					break;
				}
				break;
			default:
				switch(pact->mode_data.xml.state)
				{
				case PARSE_XML_TAG_START:
					 printf("Submode State: %i",pact->submode_state);
					 PARSE_SUBMODE_INIT_SEEKSTRING(pact,rpc_xml_tag_strings,RPC_XML_TAG_STRINGS_LEN," /<>?!");
					 printf("Submode State: %i",pact->submode_state);
				break;

				default:
					break;
				}
				break;
			}

			if(pact->submode==PARSE_SUBMODE_NONE)
			{
				buf++;
				len--;
			}
			else
			{
				DEBUG_PRINT_STR("GOING INTO SUBMODE");
				break;
			}
		}


//	DEBUG_PRINT(STR,"XML","...");
//	exit(1);



// ///////////////////////////////////////
	#ifndef FORCE_INLINE
		*len_ptr=len;
		*buf_ptr=buf;
	#endif
}
#endif
