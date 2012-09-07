/*
 * rosXmosNode.h
 *
 *  Created on: 06.09.2012
 *      Author: choll
 */

#ifndef ROSXMOSNODE_H_
#define ROSXMOSNODE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 *	This
 */
typedef struct
{
	char *node_name; 				//!< The name of the node, must be <b>unique</b> in whole ROS!
	char *ros_master_host; 			//!< Hostname of the computer running roscore
	unsigned int ros_master_port; //!< RPC port of roscore, standard is: 11311
}ros_node_setup;

/**
 * Command enum for the xmlrpc message generator
 */
typedef enum
{
	RPC_CLOSE_TAG=0, //!< Close current tag

	//Note to editors: If you add something here make sure its also available (in the <b>SAME ORDER</b>)
	//in ros_rpc_tag_strings (make it alphabetically ordered, maybe necessary!)
	RPC_TAG_ARRAY=100,			//!< place tag <array>
	RPC_TAG_BOOLEAN,			//!< place tag <boolean>
	RPC_TAG_DATA,				//!< place tag <data>
	RPC_TAG_INT,				//!< place tag <int>
	RPC_TAG_METHODCALL,			//!< place tag <methodcall>
	RPC_TAG_METHODNAME,			//!< place tag <methodname>
	RPC_TAG_METHODRESPONSE,		//!< place tag <methodresponse>
	RPC_TAG_PARAM,				//!< place tag <param>
	RPC_TAG_PARAMS,				//!< place tag <params>
	RPC_TAG_STRING,				//!< place tag <string>
	RPC_TAG_VALUE,				//!< place tag <value>
	/**
	 * Use this number for selecting custom tag strings where RPC_CUSTOM_TAG is the first (0)
	 * string of your custom array and RPC_CUSTOM_TAG+X, tag X in your array
	 */
	RPC_CUSTOM_TAG,

	//Note to editors: If you add something here make sure its also available (in the <b>SAME ORDER</b>)
    //in ros_rpc_stdtext (make it alphabetically ordered, maybe necessary!)
	RPC_STDTEXT_HASPARAM=200,				//!< place text "hasParam" 				in the current open tag
	RPC_STDTEXT_REGISTERPUBLISHER,			//!< place text "registerPublisher" 	in the current open tag
	RPC_STDTEXT_REGISTERSUBSCRIBER,			//!< place text "registerSubscriber" 	in the current open tag
	RPC_STDTEXT_REQUESTTOPIC,				//!< place text "requestTopic" 			in the current open tag
	RPC_STDTEXT_TCP_KEEPALIVE,				//!< place text "tcp_keepalive" 		in the current open tag
	RPC_STDTEXT_UNREGISTERPUBLISHER,		//!< place text "unregisterPublisher" 	in the current open tag
	RPC_STDTEXT_UNREGISTERSUBSCRIBER,		//!< place text "unregisterSubscriber" 	in the current open tag

	/**
	 * Use this number for selecting custom text strings where RPC_CUSTOM_TEXT is the first (0)
	 * string of your custom array and RPC_CUSTOM_TAG+X, tag X in your array
	 */
	RPC_CUSTOM_TEXT,



} ros_rpc_gen_command;

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
 * To normal user <b>DO NOT</b> use this function use GENERATE_XML instead!<br/>
 *
 * This function generates a xmlrpc message, see GENERATE_XML for details.
 * @param[out] message_buffer The output buffer for the message
 * @param[in] gen_array The command array for generating messages.
 * @param[in] custom_string_array The array for custom strings (topics, params etc.)
 * @param gen_index The index for the gen_array, set to 0 at start, increased by each call of generate_XML
 * @param cust_index The index for the custom string array
 * @param buf_index The index for the output buffer
 * @return Message length
 */
char __generateXML(char* message_buffer, unsigned int* gen_array, char **custom_string_array, unsigned int *gen_index, unsigned int *buf_index)
{
	unsigned int gen_command=gen_array[*gen_index];
	//Increase the index for the generator array
	(*gen_index)++;

	if(gen_command/200)//Add Text
	{
		if(gen_command>=RPC_CUSTOM_TEXT)
		{
			printf("%s\n",custom_string_array[gen_command-RPC_CUSTOM_TEXT]);
		}
		else
		{
			printf("%s\n", ros_rpc_stdtext[gen_command-200]);
		}
	}
	else if(gen_command/100) //Open Tag
	{
		if(gen_command>=RPC_CUSTOM_TAG)
		{
			printf("<%s>\n", custom_string_array[gen_command-RPC_CUSTOM_TAG]);
		}
		else
		{
			printf("<%s>\n", ros_rpc_tag_strings[gen_command-100]);
		}

		int ret=0;
		while(!ret)
		{
			ret=__generateXML(message_buffer, gen_array, custom_string_array,gen_index,buf_index);
		}

		if(gen_command>=RPC_CUSTOM_TAG)
		{
			printf("</%s>\n", custom_string_array[gen_command-RPC_CUSTOM_TAG]);
		}
		else
		{
			printf("</%s>\n", ros_rpc_tag_strings[gen_command-100]);
		}
	}
	else if(gen_command==RPC_CLOSE_TAG)
	{
		return 1;
	}
	else
	{
		//TODO ERROR
		printf("ERROR");
	}
	return 0;
}


#define GENERATE_XML(BUFFER,GEN_ARRAY,CUSTOM_STR_ARR)\
	unsigned int __gen_index=0;\
    unsigned int __buf_index=0;\
	__generateXML(BUFFER,GEN_ARRAY,CUSTOM_STR_ARR,&__gen_index,&__buf_index);\
	BUFFER[__buf_index]='\0';


#endif /* ROSXMOSNODE_H_ */
