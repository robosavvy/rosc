/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2013, Synapticon GmbH
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Synapticon GmbH nor the names of its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * msg_gen.c
 *
 * Created on: 11.09.2012
 *     Author: Christian Holl
 *
 */

#include <rosc/com_xml/msg_gen.h>
#include <rosc/string_res/msg_strings.h>
#include <rosc/com_base/eth.h>



int output(char c) //TODO REMOVE THIS FUNCTION...
{
	printf("%c",c);
	return 1;
}


typedef enum
{
	XML_SM_OPTAIN_XML_SIZE,
	XML_SM_SEND_HTTP_HEADER,
	XML_SM_SEND_XML,
	XML_SM_SEND_FINISHED
}XML_SM_SEND_State;
typedef enum
{
	HTTP_SEND_CONTENT_LEN_DESC,
	HTTP_SEND_CONTENT_LEN_VALUE,
	HTTP_SEND_HEADER_SEPARATOR,
}HTTP_SEND_LEN_State;

send_status_t sendXMLMessage(port_id_t portID, const ros_rpc_gen_command* xml_gen_array, const http_head_gen_command* http_gen_array, const char **custom_string_array)
{
	XML_SM_SEND_State xml_state=XML_SM_OPTAIN_XML_SIZE;
	HTTP_SEND_LEN_State http_state=HTTP_SEND_CONTENT_LEN_DESC;
	const char *outstring;
	char singleCharOutput[]={'\0','\0'};
	bool firstOutputWritten=0;
	int outmode;
	unsigned int number;
	unsigned int xml_len=0;
	ros_rpc_gen_command* xml_gen_ptr=(ros_rpc_gen_command*)xml_gen_array;
	int header_command=*http_gen_array; //TODO REMOVE THIS AND USE THE DIRECT VARIABLE

	while(xml_state != XML_SM_SEND_FINISHED)
	{
		switch(xml_state)
		{
		case XML_SM_OPTAIN_XML_SIZE:
		case XML_SM_SEND_XML:
		{

			int command=*xml_gen_ptr;
			/*
			 * UNSIGNED INTEGER
			 */
			if(command>=RPC_UINT_NUMBER)
			{
				number=command-RPC_UINT_NUMBER;
				outmode=-1;
			}

			/*
			 * SINGLE CHAR
			 */
			if(command>=RPC_SINGLE_CHAR)
			{
				singleCharOutput[0]=command-RPC_SINGLE_CHAR;
				outstring=singleCharOutput;
				outmode=S2B_NORMAL;

			}

			/*
			 * TEXT FIELD
			 */
			else if(command>=__RPC_STDTEXT_START-1) //Add text
			{
				if(command>=RPC_CUSTOM_TEXT)
				{
					outstring=custom_string_array[command-RPC_CUSTOM_TEXT];
					outmode=S2B_NORMAL;
				}
				else
				{
					outstring=rpc_xml_stdtext[command-__RPC_STDTEXT_START-1];
					outmode=S2B_NORMAL;
				}
			}
			/*
			 * CLOSING TAG
			 */
			else if(command>=RPC_CLOSE_TAG)
			{
				if(command>=RPC_CUSTOM_TAG+RPC_CLOSE_TAG)
				{
					outstring=custom_string_array[command-RPC_CUSTOM_TAG-RPC_CLOSE_TAG];
					outmode=S2B_CTAG;
				}
				else
				{
					outstring=rpc_xml_tag_strings[command-__RPC_TAGS_START-1-RPC_CLOSE_TAG];
					outmode=S2B_CTAG;
				}
			}
			/*
			 * OPENING TAG
			 */
			else if(command>=__RPC_TAGS_START-1)
			{
				if(command>=RPC_CUSTOM_TAG)
				{
					outstring=custom_string_array[command-RPC_CUSTOM_TAG];
					outmode=S2B_TAG;
				}
				else
				{
					outstring=rpc_xml_tag_strings[command-__RPC_TAGS_START-1];
					outmode=S2B_TAG;
				}
			}



			if(command==RPC_GENERATOR_FINISH)
			{
				if(xml_state==XML_SM_OPTAIN_XML_SIZE)
				{
					//Reset it array pointer back to the start for generation
					xml_gen_ptr=(ros_rpc_gen_command *)xml_gen_array;
					xml_state=XML_SM_SEND_HTTP_HEADER;
				}
				else if(xml_state==XML_SM_SEND_XML)
				{
					xml_state=XML_SM_SEND_FINISHED;
				}

				continue;
			}

			xml_gen_ptr++;
			command=*xml_gen_ptr;
		}


		break;
	case XML_SM_SEND_HTTP_HEADER:

			if(header_command>=HTTP_HEADER_GEN_VAL_UINT_NUMBER)
			{
				number=header_command-HTTP_HEADER_GEN_VAL_UINT_NUMBER;
				//int2buf(message_buffer,&buf_index,number);
				outmode=-1;
			}
			else if(header_command>=HTTP_HEADER_GEN_SINGLE_CHAR)
			{
				singleCharOutput[0]=header_command-HTTP_HEADER_GEN_SINGLE_CHAR;
				outstring=singleCharOutput;
				outmode=S2B_NORMAL;
			}
			else if(header_command>=HTTP_HEADER_GEN_VAL_CUSTOM) //Print custom value, no newline...
			{
				//str2buf(&buf_index,message_buffer,custom_string_array[header_command-HTTP_HEADER_GEN_VAL_CUSTOM],S2B_NORMAL);
				outstring=custom_string_array[header_command-HTTP_HEADER_GEN_VAL_CUSTOM];
				outmode=S2B_NORMAL;
			}
			else if(header_command>=__HTTP_HEADER_GEN_VAL_START) //Print std value
			{
				//str2buf(&buf_index,message_buffer,http_header_stdtext[header_command-HTTP_HEADER_VALUE_BEGIN],S2B_HTTP_HEAD_FIELD);
				outstring=http_header_stdtext[header_command-__HTTP_HEADER_GEN_VAL_START-1];
				outmode=S2B_HTTP_HEAD_FIELD;
			}
			else if(header_command>=HTTP_HEADER_GEN_DESC_CUSTOM) //Print custom descriptor
			{
				//str2buf(&buf_index,message_buffer,custom_string_array[header_command-HTTP_HEADER_GEN_DESC_CUSTOM],S2B_HTTP_HEAD_FIELD_DESC);
				outstring=custom_string_array[header_command-HTTP_HEADER_GEN_DESC_CUSTOM];
				outmode=S2B_HTTP_HEAD_FIELD_DESC;
			}
			else if(header_command>=__HTTP_HEADER_GEN_DESC_START) //Print std descriptor
			{
				//str2buf(&buf_index,message_buffer,http_header_descriptors[header_command-HTTP_HEADER_DESC_BEGIN],S2B_HTTP_HEAD_FIELD_DESC);
				outstring=http_header_descriptors[header_command-__HTTP_HEADER_GEN_DESC_START-1];
				outmode=S2B_HTTP_HEAD_FIELD_DESC;
			}



			if(header_command==HTTP_HEADER_GEN_END)
			{
				switch (http_state) {
					case HTTP_SEND_CONTENT_LEN_DESC:
						outstring=http_header_descriptors[HTTP_HEADER_GEN_DESC_CONTENT_LENGTH-__HTTP_HEADER_GEN_DESC_START-1];
						outmode=S2B_HTTP_HEAD_FIELD_DESC;
						http_state=HTTP_SEND_CONTENT_LEN_VALUE;
						break;
					case HTTP_SEND_CONTENT_LEN_VALUE:
						number=xml_len;
						outmode=-1;
						http_state=HTTP_SEND_HEADER_SEPARATOR;
						break;
					case HTTP_SEND_HEADER_SEPARATOR:
						outstring="\n\n";
						outmode=HTTP_HEADER_GEN_VAL_CUSTOM;
						xml_state=XML_SM_SEND_XML;
						break;

					default:
						break;
				}

			}
			else
			{
				http_gen_array++;
				header_command=*http_gen_array;
			}
		break;
	case XML_SM_SEND_FINISHED:
		//Just surpressing the warning of unhandled state and
		//be sure if this is ever executed to go back to the loop head
		continue;
		break;
	}

	if(outmode<0)
	{
		unsigned int a=10;
		while(number/a)a*=10;
		a/=10;
		while(a>0)
		{
			if(xml_state==XML_SM_OPTAIN_XML_SIZE) xml_len++;
			else output((char)(number/a)+48); //TODO change this to sending function with port
			number%=a;
			a/=10;
		}
	}
	else
	{
			int s;
			char *strs[3]={"","",""};
			strs[1]=(char*)outstring;
			switch(outmode)
			{
			case S2B_TAG:
				strs[0]="<";
				strs[2]=">";
				break;
			case S2B_CTAG:
				strs[0]="</";
				strs[2]=">";
				break;
			case S2B_HTTP_HEAD_FIELD_DESC:
				if(firstOutputWritten)
				strs[0]="\n";
				strs[2]=": ";
				break;
			case S2B_HTTP_HEAD_FIELD:
			case S2B_NORMAL:
				break;
			}
			for(s=0;s<3;++s)
			{
				while (*strs[s] != '\0')
				{


					if(xml_state==XML_SM_SEND_XML || xml_state==XML_SM_SEND_HTTP_HEADER)
					{
						firstOutputWritten=true;
						output(*strs[s]); //TODO change this to sending function with port
					}
					else xml_len++;
					strs[s]++;
				}
			}
	}







	}
	return SEND_STATUS_OK;
}

