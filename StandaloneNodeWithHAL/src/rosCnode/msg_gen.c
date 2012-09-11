/*
 *  Copyright by Synapticon GmbH (www.synapticon.com)  ©2012
 *
 *  msg_gen.c
 *
 *  This file is part of StandaloneNodeWithHAL.
 *
 *  StandaloneNodeWithHAL is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  StandaloneNodeWithHAL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with StandaloneNodeWithHAL. If not, see <http://www.gnu.org/licenses/>.
 *
 *	File created by choll on 11.09.2012
 */

#include "msg_gen.h"
#include "msg_strings.h"

#if defined(DEBUG_RPC_GEN)
	#warning DEBUG_RPC_GEN is set, debugging output online, remove in release
#endif


void str2buf(unsigned int *index, char* buffer, const char* str, char mode)//TODO mode type
{
	if(mode==S2B_CTAG || mode == S2B_TAG)//If tag
	{
		//Add first bracket
		buffer[*index]='<';
		(*index)++;

		if(mode==S2B_CTAG)
		{
			buffer[*index]='/';
			(*index)++;
		}
	}
	//process string
    while (*str != '\0')
    {
    	buffer[*index]=*str;
    	(*index)++;
        str++;
    }

    if(mode==S2B_TAG || mode==S2B_CTAG)
    {
    	//Add second bracket
    	buffer[*index]='>';
		(*index)++;
    }
    else if(mode==S2B_HTTP_HEAD_FIELD_DESC)
    {
    	buffer[*index]=':';
		(*index)++;
		buffer[*index]=' ';
		(*index)++;
    }
    else if(mode==S2B_HTTP_HEAD_FIELD)
	{
		buffer[*index]='\n';
		(*index)++;
	}


	//Add terminator
	#if defined(DEBUG_RPC_GEN)
    buffer[*index]='\0';
	#endif
}


void int2buf(char* message_buffer, unsigned int *buf_index, unsigned int number)
{
	//count the places
	unsigned int a=10;
	while(number/a)a*=10;
	a/=10;
	while(a>0)
	{
		message_buffer[*buf_index]=(char)(number/a)+48;
		number%=a;
		a/=10;
		(*buf_index)++;
	}
	#if defined(DEBUG_RPC_GEN)
	message_buffer[*buf_index]='\0';
	#endif
}


void generateHTTPHeader(char* message_buffer,const http_head_gen_command* gen_array, const char **custom_string_array, unsigned int *buf_index)
{
	while(*gen_array != HTTP_HEADER_GEN_END)
	{
		if((*gen_array)>=HTTP_HEADER_VAL_UINT_NUMBER)
		{
			unsigned int number=(*gen_array)-HTTP_HEADER_VAL_UINT_NUMBER;
			int2buf(message_buffer,buf_index,number);
			message_buffer[*buf_index]='\n';
			(*buf_index)++;
		}
		else if((*gen_array)>=HTTP_HEADER_GEN_VAL_CUSTOM) //Print custom value
		{
			str2buf(buf_index,message_buffer,custom_string_array[(*gen_array)-HTTP_HEADER_GEN_VAL_CUSTOM],S2B_HTTP_HEAD_FIELD);
		}
		else if((*gen_array)>=HTTP_HEADER_VALUE_BEGIN) //Print std value
		{
			str2buf(buf_index,message_buffer,http_header_stdtext[(*gen_array)-HTTP_HEADER_VALUE_BEGIN],S2B_HTTP_HEAD_FIELD);
		}
		else if((*gen_array)>=HTTP_HEADER_GEN_DESC_CUSTOM) //Print custom descriptor
		{
			str2buf(buf_index,message_buffer,custom_string_array[(*gen_array)-HTTP_HEADER_GEN_DESC_CUSTOM],S2B_HTTP_HEAD_FIELD_DESC);
		}
		else if((*gen_array)>=HTTP_HEADER_DESC_BEGIN) //Print std descriptor
		{
			str2buf(buf_index,message_buffer,http_header_descriptors[(*gen_array)-HTTP_HEADER_DESC_BEGIN],S2B_HTTP_HEAD_FIELD_DESC);
		}
		gen_array++;
	}
	//Finish Header with empty line
	message_buffer[*buf_index]='\n';

	//Terminate
	(*buf_index)++;
	#if defined(DEBUG_RPC_GEN)
		message_buffer[*buf_index]='\0';
	#endif
}


