#include "rosxmosNode.h"

/**
 * This array contains the strings for standard tags.
 * Note to editors: If you add something here make sure its also available (in the <b>SAME ORDER</b>)
 * in the ros_rpc_gen_command enum command block (for numbers>2000) (make it alphabetically ordered, maybe necessary!)
 */
char *ros_rpc_tag_strings[15] = //[][X] X must be the length of the longest string + 1 ('\0' string delimiter)
{
	"array",
	"boolean",
	"data",
	"int",
	"methodcall",
	"methodname",
	"methodresponse",
	"param",
	"params",
	"string",
	"value"
};

/**
 * This array contains the strings for standard tags.
 * Note to editors: If you add something here make sure its also available (in the <b>SAME ORDER</b>)
 * in the ros_rpc_gen_command enum command block (for numbers>4000) (make it alphabetically ordered, maybe necessary!)
 */
char *ros_rpc_stdtext[20] =//[][X] X must be the length of the longest string + 1 ('\0' string delimiter)
{
	"hasParam",
	"registerPublisher",
	"registerSubscriber",
	"requestTopic",
	"tcp_keepalive",
	"unregisterPublisher",
	"unregisterSubscriber"
};

/**
 * This array contains the strings for standard fields in the message headers
 * Note to editors: If you add something here make sure its also available (in the <b>SAME ORDER</b>)
 * in the http_head_gen_command enum command block (for numbers>=HTTP_HEADER_DESC_BEGIN)
 */
char *http_header_descriptors[18] =//[][X] X must be the length of the longest string + 1 ('\0' string delimiter)
{
		"Server",
		"User-Agent",
		"Date",
		"Host",
		"Content-Type",
		"Content-length",
		"Accepted-Encoding"
};

/**
 * This array contains the standard value strings for generating message headers
 * Note to editors: If you add something here make sure its also available (in the <b>SAME ORDER</b>)
 * in the http_head_gen_command enum command block (for numbers>=HTTP_HEADER_VALUE_BEGIN)
 */
char *http_header_stdtext[22] =//[][X] X must be the length of the longest string + 1 ('\0' string delimiter)
{
		"POST / HTTP/1.1",
		"XMLRPC ROSc-NodeLib",
		"BaseHTTP/ROSc-NodeLib",
		"text/xml",
		"HTTP/1.0",
		"OK"
};




void str2buf(unsigned int *index, char* buffer, char* str, char mode)
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
	else if(mode==S2B_HTTP_HEAD_FIELD)
	{
		buffer[*index]=' ';
		(*index)++;
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
    }
    else if(mode==S2B_HTTP_HEAD_FIELD)
	{
		buffer[*index]='\n';
		(*index)++;
	}


	//Add terminator
    buffer[*index]='\0';
}


char generateHeader(char* message_buffer, http_head_gen_command* gen_array, char **custom_string_array, unsigned int *buf_index)
{
	while(*gen_array != HTTP_HEADER_GEN_END)
	{

		if((*gen_array)>=225) //Print custom value
		{

		}
		else if((*gen_array)>=200) //Print std value
		{

		}
		else if((*gen_array)>=75) //Print custom descriptor
		{

		}
		else if((*gen_array)>=50) //Print std descriptor
		{

		}

		gen_array++;
	}

}


char generateXML(char* message_buffer, ros_rpc_gen_command* gen_array, char **custom_string_array, unsigned int *gen_index, unsigned int *buf_index)
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


