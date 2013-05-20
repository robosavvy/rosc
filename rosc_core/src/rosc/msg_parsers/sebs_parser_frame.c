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
 *   sebs_parser_frame.c created by Christian Holl
 */

#include <rosc/msg_parsers/sebs_parser_frame.h>

void sebs_parser_frame(char *buf, int32_t len, sebs_parser_data_t* data)
{
	/*
	 * What if len < 0?
	 * Network functions return len=-1 when connection was terminated for example.
	 * in that case the handler function is called.
	 */
	if(len<0)data->event=SEBS_PARSE_FW_EVENT_LEN_SMALLER_ZERO;

	/*
	 * Handling the parser input
	 */
	do
	{
		if(data->event==SEBS_PARSE_FW_EVENT_NONE)
		{
			if(data->current_parser.parser_function)//current_function is not zero
			{
				//call parser
				if(data->current_parser.parser_function(&buf, &len, data->current_parser.parser_data))
				{
					if(data->return_to_handler) //was current function called by handler?
					{
						data->event=SEBS_PARSE_FW_EVENT_HANDLER_CALL_FUNCTION_END;
					}
					else
					{
						data->current_parser.parser_function=data->next_parser.parser_function;
						data->current_parser.parser_data=data->next_parser.parser_data;
					}
				}
			}
			else //current function is zero -> Event Init
			{
				data->event=SEBS_PARSE_FW_EVENT_INIT;
			}
		}
		else //call handler
		{
			data->handler_function(data);
			data->event=SEBS_PARSE_FW_EVENT_NONE;
		}
	}
	while(len>0);
}
