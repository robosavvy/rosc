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
 *  sebs_parse_xml.c created by Christian Holl
 */

#include <rosc/sebs_parse_fw/adv_modules/sebs_parse_xml.h>

bool sebs_parse_xml(sebs_parser_data_t* pdata)
{
	sebs_parse_xml_data_t *fdata=(sebs_parse_xml_data_t *)pdata->current_parser.parser_data;
	if(pdata->function_init)
	{
		pdata->function_init=false;
		fdata->depth=0;\
		fdata->processed_bytes=0;\
		fdata->state=SEBS_PARSE_XML_STATE_ROOT;\
		fdata->substate=SEBS_PARSE_XML_SUBSTATE_NONE;\
		fdata->tag_type=SEBS_PARSE_UNKNOWN;\
	}
	while (*pdata->len > 0 && pdata->event == SEBS_PARSE_EVENT_NONE)
	{

		//Are we inside a substate (awaiting "reply" from subfunction_state?)
		//If yes, in which one?
		switch (fdata->substate)
		{
		case SEBS_PARSE_XML_SUBSTATE_TAG_ID:

			if (fdata->seekstring.result < 0)
			{
				fdata->current_tag = -1;
			}
			else
			{
				fdata->current_tag = fdata->seekstring.result;
				pdata->event = SEBS_PARSE_XML_EVENT_TAG;
			}

			if (fdata->seekstring.result
					== SEBS_PARSE_XML_TAG_TYPE_CDATA)
			{
				fdata->state =SEBS_PARSE_XML_STATE_CDATA;
			}
			else
			{
				fdata->state =SEBS_PARSE_XML_STATE_TAG;
			}
			break;

		case SEBS_PARSE_XML_SUBSTATE_ATTRIBUTE_ID:

			if (fdata->seekstring.result < 0)
			{
				fdata->attribute = -1;
			}
			else
			{
				fdata->attribute = fdata->seekstring.result;
			}
			fdata->state =SEBS_PARSE_XML_STATE_ATTRIBUTE_WAIT_EQUAL;
			break;

		case SEBS_PARSE_XML_SUBSTATE_CDATA_TAG_STRING:

			if (fdata->seekstring.result == 0)
			{
				fdata->state =SEBS_PARSE_XML_STATE_CDATA_EXPECT_OPEN_BRACKET;
			}
			else
			{
				pdata->event = SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
			}
			break;
		default:
			break;
		}

		if (fdata->substate !=SEBS_PARSE_XML_SUBSTATE_NONE)
		{
			fdata->substate =SEBS_PARSE_XML_SUBSTATE_NONE;
		}
		else
		{
			switch (**pdata->buf)
			{
			//the whitespace stuff ...
			case '\t':
			case '\n':
			case ' ':
				switch (fdata->state)
				{
				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
				case SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH:
					fdata->state =SEBS_PARSE_XML_STATE_COMMENT;
					break;

				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					fdata->state =SEBS_PARSE_XML_STATE_CDATA;
					break;

				case SEBS_PARSE_XML_STATE_CDATA_EXPECT_OPEN_BRACKET:
				case SEBS_PARSE_XML_STATE_QMTAG_EXPECT_CLOSE:
				case SEBS_PARSE_XML_STATE_TAG_START:
				case SEBS_PARSE_XML_STATE_ATTRIBUTE_WAIT_EQUAL:
					pdata->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					fdata->state =SEBS_PARSE_XML_STATE_INNER;
					break;
				default:
					break;
				}
				break;

			case '[':
				switch (fdata->state)
				{
				case SEBS_PARSE_XML_STATE_TAG_START:
					if(fdata->tag_type==SEBS_PARSE_XML_TAG_TYPE_EXCLAMATION_MARK)
					{
						fdata->state=SEBS_PARSE_XML_STATE_CDATA_START;
					}
					else
					{
						pdata->event=SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					}
					break;
				case SEBS_PARSE_XML_STATE_CDATA_EXPECT_OPEN_BRACKET:
						fdata->state =SEBS_PARSE_XML_STATE_CDATA;
						pdata->event=SEBS_PARSE_XML_EVENT_CDATA;
					break;
				case SEBS_PARSE_XML_STATE_COMMENT:
					break;
				case SEBS_PARSE_XML_STATE_INNER:
					pdata->event =
							SEBS_PARSE_XML_EVENT_CONTENT_START;
					fdata->state =SEBS_PARSE_XML_STATE_INNER_CONTENT;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					break;
				default:
					pdata->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
				}
				break;

			case '<':
				switch (fdata->state)
				{
				case SEBS_PARSE_XML_STATE_COMMENT:
					break;

				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
				case SEBS_PARSE_XML_STATE_INNER:
				case SEBS_PARSE_XML_STATE_ROOT:
					fdata->tag_type = SEBS_PARSE_XML_TAG_TYPE_NORMAL;
					fdata->state = SEBS_PARSE_XML_STATE_TAG_START;
					break;

				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					fdata->state =SEBS_PARSE_XML_STATE_CDATA;
					break;

				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
				case SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH:

				default:
					pdata->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;

				}
				break;
			case '>':
				switch (fdata->state)
				{
				case SEBS_PARSE_XML_STATE_COMMENT:
					break;
				case SEBS_PARSE_XML_STATE_TAG:
					switch (fdata->tag_type)
					{
					case SEBS_PARSE_XML_TAG_TYPE_EXCLAMATION_MARK:
						break;
					case SEBS_PARSE_XML_TAG_TYPE_QUESTION_MARK:
						pdata->event =
								SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
						break;
					case SEBS_PARSE_XML_TAG_TYPE_NORMAL:
						if (fdata->depth != SEBS_PARSE_XML_MAX_DEPTH)
						{
							fdata->depth++;
							fdata->tags[fdata->depth] = fdata->current_tag;
							pdata->event =
									SEBS_PARSE_XML_EVENT_INSIDE_TAG;
						}
						else
						{
							pdata->event =
									SEBS_PARSE_XML_EVENT_ERROR_DEPTH;
							DEBUG_PRINT_STR("XML ERROR! TOO DEEP!");
						}
						break;
					case SEBS_PARSE_XML_TAG_TYPE_CLOSE:
						if (fdata->tags[fdata->depth] == fdata->current_tag)
						{
							if (fdata->depth > 0)
								fdata->depth--;
							else
								pdata->event =
										SEBS_PARSE_XML_EVENT_ERROR_DEPTH;
						}
						else
						{
							DEBUG_PRINT_STR("XML ERROR! XML INVALID!"); //TODO Error Handling
							pdata->event =
									SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
						}
						break;

					default:
						break;
					}
					break;
				case SEBS_PARSE_XML_STATE_EXPECT_SELFCLOSE_TAG_END:
					break;
				case SEBS_PARSE_XML_STATE_QMTAG_EXPECT_CLOSE:
					break;
				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
					fdata->state =SEBS_PARSE_XML_STATE_COMMENT;
					break;
				case SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH:
					DEBUG_PRINT_STR("COMMENT END");
					if (fdata->depth == 0)
						fdata->state =SEBS_PARSE_XML_STATE_ROOT;
					else
						fdata->state =SEBS_PARSE_XML_STATE_INNER;
					break;
				case SEBS_PARSE_XML_STATE_CDATA:
					break;

				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					if(fdata->depth==0)
					{
						fdata->state=SEBS_PARSE_XML_STATE_INNER;
					}
					else
					{
						fdata->state=SEBS_PARSE_XML_STATE_ROOT;
					}
					break;
				default:
					pdata->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
				}

				if (fdata->state !=SEBS_PARSE_XML_STATE_COMMENT
						&& fdata->state !=SEBS_PARSE_XML_STATE_CDATA)
				{
					fdata->current_tag = -1;
					if (fdata->depth == 0)
					{
						fdata->state =SEBS_PARSE_XML_STATE_ROOT;
					}
					else if (fdata->depth > 0)
					{
						fdata->state =SEBS_PARSE_XML_STATE_INNER;
					}
					else
					{
						DEBUG_PRINT_STR("NEGATIVE LVL!!! MALFORMED!");
						pdata->event =
								SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					}
				}

				break;
			case '/':
				switch (fdata->state)
				{
				case SEBS_PARSE_XML_STATE_COMMENT:
					break;
				case SEBS_PARSE_XML_STATE_TAG_START:
					fdata->state =SEBS_PARSE_XML_STATE_CLOSE_TAG_START;
					break;
				case SEBS_PARSE_XML_STATE_TAG:
					fdata->state =SEBS_PARSE_XML_STATE_EXPECT_SELFCLOSE_TAG_END;
					break;

				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
				case SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH:
					fdata->state =SEBS_PARSE_XML_STATE_COMMENT;
					break;

				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					fdata->state =SEBS_PARSE_XML_STATE_CDATA;
					break;
				case SEBS_PARSE_XML_STATE_INNER:
					pdata->event =
							SEBS_PARSE_XML_EVENT_CONTENT_START;
					fdata->state =SEBS_PARSE_XML_STATE_INNER_CONTENT;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					break;
				default:
					pdata->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
				}
				break;
			case '-':
				switch (fdata->state)
				{
				case SEBS_PARSE_XML_STATE_TAG_START:
					if (fdata->tag_type
							== SEBS_PARSE_XML_TAG_TYPE_EXCLAMATION_MARK)
						fdata->state =SEBS_PARSE_XML_STATE_COMMENT_START_1ST_DASH;
					else
						pdata->event =
								SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
				case SEBS_PARSE_XML_STATE_COMMENT_START_1ST_DASH:
					DEBUG_PRINT_STR("COMMENT START");
					fdata->state =SEBS_PARSE_XML_STATE_COMMENT;
					break;
				case SEBS_PARSE_XML_STATE_COMMENT:
					fdata->state =SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH;
					break;
				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
					fdata->state =SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH;
					break;

				case SEBS_PARSE_XML_STATE_ATTRIBUTE_INSIDE_APOSTROPHE_ATTRIB:
					break;
				case SEBS_PARSE_XML_STATE_ATTRIBUTE_INSIDE_QUOTES_ATTRIB:
					break;
				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					fdata->state =SEBS_PARSE_XML_STATE_CDATA;
					break;
				case SEBS_PARSE_XML_STATE_INNER:
					pdata->event =
							SEBS_PARSE_XML_EVENT_CONTENT_START;
					fdata->state =SEBS_PARSE_XML_STATE_INNER_CONTENT;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					break;
				default:
					pdata->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
				}
				break;
			case '?':
				switch (fdata->state)
				{

				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
				case SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH:
					fdata->state =SEBS_PARSE_XML_STATE_COMMENT;
					break;
				case SEBS_PARSE_XML_STATE_COMMENT:
					break;
				case SEBS_PARSE_XML_STATE_TAG_START:
					fdata->tag_type = SEBS_PARSE_XML_TAG_TYPE_QUESTION_MARK;
					break;

				case SEBS_PARSE_XML_STATE_TAG:
					if (fdata->tag_type == SEBS_PARSE_XML_TAG_TYPE_QUESTION_MARK)
					{
						fdata->state =SEBS_PARSE_XML_STATE_QMTAG_EXPECT_CLOSE;
					}
					else
					{
						pdata->event =
								SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					}
					break;
				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					fdata->state =SEBS_PARSE_XML_STATE_CDATA;
					break;
				case SEBS_PARSE_XML_STATE_INNER:
					pdata->event =
							SEBS_PARSE_XML_EVENT_CONTENT_START;
					fdata->state =SEBS_PARSE_XML_STATE_INNER_CONTENT;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					break;
				default:
					pdata->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
				}
				break;
			case '!':
				switch (fdata->state)
				{
				case SEBS_PARSE_XML_STATE_COMMENT:
					break;
				case SEBS_PARSE_XML_STATE_TAG_START:
					fdata->tag_type = SEBS_PARSE_XML_TAG_TYPE_EXCLAMATION_MARK;
					break;

				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
				case SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH:
					fdata->state =SEBS_PARSE_XML_STATE_COMMENT;
					break;
				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					fdata->state =SEBS_PARSE_XML_STATE_CDATA;
					break;
				case SEBS_PARSE_XML_STATE_INNER:
					pdata->event =
							SEBS_PARSE_XML_EVENT_CONTENT_START;
					fdata->state =SEBS_PARSE_XML_STATE_INNER_CONTENT;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					break;
				default:
					pdata->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
					break;
				}
				break;
			case '=':
				switch (fdata->state)
				{
				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
				case SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH:
					fdata->state =SEBS_PARSE_XML_STATE_COMMENT;
					break;
				case SEBS_PARSE_XML_STATE_COMMENT:
					break;
				case SEBS_PARSE_XML_STATE_ATTRIBUTE_WAIT_EQUAL:
					fdata->state =SEBS_PARSE_XML_STATE_ATTRIBUTE_WAIT_VAL_BOUND;
					break;
				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					fdata->state =SEBS_PARSE_XML_STATE_CDATA;
					break;
				case SEBS_PARSE_XML_STATE_INNER:
					pdata->event =
							SEBS_PARSE_XML_EVENT_CONTENT_START;
					fdata->state =SEBS_PARSE_XML_STATE_INNER_CONTENT;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					break;
				default:
					pdata->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
					break;
				}
				break;
			case '\'': // '
				switch (fdata->state)
				{
				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
				case SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH:
					fdata->state =SEBS_PARSE_XML_STATE_COMMENT;
					break;
				case SEBS_PARSE_XML_STATE_COMMENT:
					break;
				case SEBS_PARSE_XML_STATE_ATTRIBUTE_WAIT_VAL_BOUND:
					fdata->state =SEBS_PARSE_XML_STATE_ATTRIBUTE_INSIDE_APOSTROPHE_ATTRIB;
					pdata->event=SEBS_PARSE_XML_EVENT_ATTRIBUTE_APOSTROPHE;
					break;
				case SEBS_PARSE_XML_STATE_ATTRIBUTE_INSIDE_APOSTROPHE_ATTRIB:
					fdata->state =SEBS_PARSE_XML_STATE_TAG;
					break;
				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					fdata->state =SEBS_PARSE_XML_STATE_CDATA;
					break;
				case SEBS_PARSE_XML_STATE_INNER:
					pdata->event =
							SEBS_PARSE_XML_EVENT_CONTENT_START;
					fdata->state =SEBS_PARSE_XML_STATE_INNER_CONTENT;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					break;
				default:
					pdata->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
					break;

				}
				break;

			case ']':
				switch (fdata->state)
				{
				case SEBS_PARSE_XML_STATE_CDATA:
					fdata->state =SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET;
					break;
				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
					fdata->state =SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET;
					break;
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET: //if ]]]
					break;
				case SEBS_PARSE_XML_STATE_INNER:
					pdata->event =
							SEBS_PARSE_XML_EVENT_CONTENT_START;
					fdata->state =SEBS_PARSE_XML_STATE_INNER_CONTENT;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					break;
				default:
					break;
				}
				break;

			case '"': // '
				switch (fdata->state)
				{
				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
				case SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH:
					fdata->state =SEBS_PARSE_XML_STATE_COMMENT;
					break;
				case SEBS_PARSE_XML_STATE_COMMENT:
					break;
				case SEBS_PARSE_XML_STATE_ATTRIBUTE_WAIT_VAL_BOUND:
					fdata->state =SEBS_PARSE_XML_STATE_ATTRIBUTE_INSIDE_QUOTES_ATTRIB;
					pdata->event=SEBS_PARSE_XML_EVENT_ATTRIBUTE_QUOTES;
					break;
				case SEBS_PARSE_XML_STATE_ATTRIBUTE_INSIDE_QUOTES_ATTRIB:
					fdata->state =SEBS_PARSE_XML_STATE_TAG;
					break;
				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					fdata->state =SEBS_PARSE_XML_STATE_CDATA;
					break;
				case SEBS_PARSE_XML_STATE_INNER:
					pdata->event =
							SEBS_PARSE_XML_EVENT_CONTENT_START;
					fdata->state =SEBS_PARSE_XML_STATE_INNER_CONTENT;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					break;
				default:
					pdata->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
					break;
				}
				break;

			default:
				switch (fdata->state)
				{
				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
				case SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH:
					fdata->state = SEBS_PARSE_XML_STATE_COMMENT;
					break;
				case SEBS_PARSE_XML_STATE_COMMENT:
					break;

				case SEBS_PARSE_XML_STATE_CDATA_START:
					fdata->substate =SEBS_PARSE_XML_SUBSTATE_CDATA_TAG_STRING;
					static const char* cdata_str="CDATA";
					SEBS_PARSE_SEEKSTRING_INIT(
							pdata,
							fdata->seekstring,
							&cdata_str,
							1, " []=\"\'/<>?!",
							true);
					break;
				case SEBS_PARSE_XML_STATE_TAG_START:
					fdata->substate =SEBS_PARSE_XML_SUBSTATE_TAG_ID;
					SEBS_PARSE_SEEKSTRING_INIT(
							pdata,
							fdata->seekstring,
							fdata->tag_strings,
							fdata->tag_strings_len, " []=\"\'/<>?!",
							true);
					break;

				case SEBS_PARSE_XML_STATE_CLOSE_TAG_START:
					fdata->substate =SEBS_PARSE_XML_SUBSTATE_TAG_ID;
					fdata->tag_type = SEBS_PARSE_XML_TAG_TYPE_CLOSE;
					SEBS_PARSE_SEEKSTRING_INIT(
							pdata,
							fdata->seekstring,
							fdata->tag_strings,
							fdata->tag_strings_len, " []=\"\'/<>?!",
							true);
					break;

				case SEBS_PARSE_XML_STATE_TAG: //A non empty space inside a tag means, that we have a attribute.
					fdata->substate =SEBS_PARSE_XML_SUBSTATE_ATTRIBUTE_ID;
					SEBS_PARSE_SEEKSTRING_INIT(
							pdata,
							fdata->seekstring,
							fdata->attribute_strings,
							fdata->attribute_strings_len,
							" []=\"\'/<>?!", true);
					break;
				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					fdata->state = SEBS_PARSE_XML_STATE_CDATA;
					break;
				case SEBS_PARSE_XML_STATE_INNER:
					pdata->event =
							SEBS_PARSE_XML_EVENT_CONTENT_START;
					fdata->state = SEBS_PARSE_XML_STATE_INNER_CONTENT;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					break;
				default:
					break;
				}
				break;
			}
			if (*pdata->len > 0
					&& pdata->event
							!= SEBS_PARSE_XML_EVENT_CONTENT_START)
			{
				++*pdata->buf;
				--*pdata->len;
			}
		}
	}
	return (SEBS_PARSE_RETURN_GO_AHEAD);
}
