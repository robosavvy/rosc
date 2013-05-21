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
	if(data->state==PARSE_XML_INIT)
	{
		data->depth=0;
		data->current_tag=XML_TAG_NONE;
		data->state=PARSE_XML_ROOT;
		data->sub_state=PARSE_XML_SUB_STATE_NONE;
	}
	while(*len>0 && data->parser_data->event == PARSE_EVENT_NONE)
	{


		//Are we inside a substate (awaiting "reply" from subfunction_state?)
		//If yes, in which one?
		switch(data->sub_state)
		{
		case PARSE_XML_SUB_TAG_ID:

			if(data->std_func_data.seekstring.result<0)
			{
				data->current_tag=XML_TAG_UNKNOWN;
			}
			else
			{
				data->current_tag=data->std_func_data.seekstring.result;
				data->parser_data->event=PARSE_EVENT_XML_TAG;
			}

			if(data->std_func_data.seekstring.result==XML_TAG_TYPE_CDATA)
			{
				data->state=PARSE_XML_CDATA;
			}
			else
			{
				data->state=PARSE_XML_TAG;
			}
			break;

		case PARSE_XML_SUB_ATTRIBUTE_ID:

			if(data->std_func_data.seekstring.result<0)
			{
				data->attribute=XML_ATTRIBUTE_UNKNOWN;
			}
			else
			{
				data->attribute=data->std_func_data.seekstring.result;
			}
			data->state=PARSE_XML_ATTRIBUTE_WAIT_EQUAL;
			break;

		case PARSE_XML_SUB_CDATA_TAG_STRING:

			if(data->std_func_data.seekstring.result!=XML_TAG_CDATA)
			{
				data->parser_data->event=PARSE_EVENT_ERROR_XML_MALFORMED;
			}
			else
			{
				data->state=PARSE_XML_CDATA_EXPECT_OPEN_BRACKET;
			}
			data->state=PARSE_XML_ATTRIBUTE_WAIT_EQUAL;
			break;
		default:
			break;
		}

		if(data->sub_state!=PARSE_XML_SUB_STATE_NONE)
		{
			data->sub_state=PARSE_XML_SUB_STATE_NONE;
		}
		else
		{
			switch(**buf)
			{
			//the whitespace stuff ...
			case '\t':
			case '\n':
			case ' ':
					switch(data->state)
					{
						case PARSE_XML_COMMENT_END_1ST_DASH:
						case PARSE_XML_COMMENT_END_2ND_DASH:
							data->state=PARSE_XML_COMMENT;
							break;

						case PARSE_XML_CDATA_FIRST_CLOSE_BRACKET:
						case PARSE_XML_CDATA_SECOND_CLOSE_BRACKET:
							data->state=PARSE_XML_CDATA;
							break;

						case PARSE_XML_CDATA_EXPECT_OPEN_BRACKET:
						case PARSE_XML_QMTAG_EXPECT_CLOSE:
						case PARSE_XML_TAG_START:
						case PARSE_XML_ATTRIBUTE_WAIT_EQUAL:
							data->parser_data->event=PARSE_EVENT_ERROR_XML_MALFORMED;
						break;
						case PARSE_XML_INNER_CONTENT:
							data->state=PARSE_XML_INNER;
							break;
						default:
							break;
					}
				break;

			case '[':
				switch(data->state)
				{
				case PARSE_XML_TAG_START:
					 data->state=PARSE_XML_CDATA_START;
					 break;
				case PARSE_XML_CDATA_EXPECT_OPEN_BRACKET:
					if(data->tag_type==XML_TAG_TYPE_EXCLAMATION_MARK)
					{
						data->state=PARSE_XML_CDATA;
					}
					else
					{
						data->parser_data->event=PARSE_EVENT_ERROR_XML_MALFORMED;
					}
					break;
				case PARSE_XML_COMMENT:
					break;
				case PARSE_XML_INNER:
					data->parser_data->event=PARSE_EVENT_XML_CONTENT_START;
					data->state=PARSE_XML_INNER_CONTENT;
					break;
				case PARSE_XML_INNER_CONTENT:
					break;
				default:
					data->parser_data->event=PARSE_EVENT_ERROR_XML_MALFORMED;
					break;
				}
				break;

			case '<':
					switch(data->state)
					{
						case PARSE_XML_COMMENT:
							break;

						case PARSE_XML_INNER_CONTENT:
						case PARSE_XML_INNER:
						case PARSE_XML_ROOT:
							data->tag_type=XML_TAG_TYPE_NORMAL;
							data->state=PARSE_XML_TAG_START;
							break;

						case PARSE_XML_CDATA_FIRST_CLOSE_BRACKET:
						case PARSE_XML_CDATA_SECOND_CLOSE_BRACKET:
							data->state=PARSE_XML_CDATA;
							break;


						case PARSE_XML_COMMENT_END_1ST_DASH:
						case PARSE_XML_COMMENT_END_2ND_DASH:

						default:
							data->parser_data->event=PARSE_EVENT_ERROR_XML_MALFORMED;
							break;

					}
				break;
			case '>':
					switch(data->state)
					{
					case PARSE_XML_COMMENT:
						break;
					case PARSE_XML_TAG:
						switch(data->tag_type)
						{
						case XML_TAG_TYPE_EXCLAMATION_MARK:
							break;
						case XML_TAG_TYPE_QUESTION_MARK:
							data->parser_data->event=PARSE_EVENT_ERROR_XML_MALFORMED;
						break;
						case XML_TAG_TYPE_NORMAL:
							if(data->depth!=SEBS_PARSE_XML_MAX_DEPTH)
							{
								data->depth++;
								data->tags[data->depth]=data->current_tag;
								data->parser_data->event=PARSE_EVENT_XML_INSIDE_TAG;
							}
							else
							{
								data->parser_data->event=PARSE_EVENT_ERROR_XML_DEPTH;
								DEBUG_PRINT_STR("XML ERROR! TOO DEEP!");
							}
							break;
						case XML_TAG_TYPE_CLOSE:
							if(data->tags[data->depth]==data->current_tag)
							{
								if(data->depth>0)
								data->depth--;
								else
								data->parser_data->event=PARSE_EVENT_ERROR_XML_DEPTH;
							}
							else
							{
								DEBUG_PRINT_STR("XML ERROR! XML INVALID!"); //TODO Error Handling
								data->parser_data->event=PARSE_EVENT_ERROR_XML_MALFORMED;
							}
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
							data->state=PARSE_XML_COMMENT;
						break;
					case PARSE_XML_COMMENT_END_2ND_DASH:
							DEBUG_PRINT_STR("COMMENT END");
							if(data->depth==0)
								data->state=PARSE_XML_ROOT;
							else
								data->state=PARSE_XML_INNER;
						break;
					case PARSE_XML_CDATA:
						break;

					case PARSE_XML_CDATA_FIRST_CLOSE_BRACKET:
					case PARSE_XML_CDATA_SECOND_CLOSE_BRACKET:
						data->state=PARSE_XML_CDATA;
						break;
					default:
						data->parser_data->event=PARSE_EVENT_ERROR_XML_MALFORMED;
						break;
					}

					if(data->state!=PARSE_XML_COMMENT &&
					   data->state!=PARSE_XML_CDATA)
					{
						data->current_tag=XML_TAG_NONE;
						if(data->depth==0)
						{
							data->state=PARSE_XML_ROOT;
						}
						else if(data->depth>0)
						{
							data->state=PARSE_XML_INNER;
						}
						else
						{
							DEBUG_PRINT_STR("NEGATIVE LVL!!! MALFORMED!");
							data->parser_data->event=PARSE_EVENT_ERROR_XML_MALFORMED;
						}
					}

				break;
			case '/':
				switch(data->state)
				{
					case PARSE_XML_COMMENT:
						break;
					case PARSE_XML_TAG_START:
						data->state=PARSE_XML_CLOSE_TAG_START;
					break;
					case PARSE_XML_TAG:
						data->state=PARSE_XML_EXPECT_SELFCLOSE_TAG_END;
					break;

					case PARSE_XML_COMMENT_END_1ST_DASH:
					case PARSE_XML_COMMENT_END_2ND_DASH:
							data->state=PARSE_XML_COMMENT;
						break;

					case PARSE_XML_CDATA_FIRST_CLOSE_BRACKET:
					case PARSE_XML_CDATA_SECOND_CLOSE_BRACKET:
						data->state=PARSE_XML_CDATA;
						break;
					case PARSE_XML_INNER:
						data->parser_data->event=PARSE_EVENT_XML_CONTENT_START;
						data->state=PARSE_XML_INNER_CONTENT;
						break;
					case PARSE_XML_INNER_CONTENT:
						break;
					default:
						data->parser_data->event=PARSE_EVENT_ERROR_XML_MALFORMED;
					break;
				}
				break;
			case '-':
				switch(data->state)
				{
					case PARSE_XML_TAG_START:
						if(data->tag_type==XML_TAG_TYPE_EXCLAMATION_MARK)
						data->state=PARSE_XML_COMMENT_START_1ST_DASH;
						else
							data->parser_data->event=PARSE_EVENT_ERROR_XML_MALFORMED;
						break;
					case PARSE_XML_COMMENT_START_1ST_DASH:
						DEBUG_PRINT_STR("COMMENT START");
						data->state=PARSE_XML_COMMENT;
						break;
					case PARSE_XML_COMMENT:
							data->state=PARSE_XML_COMMENT_END_1ST_DASH;
						break;
					case PARSE_XML_COMMENT_END_1ST_DASH:
							data->state=PARSE_XML_COMMENT_END_2ND_DASH;
						break;

					case PARSE_XML_ATTRIBUTE_INSIDE_APOSTROPHE_ATTRIB:
						break;
					case PARSE_XML_ATTRIBUTE_INSIDE_QUOTES_ATTRIB:
						break;
					case PARSE_XML_CDATA_FIRST_CLOSE_BRACKET:
					case PARSE_XML_CDATA_SECOND_CLOSE_BRACKET:
						data->state=PARSE_XML_CDATA;
						break;
					case PARSE_XML_INNER:
						data->parser_data->event=PARSE_EVENT_XML_CONTENT_START;
						data->state=PARSE_XML_INNER_CONTENT;
						break;
					case PARSE_XML_INNER_CONTENT:
						break;
					default:
						data->parser_data->event=PARSE_EVENT_ERROR_XML_MALFORMED;
						break;
				}
				break;
			case '?':
				switch(data->state)
				{

					case PARSE_XML_COMMENT_END_1ST_DASH:
					case PARSE_XML_COMMENT_END_2ND_DASH:
						data->state=PARSE_XML_COMMENT;
					break;
					case PARSE_XML_COMMENT:
					break;
					case PARSE_XML_TAG_START:
						data->tag_type=XML_TAG_TYPE_QUESTION_MARK;
					break;

					case PARSE_XML_TAG:
						if(data->tag_type==XML_TAG_TYPE_QUESTION_MARK)
						{
							data->state=PARSE_XML_QMTAG_EXPECT_CLOSE;
						}
						else
						{
							data->parser_data->event=PARSE_EVENT_ERROR_XML_MALFORMED;
						}
						break;
					case PARSE_XML_CDATA_FIRST_CLOSE_BRACKET:
					case PARSE_XML_CDATA_SECOND_CLOSE_BRACKET:
						data->state=PARSE_XML_CDATA;
						break;
					case PARSE_XML_INNER:
						data->parser_data->event=PARSE_EVENT_XML_CONTENT_START;
						data->state=PARSE_XML_INNER_CONTENT;
						break;
					case PARSE_XML_INNER_CONTENT:
						break;
					default:
						data->parser_data->event=PARSE_EVENT_ERROR_XML_MALFORMED;
					break;
				}
				break;
			case '!':
				switch(data->state)
				{
					case PARSE_XML_COMMENT:
					break;
					case PARSE_XML_TAG_START:
						data->tag_type=XML_TAG_TYPE_EXCLAMATION_MARK;
					break;

					case PARSE_XML_COMMENT_END_1ST_DASH:
					case PARSE_XML_COMMENT_END_2ND_DASH:
							data->state=PARSE_XML_COMMENT;
						break;
					case PARSE_XML_CDATA_FIRST_CLOSE_BRACKET:
					case PARSE_XML_CDATA_SECOND_CLOSE_BRACKET:
						data->state=PARSE_XML_CDATA;
						break;
					case PARSE_XML_INNER:
						data->parser_data->event=PARSE_EVENT_XML_CONTENT_START;
						data->state=PARSE_XML_INNER_CONTENT;
						break;
					case PARSE_XML_INNER_CONTENT:
						break;
					default:
						data->parser_data->event=PARSE_EVENT_ERROR_XML_MALFORMED;
						break;
					break;
				}
				break;
			case '=':
				switch(data->state)
				{
					case PARSE_XML_COMMENT_END_1ST_DASH:
					case PARSE_XML_COMMENT_END_2ND_DASH:
						data->state=PARSE_XML_COMMENT;
					break;
					case PARSE_XML_COMMENT:
					break;
					case PARSE_XML_ATTRIBUTE_WAIT_EQUAL:
						data->state=PARSE_XML_ATTRIBUTE_WAIT_VAL_BOUND;
						break;
					case PARSE_XML_CDATA_FIRST_CLOSE_BRACKET:
					case PARSE_XML_CDATA_SECOND_CLOSE_BRACKET:
						data->state=PARSE_XML_CDATA;
						break;
					case PARSE_XML_INNER:
						data->parser_data->event=PARSE_EVENT_XML_CONTENT_START;
						data->state=PARSE_XML_INNER_CONTENT;
						break;
					case PARSE_XML_INNER_CONTENT:
						break;
					default:
						data->parser_data->event=PARSE_EVENT_ERROR_XML_MALFORMED;
						break;
					break;
				}
				break;
			case '\'': // '
				switch(data->state)
				{
					case PARSE_XML_COMMENT_END_1ST_DASH:
					case PARSE_XML_COMMENT_END_2ND_DASH:
						data->state=PARSE_XML_COMMENT;
					break;
					case PARSE_XML_COMMENT:
					break;
					case PARSE_XML_ATTRIBUTE_WAIT_VAL_BOUND:
						data->state=PARSE_XML_ATTRIBUTE_INSIDE_APOSTROPHE_ATTRIB;
						break;
					case PARSE_XML_ATTRIBUTE_INSIDE_APOSTROPHE_ATTRIB:
						data->state=PARSE_XML_TAG;
						break;
					case PARSE_XML_CDATA_FIRST_CLOSE_BRACKET:
					case PARSE_XML_CDATA_SECOND_CLOSE_BRACKET:
						data->state=PARSE_XML_CDATA;
						break;
					case PARSE_XML_INNER:
						data->parser_data->event=PARSE_EVENT_XML_CONTENT_START;
						data->state=PARSE_XML_INNER_CONTENT;
						break;
					case PARSE_XML_INNER_CONTENT:
						break;
					default:
						data->parser_data->event=PARSE_EVENT_ERROR_XML_MALFORMED;
						break;
					break;

				}
				break;

			case ']':
				switch(data->state)
				{
				case PARSE_XML_CDATA:
					data->state=PARSE_XML_CDATA_FIRST_CLOSE_BRACKET;
					break;
				case PARSE_XML_CDATA_FIRST_CLOSE_BRACKET:
					data->state=PARSE_XML_CDATA_SECOND_CLOSE_BRACKET;
					break;
				case PARSE_XML_CDATA_SECOND_CLOSE_BRACKET: //if ]]]
					break;
				case PARSE_XML_INNER:
					data->parser_data->event=PARSE_EVENT_XML_CONTENT_START;
					data->state=PARSE_XML_INNER_CONTENT;
					break;
				case PARSE_XML_INNER_CONTENT:
					break;
				default:
					break;
				}
				break;

			case '"': // '
				switch(data->state)
				{
					case PARSE_XML_COMMENT_END_1ST_DASH:
					case PARSE_XML_COMMENT_END_2ND_DASH:
						data->state=PARSE_XML_COMMENT;
					break;
					case PARSE_XML_COMMENT:
					break;
					case PARSE_XML_ATTRIBUTE_WAIT_VAL_BOUND:
						data->state=PARSE_XML_ATTRIBUTE_INSIDE_QUOTES_ATTRIB;
						break;
					case PARSE_XML_ATTRIBUTE_INSIDE_QUOTES_ATTRIB:
						data->state=PARSE_XML_TAG;
						break;
					case PARSE_XML_CDATA_FIRST_CLOSE_BRACKET:
					case PARSE_XML_CDATA_SECOND_CLOSE_BRACKET:
						data->state=PARSE_XML_CDATA;
						break;
					case PARSE_XML_INNER:
						data->parser_data->event=PARSE_EVENT_XML_CONTENT_START;
						data->state=PARSE_XML_INNER_CONTENT;
						break;
					case PARSE_XML_INNER_CONTENT:
						break;
					default:
						data->parser_data->event=PARSE_EVENT_ERROR_XML_MALFORMED;
						break;
					break;
				}
				break;

			default:
				switch(data->state)
				{
				case PARSE_XML_COMMENT_END_1ST_DASH:
				case PARSE_XML_COMMENT_END_2ND_DASH:
						data->state=PARSE_XML_COMMENT;
					break;
				case PARSE_XML_COMMENT:
					break;

				case PARSE_XML_CDATA_START:
					data->sub_state=PARSE_XML_SUB_CDATA_TAG_STRING;
					PARSE_SUBMODE_INIT_SEEKSTRING(data->parser_data->current_parser.parser_function,(&data->std_func_data.seekstring),rpc_xml_tag_strings,RPC_XML_TAG_STRINGS_LEN," []=\"\'/<>?!",true);
					break;
				case PARSE_XML_TAG_START:
					 data->sub_state=PARSE_XML_SUB_TAG_ID;
					 PARSE_SUBMODE_INIT_SEEKSTRING(data->parser_data->current_parser.parser_function,(&data->std_func_data.seekstring),rpc_xml_tag_strings,RPC_XML_TAG_STRINGS_LEN," []=\"\'/<>?!",true);
				break;
				case PARSE_XML_CLOSE_TAG_START:
					 data->sub_state=PARSE_XML_SUB_TAG_ID;
					 data->tag_type=XML_TAG_TYPE_CLOSE;
					 PARSE_SUBMODE_INIT_SEEKSTRING(data->parser_data->current_parser.parser_function,(&data->std_func_data.seekstring),rpc_xml_tag_strings,RPC_XML_TAG_STRINGS_LEN," []=\"\'/<>?!",true);
				break;

				case PARSE_XML_TAG: //A non empty space inside a tag means, that we have a attribute.
					 data->sub_state=PARSE_XML_SUB_ATTRIBUTE_ID;
					 PARSE_SUBMODE_INIT_SEEKSTRING(data->parser_data->current_parser.parser_function,(&data->std_func_data.seekstring),rpc_xml_attribute_strings,RPC_XML_ATTRIBUTE_STRINGS_LEN," []=\"\'/<>?!",true);
					 break;
				case PARSE_XML_CDATA_FIRST_CLOSE_BRACKET:
				case PARSE_XML_CDATA_SECOND_CLOSE_BRACKET:
					data->state=PARSE_XML_CDATA;
					break;
				case PARSE_XML_INNER:
					data->parser_data->event=PARSE_EVENT_XML_CONTENT_START;
					data->state=PARSE_XML_INNER_CONTENT;
					break;
				case PARSE_XML_INNER_CONTENT:
					break;
				default:
					break;
				}
				break;
			}
			if(*len>0 && data->parser_data->event!=PARSE_EVENT_XML_CONTENT_START)
			{
				++*buf;
				--*len;
			}
		}
	}
}