char generateXML(char* message_buffer, const ros_rpc_gen_command* gen_array, const char **custom_string_array, unsigned int *gen_index, unsigned int *buf_index)
{
	unsigned int gen_command=gen_array[*gen_index];
	bool isTag=false;
	//Increase the index for the generator array
	(*gen_index)++;

	if(gen_command/RPC_STDTEXT_START)//Add Text
	{
		if(gen_command>=RPC_CUSTOM_TEXT)
		{
			#if defined(DEBUG_RPC_GEN)
			printf("%s\n",custom_string_array[gen_command-RPC_CUSTOM_TEXT]);
			#endif
			str2buf(buf_index, message_buffer, custom_string_array[gen_command-RPC_CUSTOM_TEXT],S2B_NORMAL);
			return 0;
		}
		else
		{
			#if defined(DEBUG_RPC_GEN)
			printf("%s\n", ros_rpc_stdtext[gen_command-RPC_STDTEXT_START]);
			#endif
			str2buf(buf_index, message_buffer, ros_rpc_stdtext[gen_command-RPC_STDTEXT_START],S2B_NORMAL);
			return 0;
		}
	}
	else if(gen_command/RPC_TAG_START) //Open Tag
	{
		isTag=true;
		if(gen_command>=RPC_CUSTOM_TAG)
		{
			#if defined(DEBUG_RPC_GEN)
			printf("<%s>\n", custom_string_array[gen_command-RPC_CUSTOM_TAG]);
			#endif
			str2buf(buf_index, message_buffer,custom_string_array[gen_command-RPC_CUSTOM_TAG],S2B_TAG);
		}
		else
		{
			#if defined(DEBUG_RPC_GEN)
			printf("<%s>\n", ros_rpc_tag_strings[gen_command-RPC_TAG_START]);
			#endif
			str2buf(buf_index, message_buffer,ros_rpc_tag_strings[gen_command-RPC_TAG_START],S2B_TAG);
		}
	}
	else if(gen_command==RPC_CLOSE_TAG)
	{
		return GEN_LEVEL_UP; //Go up one level (or end if first function)
	}
	else if(gen_command==RPC_GENERATOR_FINISH)
	{
		return GEN_STOP; //Abort all message generation
	}
	else if(gen_command==RPC_XML_DECLARATION)
	{
		#if defined(DEBUG_RPC_GEN)
		printf("<?xml version=\"1.0\"?>\n");
		#endif
		str2buf(buf_index, message_buffer,"<?xml version=\"1.0\"?>",S2B_NORMAL);
	}
	else
	{
		#if defined(DEBUG_RPC_GEN)
			printf("Debug: Error in XML generation!\n");
		#endif
			str2buf(buf_index, message_buffer,"XMLRPC Error",S2B_NORMAL);
		return GEN_ERROR;
	}

	int ret=GEN_RETURN_GO_AHEAD;
	while(!ret)
	{
		ret=generateXML(message_buffer, gen_array, custom_string_array,gen_index,buf_index);
		if(ret==GEN_STOP || ret==GEN_ERROR)
		{
			return ret;
		}
	}
	//if current output is a tag end it now
	if(isTag==true)
	{
		if(gen_command>=RPC_CUSTOM_TAG)
		{
			#if defined(DEBUG_RPC_GEN)
			printf("</%s>\n", custom_string_array[gen_command-RPC_CUSTOM_TAG]);
			#endif
			str2buf(buf_index, message_buffer, custom_string_array[gen_command-RPC_CUSTOM_TAG],S2B_CTAG);
		}
		else
		{
			#if defined(DEBUG_RPC_GEN)
			printf("</%s>\n", ros_rpc_tag_strings[gen_command-100]);
			#endif
			str2buf(buf_index, message_buffer,ros_rpc_tag_strings[gen_command-100],S2B_CTAG);
		}
	}
	return 0;
}
