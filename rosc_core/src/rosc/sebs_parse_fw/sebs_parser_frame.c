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

#include <rosc/sebs_parse_fw/sebs_parser_frame.h>


void sebs_parser_frame(uint8_t *buf, int32_t len, sebs_parser_data_t* data)
{

	//pointing to current length and buffer for the handler
	data->len=&len;
	data->buf=&buf;


	/*
	 * What if len < 0?
	 * Network functions return len=-1 when connection was terminated for example.
	 * in that case the handler function is called.
	 */
	if (len <= 0)
	{
		data->event = SEBS_PARSE_EVENT_LEN_EQUAL_SMALLER_ZERO;
		data->call_len=0;
	}
	else
	{
		data->call_len=len;
		if(data->security_len != 0)
		{
			if((data->overall_len+data->call_len) > data->security_len)
			{
				data->event=SEBS_PARSE_EVENT_MESSAGE_SECURITY_OVER_SIZE;
			}
		}
	}

	/*
	 * Handling the parser input
	 */
	bool finish_call=false;
	do
	{

		sebs_parse_function_t function;
		bool switch_functions=false;
		bool handler=false;
		if (data->event == SEBS_PARSE_EVENT_NONE && !data->handler_init)
		{
			function=data->current_parser.parser_function;
		}
		else
		{
			function=data->handler_function;
			handler=true;
		}

		//Call the current function
		sebs_parse_return_t result=function(data);

		//When the current function was the handler, reset event
		if(handler)
			data->event=SEBS_PARSE_EVENT_NONE;

		//Reset finish call in parser struct
		finish_call=data->finish_call=false;

		switch(result)
		{
		case SEBS_PARSE_RETURN_FINISHED:
			if(data->return_to_handler)
			{
				data->event=SEBS_PARSE_EVENT_HANDLER_CALL_FUNCTION_END;
				data->return_to_handler = false;
			}
			finish_call=data->finish_call=true;
			switch_functions=true;
		break;

		case SEBS_PARSE_RETURN_INIT:
			data->function_init=true;
			switch_functions=true;
			if(handler)
				data->return_to_handler=true;
		break;

		case SEBS_PARSE_RETURN_INIT_ADV:
			data->function_init=true;
			switch_functions=true;
			break;

		case SEBS_PARSE_RETURN_GO_AHEAD:
			break;
		}
		if(switch_functions)
		{
			//Switch current parser call and next parser call
			sebs_parser_call_t store;
			store.parser_function = data->next_parser.parser_function;
			store.parser_data = data->next_parser.parser_data;

			data->next_parser.parser_function =
					data->current_parser.parser_function;
			data->next_parser.parser_data =
					data->current_parser.parser_data;

			data->current_parser.parser_function = store.parser_function;
			data->current_parser.parser_data = store.parser_data;
		}

	} while (len > 0 || finish_call || data->event!=SEBS_PARSE_EVENT_NONE);
		data->overall_len+=data->call_len;
}
