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

		while(len>0 && pact->event == PARSE_EVENT_NONE &&
				pact->submode==PARSE_SUBMODE_NONE)
		{


			//Are we inside a substate (awaiting "reply" from subfunction_state?)
			//If yes, in which one?
			switch(pact->mode_data.xml.sub_state)
			{
			case PARSE_XML_SUB_TAG_ID:
				pact->mode_data.xml.sub_state=PARSE_XML_SUB_STATE_NONE;
				if(pact->submode_result<0)
				{
					pact->mode_data.xml.current_tag=XML_TAG_UNKNOWN;
				}
				else
				{
					pact->mode_data.xml.current_tag=pact->submode_result;
					pact->event=PARSE_EVENT_TAG;
				}

				if(pact->submode_result==XML_TAG_TYPE_CDATA)
				{
					pact->mode_data.xml.state=PARSE_XML_CDATA;
				}
				else
				{
					pact->mode_data.xml.state=PARSE_XML_TAG;
				}
				break;

			case PARSE_XML_SUB_ATTRIBUTE_ID:
				pact->mode_data.xml.sub_state=PARSE_XML_SUB_STATE_NONE;
				if(pact->submode_result<0)
				{
					pact->mode_data.xml.attribute=XML_ATTRIBUTE_UNKNOWN;
				}
				else
				{
					pact->mode_data.xml.attribute=pact->submode_result;
				}
				pact->mode_data.xml.state=PARSE_XML_ATTRIBUTE_WAIT_EQUAL;
				break;

			case PARSE_XML_SUB_CDATA_TAG_STRING:
				pact->mode_data.xml.sub_state=PARSE_XML_SUB_STATE_NONE;
				if(pact->submode_result!=XML_TAG_CDATA)
				{
					pact->event=PARSE_EVENT_MALFORMED_XML;
				}
				else
				{
					pact->mode_data.xml.state=PARSE_XML_CDATA_EXPECT_OPEN_BRACKET;
				}
				pact->mode_data.xml.state=PARSE_XML_ATTRIBUTE_WAIT_EQUAL;
				break;
			default:
				break;
			}

				switch(*buf)
				{
				case ' ':
						switch(pact->mode_data.xml.state)
						{
							case PARSE_XML_COMMENT_END_1ST_DASH:
							case PARSE_XML_COMMENT_END_2ND_DASH:
								pact->mode_data.xml.state=PARSE_XML_COMMENT;
								break;

							case PARSE_XML_CDATA_FIRST_BRACKET:
							case PARSE_XML_CDATA_SECOND_BRACKET:
								pact->mode_data.xml.state=PARSE_XML_CDATA;
								break;

							case PARSE_XML_CDATA_EXPECT_OPEN_BRACKET:
							case PARSE_XML_QMTAG_EXPECT_CLOSE:
							case PARSE_XML_TAG_START:
							case PARSE_XML_ATTRIBUTE_WAIT_EQUAL:
								pact->event=PARSE_EVENT_MALFORMED_XML;
							break;
							default:
								break;
						}
					break;

				case '[':
					switch(pact->mode_data.xml.state)
					{
					case PARSE_XML_TAG_START:
						 pact->mode_data.xml.state=PARSE_XML_CDATA_START;
						 break;
					case PARSE_XML_CDATA_EXPECT_OPEN_BRACKET:
						if(pact->mode_data.xml.tag_type==XML_TAG_TYPE_EXCLAMATION_MARK)
						{
							pact->mode_data.xml.state=PARSE_XML_CDATA;
						}
						else
						{
							pact->event=PARSE_EVENT_MALFORMED_XML;
						}
						break;
					case PARSE_XML_COMMENT:
						break;
					default:
						pact->event=PARSE_EVENT_MALFORMED_XML;
						break;
					}
					break;

				case '<':
						switch(pact->mode_data.xml.state)
						{
							case PARSE_XML_COMMENT:
								break;
							case PARSE_XML_INNER:
							case PARSE_XML_ROOT:
								pact->mode_data.xml.tag_type=XML_TAG_TYPE_NORMAL;
								pact->mode_data.xml.state=PARSE_XML_TAG_START;
								break;

							case PARSE_XML_CDATA_FIRST_BRACKET:
							case PARSE_XML_CDATA_SECOND_BRACKET:
								pact->mode_data.xml.state=PARSE_XML_CDATA;
								break;


							case PARSE_XML_COMMENT_END_1ST_DASH:
							case PARSE_XML_COMMENT_END_2ND_DASH:
							default:
								pact->event=PARSE_EVENT_MALFORMED_XML;
								break;

						}
					break;
				case '>':
						switch(pact->mode_data.xml.state)
						{
						case PARSE_XML_COMMENT:
							break;
						case PARSE_XML_TAG:
							switch(pact->mode_data.xml.tag_type)
							{
							case XML_TAG_TYPE_EXCLAMATION_MARK:
								break;
							case XML_TAG_TYPE_QUESTION_MARK:
								pact->event=PARSE_EVENT_MALFORMED_XML;
							break;
							case XML_TAG_TYPE_NORMAL:
								pact->mode_data.xml.depth++;
								break;
							case XML_TAG_TYPE_CLOSE:
								pact->mode_data.xml.depth--;
								break;

							default:
								break;
							}
							break;
						case PARSE_XML_EXPECT_SELFCLOSE_TAG_END:
							break;
						case PARSE_XML_QMTAG_EXPECT_CLOSE:
							break;
						case PARSE_XML_COMMENT_END_1ST_DASH:
								pact->mode_data.xml.state=PARSE_XML_COMMENT;
							break;
						case PARSE_XML_COMMENT_END_2ND_DASH:
								DEBUG_PRINT_STR("COMMENT END");
								if(pact->mode_data.xml.depth==0)
									pact->mode_data.xml.state=PARSE_XML_ROOT;
								else
									pact->mode_data.xml.state=PARSE_XML_INNER;
							break;
						case PARSE_XML_CDATA:
							break;

						case PARSE_XML_CDATA_FIRST_BRACKET:
						case PARSE_XML_CDATA_SECOND_BRACKET:
							pact->mode_data.xml.state=PARSE_XML_CDATA;
							break;
						default:
							pact->event=PARSE_EVENT_MALFORMED_XML;
							break;
						}

						if(pact->mode_data.xml.state!=PARSE_XML_COMMENT &&
						   pact->mode_data.xml.state!=PARSE_XML_CDATA)
						{
							//pact->mode_data.xml.tag_type=XML_TAG_TYPE_NORMAL;
							pact->mode_data.xml.current_tag=XML_TAG_NONE;
							if(pact->mode_data.xml.depth==0)
							{
								pact->mode_data.xml.state=PARSE_XML_ROOT;
							}
							else if(pact->mode_data.xml.depth>0)
							{
								pact->mode_data.xml.state=PARSE_XML_INNER;
							}
							else
							{
								DEBUG_PRINT_STR("NEGATIVE LVL!!! MALFORMED!");
								pact->event=PARSE_EVENT_MALFORMED_XML;
							}
						}
						//DEBUG_PRINT(INT,"DEPTH",pact->mode_data.xml.depth);
					break;
				case '/':
					switch(pact->mode_data.xml.state)
					{
						case PARSE_XML_COMMENT:
							break;
						case PARSE_XML_TAG_START:
							pact->mode_data.xml.state=PARSE_XML_CLOSE_TAG_START;
						break;
						case PARSE_XML_TAG:
							pact->mode_data.xml.state=PARSE_XML_EXPECT_SELFCLOSE_TAG_END;
						break;

						case PARSE_XML_COMMENT_END_1ST_DASH:
						case PARSE_XML_COMMENT_END_2ND_DASH:
								pact->mode_data.xml.state=PARSE_XML_COMMENT;
							break;

						case PARSE_XML_INNER:
						case PARSE_XML_ROOT:
							break;
						case PARSE_XML_CDATA_FIRST_BRACKET:
						case PARSE_XML_CDATA_SECOND_BRACKET:
							pact->mode_data.xml.state=PARSE_XML_CDATA;
							break;
						default:
							pact->event=PARSE_EVENT_MALFORMED_XML;
						break;
					}
					break;
				case '-':
					switch(pact->mode_data.xml.state)
					{
						case PARSE_XML_TAG_START:
							if(pact->mode_data.xml.tag_type==XML_TAG_TYPE_EXCLAMATION_MARK)
							pact->mode_data.xml.state=PARSE_XML_COMMENT_START_1ST_DASH;
							else
								pact->event=PARSE_EVENT_MALFORMED_XML;
							break;
						case PARSE_XML_COMMENT_START_1ST_DASH:
							DEBUG_PRINT_STR("COMMENT START");
							pact->mode_data.xml.state=PARSE_XML_COMMENT;
							break;
						case PARSE_XML_COMMENT:
								pact->mode_data.xml.state=PARSE_XML_COMMENT_END_1ST_DASH;
							break;
						case PARSE_XML_COMMENT_END_1ST_DASH:
								pact->mode_data.xml.state=PARSE_XML_COMMENT_END_2ND_DASH;
							break;

						case PARSE_XML_INNER:
							break;
						case PARSE_XML_ATTRIBUTE_INSIDE_APOSTROPHE_ATTRIB:
							break;
						case PARSE_XML_ATTRIBUTE_INSIDE_QUOTES_ATTRIB:
							break;
						case PARSE_XML_CDATA_FIRST_BRACKET:
						case PARSE_XML_CDATA_SECOND_BRACKET:
							pact->mode_data.xml.state=PARSE_XML_CDATA;
							break;
						default:
							pact->event=PARSE_EVENT_MALFORMED_XML;
							break;
					}
					break;
				case '?':
					switch(pact->mode_data.xml.state)
					{

						case PARSE_XML_COMMENT_END_1ST_DASH:
						case PARSE_XML_COMMENT_END_2ND_DASH:
							pact->mode_data.xml.state=PARSE_XML_COMMENT;
						break;
						case PARSE_XML_COMMENT:
						break;
						case PARSE_XML_TAG_START:
							pact->mode_data.xml.tag_type=XML_TAG_TYPE_QUESTION_MARK;
						break;

						case PARSE_XML_INNER:
							break;

						case PARSE_XML_TAG:
							if(pact->mode_data.xml.tag_type==XML_TAG_TYPE_QUESTION_MARK)
							{
								pact->mode_data.xml.state=PARSE_XML_QMTAG_EXPECT_CLOSE;
							}
							else
							{
								pact->event=PARSE_EVENT_MALFORMED_XML;
							}
							break;
						case PARSE_XML_CDATA_FIRST_BRACKET:
						case PARSE_XML_CDATA_SECOND_BRACKET:
							pact->mode_data.xml.state=PARSE_XML_CDATA;
							break;
						default:
							pact->event=PARSE_EVENT_MALFORMED_XML;
						break;
					}
					break;
				case '!':
					switch(pact->mode_data.xml.state)
					{
						case PARSE_XML_COMMENT:
						break;
						case PARSE_XML_TAG_START:
							pact->mode_data.xml.tag_type=XML_TAG_TYPE_EXCLAMATION_MARK;
						break;

						case PARSE_XML_INNER:
							break;

						case PARSE_XML_COMMENT_END_1ST_DASH:
						case PARSE_XML_COMMENT_END_2ND_DASH:
								pact->mode_data.xml.state=PARSE_XML_COMMENT;
							break;
						case PARSE_XML_CDATA_FIRST_BRACKET:
						case PARSE_XML_CDATA_SECOND_BRACKET:
							pact->mode_data.xml.state=PARSE_XML_CDATA;
							break;
						default:
							pact->event=PARSE_EVENT_MALFORMED_XML;
							break;
						break;
					}
					break;
				case '=':
					switch(pact->mode_data.xml.state)
					{
						case PARSE_XML_COMMENT_END_1ST_DASH:
						case PARSE_XML_COMMENT_END_2ND_DASH:
							pact->mode_data.xml.state=PARSE_XML_COMMENT;
						break;
						case PARSE_XML_COMMENT:
						break;
						case PARSE_XML_ATTRIBUTE_WAIT_EQUAL:
							pact->mode_data.xml.state=PARSE_XML_ATTRIBUTE_WAIT_VAL_BOUND;
							break;
						case PARSE_XML_CDATA_FIRST_BRACKET:
						case PARSE_XML_CDATA_SECOND_BRACKET:
							pact->mode_data.xml.state=PARSE_XML_CDATA;
							break;
						default:
							pact->event=PARSE_EVENT_MALFORMED_XML;
							break;
						break;
					}
					break;
				case '\'': // '
					switch(pact->mode_data.xml.state)
					{
						case PARSE_XML_COMMENT_END_1ST_DASH:
						case PARSE_XML_COMMENT_END_2ND_DASH:
							pact->mode_data.xml.state=PARSE_XML_COMMENT;
						break;
						case PARSE_XML_COMMENT:
						break;
						case PARSE_XML_ATTRIBUTE_WAIT_VAL_BOUND:
							pact->mode_data.xml.state=PARSE_XML_ATTRIBUTE_INSIDE_APOSTROPHE_ATTRIB;
							break;
						case PARSE_XML_ATTRIBUTE_INSIDE_APOSTROPHE_ATTRIB:
							pact->mode_data.xml.state=PARSE_XML_TAG;
							break;
						case PARSE_XML_CDATA_FIRST_BRACKET:
						case PARSE_XML_CDATA_SECOND_BRACKET:
							pact->mode_data.xml.state=PARSE_XML_CDATA;
							break;
						default:
							pact->event=PARSE_EVENT_MALFORMED_XML;
							break;
						break;

					}
					break;

				case ']':
					switch(pact->mode_data.xml.state)
					{
					case PARSE_XML_CDATA:
						pact->mode_data.xml.state=PARSE_XML_CDATA_FIRST_BRACKET;
						break;
					case PARSE_XML_CDATA_FIRST_BRACKET:
						pact->mode_data.xml.state=PARSE_XML_CDATA_SECOND_BRACKET;
						break;
					case PARSE_XML_CDATA_SECOND_BRACKET: //if ]]]
						break;
					default:
						break;
					}
					break;

				case '"': // '
					switch(pact->mode_data.xml.state)
					{
						case PARSE_XML_COMMENT_END_1ST_DASH:
						case PARSE_XML_COMMENT_END_2ND_DASH:
							pact->mode_data.xml.state=PARSE_XML_COMMENT;
						break;
						case PARSE_XML_COMMENT:
						break;
						case PARSE_XML_ATTRIBUTE_WAIT_VAL_BOUND:
							pact->mode_data.xml.state=PARSE_XML_ATTRIBUTE_INSIDE_QUOTES_ATTRIB;
							break;
						case PARSE_XML_ATTRIBUTE_INSIDE_QUOTES_ATTRIB:
							pact->mode_data.xml.state=PARSE_XML_TAG;
							break;
						case PARSE_XML_CDATA_FIRST_BRACKET:
						case PARSE_XML_CDATA_SECOND_BRACKET:
							pact->mode_data.xml.state=PARSE_XML_CDATA;
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
					case PARSE_XML_COMMENT_END_1ST_DASH:
					case PARSE_XML_COMMENT_END_2ND_DASH:
							pact->mode_data.xml.state=PARSE_XML_COMMENT;
						break;
					case PARSE_XML_COMMENT:
						break;

					case PARSE_XML_CDATA_START:
						pact->mode_data.xml.sub_state=PARSE_XML_SUB_CDATA_TAG_STRING;
						PARSE_SUBMODE_INIT_SEEKSTRING(pact,rpc_xml_tag_strings,RPC_XML_TAG_STRINGS_LEN," []=\"\'/<>?!");
						break;
					case PARSE_XML_TAG_START:
						 pact->mode_data.xml.sub_state=PARSE_XML_SUB_TAG_ID;
						 PARSE_SUBMODE_INIT_SEEKSTRING(pact,rpc_xml_tag_strings,RPC_XML_TAG_STRINGS_LEN," []=\"\'/<>?!");
					break;
					case PARSE_XML_CLOSE_TAG_START:
						 pact->mode_data.xml.sub_state=PARSE_XML_SUB_TAG_ID;
						 pact->mode_data.xml.tag_type=XML_TAG_TYPE_CLOSE;
						 PARSE_SUBMODE_INIT_SEEKSTRING(pact,rpc_xml_tag_strings,RPC_XML_TAG_STRINGS_LEN," []=\"\'/<>?!");
					break;

					case PARSE_XML_TAG: //A non empty space inside a tag means, that we have a attribute.
						 pact->mode_data.xml.sub_state=PARSE_XML_SUB_ATTRIBUTE_ID;
						 PARSE_SUBMODE_INIT_SEEKSTRING(pact,rpc_xml_attribute_strings,RPC_XML_ATTRIBUTE_STRINGS_LEN," []=\"\'/<>?!");
						break;
					case PARSE_XML_CDATA_FIRST_BRACKET:
					case PARSE_XML_CDATA_SECOND_BRACKET:
						pact->mode_data.xml.state=PARSE_XML_CDATA;
						break;
					default:
						break;
					}
					break;
				}

				if(pact->submode==PARSE_SUBMODE_NONE && len>0)
				{
					buf++;
					len--;
				}
		}


// ///////////////////////////////////////
	#ifndef FORCE_INLINE
		*len_ptr=len;
		*buf_ptr=buf;
	#endif
}
#endif
