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

bool sebs_parse_xml(char **buf, int32_t *len, sebs_parse_xml_data_t *data)
{
	if (data->state ==SEBS_PARSE_XML_STATE_INIT)
	{
		data->depth = 0;
		data->current_tag = SEBS_PARSE_XML_TAG_NONE;
		data->state = SEBS_PARSE_XML_STATE_ROOT;
		data->substate =SEBS_PARSE_XML_SUBSTATE_NONE;
	}
	while (*len > 0 && data->parser_data->event == SEBS_PARSE_EVENT_NONE)
	{

		//Are we inside a substate (awaiting "reply" from subfunction_state?)
		//If yes, in which one?
		switch (data->substate)
		{
		case SEBS_PARSE_XML_SUBSTATE_TAG_ID:

			if (data->std_func_data.seekstring.result < 0)
			{
				data->current_tag = SEBS_PARSE_XML_TAG_UNKNOWN;
			}
			else
			{
				data->current_tag = data->std_func_data.seekstring.result;
				data->parser_data->event = SEBS_PARSE_XML_EVENT_TAG;
			}

			if (data->std_func_data.seekstring.result
					== SEBS_PARSE_XML_TAG_TYPE_CDATA)
			{
				data->state =SEBS_PARSE_XML_STATE_CDATA;
			}
			else
			{
				data->state =SEBS_PARSE_XML_STATE_TAG;
			}
			break;

		case SEBS_PARSE_XML_SUBSTATE_ATTRIBUTE_ID:

			if (data->std_func_data.seekstring.result < 0)
			{
				data->attribute = SEBS_PARSE_XML_ATTRIBUTE_UNKNOWN;
			}
			else
			{
				data->attribute = data->std_func_data.seekstring.result;
			}
			data->state =SEBS_PARSE_XML_STATE_ATTRIBUTE_WAIT_EQUAL;
			break;

		case SEBS_PARSE_XML_SUBSTATE_CDATA_TAG_STRING:

			if (data->std_func_data.seekstring.result
					!= SEBS_PARSE_XML_TAG_CDATA)
			{
				data->parser_data->event = SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
			}
			else
			{
				data->state =SEBS_PARSE_XML_STATE_CDATA_EXPECT_OPEN_BRACKET;
			}
			data->state =SEBS_PARSE_XML_STATE_ATTRIBUTE_WAIT_EQUAL;
			break;
		default:
			break;
		}

		if (data->substate !=SEBS_PARSE_XML_SUBSTATE_NONE)
		{
			data->substate =SEBS_PARSE_XML_SUBSTATE_NONE;
		}
		else
		{
			switch (**buf)
			{
			//the whitespace stuff ...
			case '\t':
			case '\n':
			case ' ':
				switch (data->state)
				{
				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
				case SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH:
					data->state =SEBS_PARSE_XML_STATE_COMMENT;
					break;

				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					data->state =SEBS_PARSE_XML_STATE_CDATA;
					break;

				case SEBS_PARSE_XML_STATE_CDATA_EXPECT_OPEN_BRACKET:
				case SEBS_PARSE_XML_STATE_QMTAG_EXPECT_CLOSE:
				case SEBS_PARSE_XML_STATE_TAG_START:
				case SEBS_PARSE_XML_STATE_ATTRIBUTE_WAIT_EQUAL:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					data->state =SEBS_PARSE_XML_STATE_INNER;
					break;
				default:
					break;
				}
				break;

			case '[':
				switch (data->state)
				{
				case SEBS_PARSE_XML_STATE_TAG_START:
					data->state =SEBS_PARSE_XML_STATE_CDATA_START;
					break;
				case SEBS_PARSE_XML_STATE_CDATA_EXPECT_OPEN_BRACKET:
					if (data->tag_type
							== SEBS_PARSE_XML_TAG_TYPE_EXCLAMATION_MARK)
					{
						data->state =SEBS_PARSE_XML_STATE_CDATA;
					}
					else
					{
						data->parser_data->event =
								SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					}
					break;
				case SEBS_PARSE_XML_STATE_COMMENT:
					break;
				case SEBS_PARSE_XML_STATE_INNER:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_CONTENT_START;
					data->state =SEBS_PARSE_XML_STATE_INNER_CONTENT;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					break;
				default:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
				}
				break;

			case '<':
				switch (data->state)
				{
				case SEBS_PARSE_XML_STATE_COMMENT:
					break;

				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
				case SEBS_PARSE_XML_STATE_INNER:
				case SEBS_PARSE_XML_STATE_ROOT:
					data->tag_type = SEBS_PARSE_XML_TAG_TYPE_NORMAL;
					data->state = SEBS_PARSE_XML_STATE_TAG_START;
					break;

				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					data->state =SEBS_PARSE_XML_STATE_CDATA;
					break;

				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
				case SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH:

				default:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;

				}
				break;
			case '>':
				switch (data->state)
				{
				case SEBS_PARSE_XML_STATE_COMMENT:
					break;
				case SEBS_PARSE_XML_STATE_TAG:
					switch (data->tag_type)
					{
					case SEBS_PARSE_XML_TAG_TYPE_EXCLAMATION_MARK:
						break;
					case SEBS_PARSE_XML_TAG_TYPE_QUESTION_MARK:
						data->parser_data->event =
								SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
						break;
					case SEBS_PARSE_XML_TAG_TYPE_NORMAL:
						if (data->depth != SEBS_PARSE_XML_MAX_DEPTH)
						{
							data->depth++;
							data->tags[data->depth] = data->current_tag;
							data->parser_data->event =
									SEBS_PARSE_XML_EVENT_INSIDE_TAG;
						}
						else
						{
							data->parser_data->event =
									SEBS_PARSE_XML_EVENT_ERROR_DEPTH;
							DEBUG_PRINT_STR("XML ERROR! TOO DEEP!");
						}
						break;
					case SEBS_PARSE_XML_TAG_TYPE_CLOSE:
						if (data->tags[data->depth] == data->current_tag)
						{
							if (data->depth > 0)
								data->depth--;
							else
								data->parser_data->event =
										SEBS_PARSE_XML_EVENT_ERROR_DEPTH;
						}
						else
						{
							DEBUG_PRINT_STR("XML ERROR! XML INVALID!"); //TODO Error Handling
							data->parser_data->event =
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
					data->state =SEBS_PARSE_XML_STATE_COMMENT;
					break;
				case SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH:
					DEBUG_PRINT_STR("COMMENT END");
					if (data->depth == 0)
						data->state =SEBS_PARSE_XML_STATE_ROOT;
					else
						data->state =SEBS_PARSE_XML_STATE_INNER;
					break;
				case SEBS_PARSE_XML_STATE_CDATA:
					break;

				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					data->state =SEBS_PARSE_XML_STATE_CDATA;
					break;
				default:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
				}

				if (data->state !=SEBS_PARSE_XML_STATE_COMMENT
						&& data->state !=SEBS_PARSE_XML_STATE_CDATA)
				{
					data->current_tag = SEBS_PARSE_XML_TAG_NONE;
					if (data->depth == 0)
					{
						data->state =SEBS_PARSE_XML_STATE_ROOT;
					}
					else if (data->depth > 0)
					{
						data->state =SEBS_PARSE_XML_STATE_INNER;
					}
					else
					{
						DEBUG_PRINT_STR("NEGATIVE LVL!!! MALFORMED!");
						data->parser_data->event =
								SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					}
				}

				break;
			case '/':
				switch (data->state)
				{
				case SEBS_PARSE_XML_STATE_COMMENT:
					break;
				case SEBS_PARSE_XML_STATE_TAG_START:
					data->state =SEBS_PARSE_XML_STATE_CLOSE_TAG_START;
					break;
				case SEBS_PARSE_XML_STATE_TAG:
					data->state =SEBS_PARSE_XML_STATE_EXPECT_SELFCLOSE_TAG_END;
					break;

				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
				case SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH:
					data->state =SEBS_PARSE_XML_STATE_COMMENT;
					break;

				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					data->state =SEBS_PARSE_XML_STATE_CDATA;
					break;
				case SEBS_PARSE_XML_STATE_INNER:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_CONTENT_START;
					data->state =SEBS_PARSE_XML_STATE_INNER_CONTENT;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					break;
				default:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
				}
				break;
			case '-':
				switch (data->state)
				{
				case SEBS_PARSE_XML_STATE_TAG_START:
					if (data->tag_type
							== SEBS_PARSE_XML_TAG_TYPE_EXCLAMATION_MARK)
						data->state =SEBS_PARSE_XML_STATE_COMMENT_START_1ST_DASH;
					else
						data->parser_data->event =
								SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
				case SEBS_PARSE_XML_STATE_COMMENT_START_1ST_DASH:
					DEBUG_PRINT_STR("COMMENT START");
					data->state =SEBS_PARSE_XML_STATE_COMMENT;
					break;
				case SEBS_PARSE_XML_STATE_COMMENT:
					data->state =SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH;
					break;
				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
					data->state =SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH;
					break;

				case SEBS_PARSE_XML_STATE_ATTRIBUTE_INSIDE_APOSTROPHE_ATTRIB:
					break;
				case SEBS_PARSE_XML_STATE_ATTRIBUTE_INSIDE_QUOTES_ATTRIB:
					break;
				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					data->state =SEBS_PARSE_XML_STATE_CDATA;
					break;
				case SEBS_PARSE_XML_STATE_INNER:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_CONTENT_START;
					data->state =SEBS_PARSE_XML_STATE_INNER_CONTENT;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					break;
				default:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
				}
				break;
			case '?':
				switch (data->state)
				{

				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
				case SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH:
					data->state =SEBS_PARSE_XML_STATE_COMMENT;
					break;
				case SEBS_PARSE_XML_STATE_COMMENT:
					break;
				case SEBS_PARSE_XML_STATE_TAG_START:
					data->tag_type = SEBS_PARSE_XML_TAG_TYPE_QUESTION_MARK;
					break;

				case SEBS_PARSE_XML_STATE_TAG:
					if (data->tag_type == SEBS_PARSE_XML_TAG_TYPE_QUESTION_MARK)
					{
						data->state =SEBS_PARSE_XML_STATE_QMTAG_EXPECT_CLOSE;
					}
					else
					{
						data->parser_data->event =
								SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					}
					break;
				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					data->state =SEBS_PARSE_XML_STATE_CDATA;
					break;
				case SEBS_PARSE_XML_STATE_INNER:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_CONTENT_START;
					data->state =SEBS_PARSE_XML_STATE_INNER_CONTENT;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					break;
				default:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
				}
				break;
			case '!':
				switch (data->state)
				{
				case SEBS_PARSE_XML_STATE_COMMENT:
					break;
				case SEBS_PARSE_XML_STATE_TAG_START:
					data->tag_type = SEBS_PARSE_XML_TAG_TYPE_EXCLAMATION_MARK;
					break;

				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
				case SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH:
					data->state =SEBS_PARSE_XML_STATE_COMMENT;
					break;
				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					data->state =SEBS_PARSE_XML_STATE_CDATA;
					break;
				case SEBS_PARSE_XML_STATE_INNER:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_CONTENT_START;
					data->state =SEBS_PARSE_XML_STATE_INNER_CONTENT;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					break;
				default:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
					break;
				}
				break;
			case '=':
				switch (data->state)
				{
				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
				case SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH:
					data->state =SEBS_PARSE_XML_STATE_COMMENT;
					break;
				case SEBS_PARSE_XML_STATE_COMMENT:
					break;
				case SEBS_PARSE_XML_STATE_ATTRIBUTE_WAIT_EQUAL:
					data->state =SEBS_PARSE_XML_STATE_ATTRIBUTE_WAIT_VAL_BOUND;
					break;
				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					data->state =SEBS_PARSE_XML_STATE_CDATA;
					break;
				case SEBS_PARSE_XML_STATE_INNER:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_CONTENT_START;
					data->state =SEBS_PARSE_XML_STATE_INNER_CONTENT;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					break;
				default:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
					break;
				}
				break;
			case '\'': // '
				switch (data->state)
				{
				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
				case SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH:
					data->state =SEBS_PARSE_XML_STATE_COMMENT;
					break;
				case SEBS_PARSE_XML_STATE_COMMENT:
					break;
				case SEBS_PARSE_XML_STATE_ATTRIBUTE_WAIT_VAL_BOUND:
					data->state =SEBS_PARSE_XML_STATE_ATTRIBUTE_INSIDE_APOSTROPHE_ATTRIB;
					break;
				case SEBS_PARSE_XML_STATE_ATTRIBUTE_INSIDE_APOSTROPHE_ATTRIB:
					data->state =SEBS_PARSE_XML_STATE_TAG;
					break;
				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					data->state =SEBS_PARSE_XML_STATE_CDATA;
					break;
				case SEBS_PARSE_XML_STATE_INNER:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_CONTENT_START;
					data->state =SEBS_PARSE_XML_STATE_INNER_CONTENT;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					break;
				default:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
					break;

				}
				break;

			case ']':
				switch (data->state)
				{
				case SEBS_PARSE_XML_STATE_CDATA:
					data->state =SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET;
					break;
				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
					data->state =SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET;
					break;
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET: //if ]]]
					break;
				case SEBS_PARSE_XML_STATE_INNER:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_CONTENT_START;
					data->state =SEBS_PARSE_XML_STATE_INNER_CONTENT;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					break;
				default:
					break;
				}
				break;

			case '"': // '
				switch (data->state)
				{
				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
				case SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH:
					data->state =SEBS_PARSE_XML_STATE_COMMENT;
					break;
				case SEBS_PARSE_XML_STATE_COMMENT:
					break;
				case SEBS_PARSE_XML_STATE_ATTRIBUTE_WAIT_VAL_BOUND:
					data->state =SEBS_PARSE_XML_STATE_ATTRIBUTE_INSIDE_QUOTES_ATTRIB;
					break;
				case SEBS_PARSE_XML_STATE_ATTRIBUTE_INSIDE_QUOTES_ATTRIB:
					data->state =SEBS_PARSE_XML_STATE_TAG;
					break;
				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					data->state =SEBS_PARSE_XML_STATE_CDATA;
					break;
				case SEBS_PARSE_XML_STATE_INNER:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_CONTENT_START;
					data->state =SEBS_PARSE_XML_STATE_INNER_CONTENT;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					break;
				default:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_ERROR_MALFORMED;
					break;
					break;
				}
				break;

			default:
				switch (data->state)
				{
				case SEBS_PARSE_XML_STATE_COMMENT_END_1ST_DASH:
				case SEBS_PARSE_XML_STATE_COMMENT_END_2ND_DASH:
					data->state = SEBS_PARSE_XML_STATE_COMMENT;
					break;
				case SEBS_PARSE_XML_STATE_COMMENT:
					break;

				case SEBS_PARSE_XML_STATE_CDATA_START:
					data->substate =SEBS_PARSE_XML_SUBSTATE_CDATA_TAG_STRING;
					SEBS_PARSE_SEEKSTRING_INIT(
							data->parser_data->next_parser,
							data->std_func_data.seekstring,
							SEBS_PARSE_KNOWN_TAGS_ARRAY,
							SEBS_PARSE_KNOWN_TAGS_ARRAY_LEN, " []=\"\'/<>?!",
							true);
					break;
				case SEBS_PARSE_XML_STATE_TAG_START:
					data->substate =SEBS_PARSE_XML_SUBSTATE_TAG_ID;
					SEBS_PARSE_SEEKSTRING_INIT(
							data->parser_data->next_parser,
							data->std_func_data.seekstring,
							SEBS_PARSE_KNOWN_TAGS_ARRAY,
							SEBS_PARSE_KNOWN_TAGS_ARRAY_LEN, " []=\"\'/<>?!",
							true);
					break;

				case SEBS_PARSE_XML_STATE_CLOSE_TAG_START:
					data->substate =SEBS_PARSE_XML_SUBSTATE_TAG_ID;
					data->tag_type = SEBS_PARSE_XML_TAG_TYPE_CLOSE;
					SEBS_PARSE_SEEKSTRING_INIT(
							data->parser_data->next_parser,
							data->std_func_data.seekstring,
							SEBS_PARSE_KNOWN_TAGS_ARRAY,
							SEBS_PARSE_KNOWN_TAGS_ARRAY_LEN, " []=\"\'/<>?!",
							true);
					break;

				case SEBS_PARSE_XML_STATE_TAG: //A non empty space inside a tag means, that we have a attribute.
					data->substate =SEBS_PARSE_XML_SUBSTATE_ATTRIBUTE_ID;
					SEBS_PARSE_SEEKSTRING_INIT(
							data->parser_data->next_parser,
							data->std_func_data.seekstring,
							SEBS_PARSE_KNOWN_ATTRIBUTES_ARRAY,
							SEBS_PARSE_KNOWN_ATTRIBUTES_ARRAY_LEN,
							" []=\"\'/<>?!", true);
					break;
				case SEBS_PARSE_XML_STATE_CDATA_FIRST_CLOSE_BRACKET:
				case SEBS_PARSE_XML_STATE_CDATA_SECOND_CLOSE_BRACKET:
					data->state = SEBS_PARSE_XML_STATE_CDATA;
					break;
				case SEBS_PARSE_XML_STATE_INNER:
					data->parser_data->event =
							SEBS_PARSE_XML_EVENT_CONTENT_START;
					data->state = SEBS_PARSE_XML_STATE_INNER_CONTENT;
					break;
				case SEBS_PARSE_XML_STATE_INNER_CONTENT:
					break;
				default:
					break;
				}
				break;
			}
			if (*len > 0
					&& data->parser_data->event
							!= SEBS_PARSE_XML_EVENT_CONTENT_START)
			{
				++*buf;
				--*len;
			}
		}
	}
	return false;
}
