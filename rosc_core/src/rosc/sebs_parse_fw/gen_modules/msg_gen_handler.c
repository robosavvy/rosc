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
	msg_gen_handler_data_t *hdata=pdata->handler_data;
	msg_gen_handler_init_t *idata=pdata->init_data;
	if(pdata->handler_init)
	{
		DEBUG_PRINT_STR("MSG GEN HANDLER INIT");
		pdata->handler_init=false;
		pdata->function_init=true;
		pdata->return_to_handler=false;
		pdata->overall_len=0;
		pdata->security_len=0;
		hdata->handler_state=MSG_GEN_HANDLER_STATE_SIZE;
		hdata->buffer_size=*pdata->len; //Get buffer len
		hdata->def_state=0;
	}


	switch (hdata->handler_state)
	{
		case MSG_GEN_HANDLER_STATE_SIZE:

			hdata->handler_state=MSG_GEN_HANDLER_STATE_OUTPUT;//state on next handler call
			SIZE_DETER_INIT(pdata,hdata->size_deter,idata->message_definition);
			break;

		case MSG_GEN_HANDLER_STATE_OUTPUT:

			hdata->message_size=hdata->size_deter.message_size; //Get message size

		while(1)
		{
//			if(*pdata->len>0)
//			switch(hdata->message_definition[hdata->msg_def].type)
//			{
//				case MSG_GEN_TYPE_BOOL:
//				case MSG_GEN_TYPE_CHAR:
//				case MSG_GEN_TYPE_INT8:
//				case MSG_GEN_TYPE_UINT8:
//					**pdata->buf=*((char *)hdata->message_definition[hdata->msg_def].data);
//					++*pdata->buf;
//					--*pdata->len;
//					++hdata->msg_def;
//				break;
//
//				case MSG_GEN_TYPE_TAG:
//					if(hdata->def_state==0)
//					{
//						**pdata->buf='<';
//						++*pdata->buf;
//						--*pdata->len;
//						hdata->def_state=1;
//						//TODO CALL BUFF_FILL HERE
//					}
//					else if(hdata->def_state==1)
//					{
//						**pdata->buf='>';
//						++*pdata->buf;
//						--*pdata->len;
//						hdata->def_state=0;
//						++hdata->msg_def;
//					}
//					break;
//
//				case MSG_GEN_TYPE_CLOSE_TAG:
//					if(hdata->def_state==0)
//					{
//						**pdata->buf='<';
//						++*pdata->buf;
//						--*pdata->len;
//						hdata->def_state=1;
//						//TODO CALL BUFF_FILL HERE
//					}
//					else if(hdata->def_state==1)
//					{
//						**pdata->buf='/';
//						++*pdata->buf;
//						--*pdata->len;
//						hdata->def_state=2;
//					}
//					else
//					{
//						**pdata->buf='>';
//						++*pdata->buf;
//						--*pdata->len;
//						hdata->def_state=0;
//						++hdata->msg_def;
//					}
//				break;
//
//				case MSG_GEN_TYPE_INT16:
//				case MSG_GEN_TYPE_UINT16:
//					++hdata->msg_def;
//					BUFFER_FILL_INIT_SIZE(pdata,hdata->buffer_fill,&hdata->message_definition[hdata->msg_def].size,2,2);
//				break;
//
//				case MSG_GEN_TYPE_FLOAT32:
//				case MSG_GEN_TYPE_INT32:
//				case MSG_GEN_TYPE_UINT32:
//					++hdata->msg_def;
//					BUFFER_FILL_INIT_SIZE(pdata,hdata->buffer_fill,&hdata->message_definition[hdata->msg_def].size,4,4);
//				break;
//
//				case MSG_GEN_TYPE_UINT64:
//				case MSG_GEN_TYPE_INT64:
//				case MSG_GEN_TYPE_FLOAT64:
//					++hdata->msg_def;
//					BUFFER_FILL_INIT_SIZE(pdata,hdata->buffer_fill,&hdata->message_definition[hdata->msg_def].size,8,8);
//				break;
//
//				case MSG_GEN_TYPE_RESET_LEN:
//				break;
//
//				case MSG_GEN_TYPE_ROSRPC_FIELD:
//					DEBUG_PRINT_STR("\nROSRPC");
//					switch(hdata->def_state)
//					{
//						case 0://Field size
//							hdata->def_state=1;
//							BUFFER_FILL_INIT_SIZE(pdata,hdata->buffer_fill,&hdata->message_definition[hdata->msg_def].size,4,4);
//							break;
//						case 1://id string
//							hdata->def_state=2;
//							BUFFER_FILL_INIT_STRING(pdata,hdata->buffer_fill,hdata->message_definition[hdata->msg_def].data);
//							break;
//						case 2://equal
//							hdata->def_state=0;
//							**pdata->buf='=';
//							++*pdata->buf;
//							--*pdata->len;
//							++hdata->def_state;
//							++hdata->msg_def;
//							break;
//					}
//				break;
//
//				case MSG_GEN_TYPE_STRING:
//					DEBUG_PRINT(STR,"\nSTRING: ",(char*)hdata->message_definition[hdata->msg_def].data);
//					BUFFER_FILL_INIT_STRING(pdata,hdata->buffer_fill,hdata->message_definition[hdata->msg_def++].data);
//				break;
//
//				case MSG_GEN_TYPE_WHOLE_LEN_BIN:
//					DEBUG_PRINT_STR("\nLEN BIN");
//					++hdata->msg_def;
//					BUFFER_FILL_INIT_SIZE(pdata,hdata->buffer_fill,&hdata->message_size,4,4);
//				break;
//
//				case MSG_GEN_TYPE_WHOLE_LEN_STRING:
//
//				break;
//
//				case MSG_GEN_TYPE_END:
//					pdata->len=0;
//					return SEBS_PARSE_RETURN_GO_AHEAD;
//				break;
//			}
//
//			printf("len %i\n",*pdata->len);
//			if(*pdata->len==0)
//			{
//				int byte;
//				*pdata->len+=hdata->buffer_size;
//				*pdata->buf-=hdata->buffer_size;
//				printf("current package: ");
//				for (byte = 0; byte < *pdata->len; ++byte)
//				{
//					if((*pdata->buf)[byte] > ' ' && (*pdata->buf)[byte]< '~')
//					{
//						printf("%c", (*pdata->buf)[byte]);
//					}
//					else
//					{
//						printf("[%x]", (unsigned int) (*pdata->buf)[byte]);
//					}
//				}
//				printf("\n");
//				if(pdata->event!=SEBS_PARSE_EVENT_NONE)
//					return (SEBS_PARSE_RETURN_GO_AHEAD);
//
//			}


		}

			break;


		default:

			break;
	}



	return (SEBS_PARSE_RETURN_GO_AHEAD);
}

