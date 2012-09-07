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




void str2buf(unsigned int *index, char* buffer, char* str)
{
    while (*str != '\0')
    {
    	buffer[*index]=*str;
    	(*index)++;
        str++;
    }
    buffer[*index]='\0';
}

char __generateXML(char* message_buffer, unsigned int* gen_array, char **custom_string_array, unsigned int *gen_index, unsigned int *buf_index)
{
	unsigned int gen_command=gen_array[*gen_index];
	bool isTag=false;
	//Increase the index for the generator array
	(*gen_index)++;

	if(gen_command/200)//Add Text
	{
		if(gen_command>=RPC_CUSTOM_TEXT)
		{
			printf("%s\n",custom_string_array[gen_command-RPC_CUSTOM_TEXT]);
			return 0;
		}
		else
		{
			printf("%s\n", ros_rpc_stdtext[gen_command-200]);
			return 0;
		}
	}
	else if(gen_command/100) //Open Tag
	{
		isTag=true;
		if(gen_command>=RPC_CUSTOM_TAG)
		{
			printf("<%s>\n", custom_string_array[gen_command-RPC_CUSTOM_TAG]);
		}
		else
		{
			printf("<%s>\n", ros_rpc_tag_strings[gen_command-100]);
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
		printf("<?xml version=\"1.0\"?>\n");
	}
	else
	{
		//TODO ERROR
		printf("ERROR");
	}

	int ret=GEN_RETURN_GO_AHEAD;
	while(!ret)
	{
		ret=__generateXML(message_buffer, gen_array, custom_string_array,gen_index,buf_index);
		if(ret==GEN_STOP)
		{
			return GEN_STOP;
		}
	}
	//if current output is a tag end it now
	if(isTag==true)
	{
		if(gen_command>=RPC_CUSTOM_TAG)
		{
			printf("</%s>\n", custom_string_array[gen_command-RPC_CUSTOM_TAG]);
		}
		else
		{
			printf("</%s>\n", ros_rpc_tag_strings[gen_command-100]);
		}
	}
	return 0;
}
