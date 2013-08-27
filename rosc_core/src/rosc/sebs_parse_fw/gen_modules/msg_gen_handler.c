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
 *  msg_gen_handler.c created by Christian Holl
 */

#include <rosc/sebs_parse_fw/gen_modules/msg_gen_handler.h>
#include <rosc/sebs_parse_fw/gen_modules/size_deter.h>
#include <rosc/sebs_parse_fw/gen_modules/buffer_fill.h>

sebs_parse_return_t msg_gen_handler(sebs_parser_data_t* pdata)
{
	msg_gen_handler_data_t *hdata = pdata->handler_data;
	msg_gen_handler_init_t *idata = pdata->init_data;
	if (pdata->handler_init)
	{
		DEBUG_PRINT_STR("MSG GEN HANDLER INIT");
		pdata->handler_init = false;
		pdata->function_init = true;
		pdata->return_to_handler = false;
		pdata->overall_len = 0;
		pdata->security_len = 0;
		hdata->handler_state = MSG_GEN_HANDLER_STATE_SIZE;
		hdata->payload_size_available=false;
		hdata->message_definition = idata->message_definition;
		hdata->buffer_size = *pdata->len; //Get buffer len
		hdata->def_state = 0;
	}

	while(1)
	{
		if(*pdata->len==0)
		{
			int byte;
			*pdata->len+=hdata->buffer_size;
			*pdata->buf-=hdata->buffer_size;
			printf("current package: ");
			for (byte = 0; byte < *pdata->len; ++byte)
			{
				if((*pdata->buf)[byte] > ' ' && (*pdata->buf)[byte]< '~')
				{
					printf("%c", (*pdata->buf)[byte]);
				}
				else
				{
					printf("[%x]", (unsigned int) (*pdata->buf)[byte]);
				}
			}
			printf("\n");
			if(pdata->event!=SEBS_PARSE_EVENT_NONE)
				return (SEBS_PARSE_RETURN_GO_AHEAD);

		}

		switch(hdata->handler_state)
		{

				default:

	//Check for possible errors //TODO add some reasonable symbol here

				switch(*hdata->message_definition->type)
				{
					case __MSG_TYPE_NONE:
					case __MSG_TYPE_DESCRIPTORS:
					case __MSG_TYPE_FLOAT_BINARY_SEP:
					case __MSG_TYPE_FLOAT_STRING_SEP:
					case __MSG_TYPE_INT_BINARY_SEP:
					case __MSG_TYPE_INT_STRING_SEP:
					case __MSG_TYPE_ROSRPC_FIELD_STRINGS:
					case __MSG_TYPE_UINT_BINARY_SEP:
					case __MSG_TYPE_UINT_STRING_SEP:
					case __MSG_TYPE_XMLRPC_CLOSE_TAGS:
					case __MSG_TYPE_XMLRPC_OPEN_TAGS:
					DEBUG_PRINT_STR("msg_gen_handler: FOUND SYMBOL NOT TO BE USED AS MESSAGE DEFINITION!");
					break;

					case MSG_TYPE_PAYLOAD_SIZE_STRING:
					case MSG_TYPE_PAYLOAD_SIZE_BINARY:
						if(!hdata->payload_size_available) //Was the payload size already calculated?
						{//No, then do it now...
							hdata->payload_size_available=true;
							SIZE_DETER_INIT(pdata, hdata->size_deter, hdata->message_definition->type, hdata->message_definition->data, SIZE_DETER_PAYLOAD_SIZE);
						}
						else
						{
							if(hdata->payload_size_available==MSG_TYPE_PAYLOAD_SIZE_BINARY)
							{
								BUFFER_FILL_INIT_BINARY(pdata, hdata->buffer_fill, &hdata->payload_size, 4, 4);
							}
							else
							{
								//TODO NUM TO STRING
							}
						}
					break;

					case MSG_TYPE_PAYLOAD_SIZE_START:
						SIZE_DETER_INIT(pdata, hdata->size_deter, hdata->message_definition->type, hdata->message_definition->data, SIZE_DETER_PAYLOAD_SIZE);
					break;

					case MSG_TYPE_CHAR:
						BUFFER_FILL_INIT_BINARY(pdata, hdata->buffer_fill, *hdata->message_data_fields, 1, 1);
						++hdata->message_data_fields;
					break;

					case MSG_TYPE_STRING:
						BUFFER_FILL_INIT_STRING(pdata,hdata->buffer_fill, *hdata->message_data_fields);
						++hdata->message_data_fields;
					break;

					case MSG_TYPE_ROSRPC_FIELD_END:

					break;

					case MSG_TYPE_MESSAGE_END:

					break;

					default:
					//Do not change the order here! Otherwise the order must be change in msg_gen_type_t as well!
					if(*hdata->message_definition->type>__MSG_TYPE_ROSRPC_FIELD_STRINGS)
					{
						switch(hdata->def_state)
						{
							case 0://Determine field size
								hdata->def_state=1;
								SIZE_DETER_INIT(pdata, hdata->size_deter, hdata->message_definition->type, hdata->message_definition->data, SIZE_DETER_ROSRPC_FIELD);
								break;
							case 1://Write field size
								hdata->def_state=2;
								BUFFER_FILL_INIT_SIZE(pdata,hdata->buffer_fill,&hdata->size_deter.size,4,4);
								break;
							case 2://id string
								hdata->def_state=3;
								BUFFER_FILL_INIT_STRING(pdata,hdata->buffer_fill,ros_field_strings[*hdata->message_definition->type - __MSG_TYPE_ROSRPC_FIELD_STRINGS -1]);
								break;
							case 3://equal
								hdata->def_state=0;
								**pdata->buf='=';
								++*pdata->buf;
								--*pdata->len;
								++hdata->def_state;
								++hdata->msg_def;
								break;
						}
					}
					else if(*hdata->message_definition->type>__MSG_TYPE_XMLRPC_CLOSE_TAGS)
					{

					}
					else if(*hdata->message_definition->type>__MSG_TYPE_XMLRPC_OPEN_TAGS)
					{

					}
					else if(*hdata->message_definition->type>__MSG_TYPE_DESCRIPTORS)
					{

					}
					else if(*hdata->message_definition->type>__MSG_TYPE_UINT_STRING_SEP)
					{

					}
					else if(*hdata->message_definition->type>__MSG_TYPE_INT_STRING_SEP)
					{

					}
					else if(*hdata->message_definition->type>__MSG_TYPE_FLOAT_STRING_SEP)
					{

					}
					else if(*hdata->message_definition->type>__MSG_TYPE_UINT_BINARY_SEP)
					{

					}
					else if(*hdata->message_definition->type>__MSG_TYPE_INT_BINARY_SEP)
					{

					}
					else if(*hdata->message_definition->type>__MSG_TYPE_FLOAT_BINARY_SEP)
					{

					}
					break;

				}
				hdata->message_definition->type++;
				break;

			}




	}
		return (SEBS_PARSE_RETURN_GO_AHEAD);
}

