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
 *  size_deter.c created by Christian Holl
 */

#include <rosc/sebs_parse_fw/gen_modules/size_deter.h>
#include <rosc/debug/debug_out.h>

sebs_parse_return_t size_deter(sebs_parser_data_t* pdata)
{
	size_deter_data_t *fdata =
			(size_deter_data_t *) pdata->current_parser.parser_data;
	if (pdata->function_init)
	{
		pdata->function_init = false;
		fdata->message_size = 0;
	}

	uint32_t current_out_buffer_size=0;
	uint32_t def = 0;
	uint32_t size = 0;
	uint32_t subsize=0;
	uint32_t sub_size_storage;
	msg_gen_command_t *message_def=fdata->message_info_array;
	do
	{
		bool part_finished=false;
		char *str;
		uint32_t outstr_size;

		str=0;
		subsize=0;

	switch(message_def[def].type)
	{
		case MSG_GEN_TYPE_WHOLE_LEN_BIN:
		case MSG_GEN_TYPE_WHOLE_LEN_STRING:
		case MSG_GEN_TYPE_END:
			break;

		case MSG_GEN_TYPE_CHAR:
			subsize += 1;
			break;

		case MSG_GEN_TYPE_STRING:
			str = (char *) message_def[def].data;
			break;

		case MSG_GEN_TYPE_TAG:
			subsize += 2; // < + >
			str = (char *) message_def[def].data;
			break;

		case MSG_GEN_TYPE_CLOSE_TAG:
			subsize += 3; //</ >
			str = (char *) message_def[def].data;
			break;

		case MSG_GEN_TYPE_ROSRPC_FIELD:
			subsize += 1; //=
			str = (char *) message_def[def].data;
			break;

		case MSG_GEN_TYPE_FLOAT32:
			subsize += 4;
			break;

		case MSG_GEN_TYPE_FLOAT64:
			subsize += 8;
			break;

		case MSG_GEN_TYPE_BOOL:
		case MSG_GEN_TYPE_INT8:
		case MSG_GEN_TYPE_UINT8:
			subsize += 1;
			break;

		case MSG_GEN_TYPE_INT16:
		case MSG_GEN_TYPE_UINT16:
			subsize += 2;
			break;

		case MSG_GEN_TYPE_INT32:
		case MSG_GEN_TYPE_UINT32:
			subsize += 4;
			break;

		case MSG_GEN_TYPE_INT64:
		case MSG_GEN_TYPE_UINT64:
			subsize += 8;
			break;

		case MSG_GEN_TYPE_RESET_LEN:
			fdata->message_size = 0;
			break;

	}

	//If there is a string, get it's length
	if (str != 0)
		while (*str != '\0')
		{
			subsize++;
			++str;
		}

	//Add the current size to the whole message size
	fdata->message_size += subsize;

	//Where to store the size?
	for (sub_size_storage = def;
			message_def[sub_size_storage].belongs_to_previous
					&& sub_size_storage != 0; --sub_size_storage)
		;
	//Store the size...
	message_def[sub_size_storage].size += subsize;

	//Some debug info
	DEBUG_PRINT(INT,"\nElement",sub_size_storage);
	DEBUG_PRINT(INT,"Size",message_def[sub_size_storage].size);
	}while (message_def[def].type != MSG_GEN_TYPE_END); //return when message end reached...



	return (SEBS_PARSE_RETURN_FINISHED);
}
