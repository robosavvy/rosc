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
#include <stdbool.h>

/**
 *	This struct stores the basic ROS node configuration
 */
typedef struct
{
	char 			*node_name; 			//!< The name of the node, must be <b>unique</b> in whole ROS!
	char 			*ros_master_host; 		//!< Hostname of the computer running roscore
	unsigned int 	ros_master_port; 		//!< RPC port of roscore, standard is: 11311
}ros_node_setup;


/**
 * Command enum for the xmlrpc message generator
 */
typedef enum
{

	RPC_CLOSE_TAG=0, //!< Close current tag

	RPC_GENERATOR_FINISH=1, //!<Stops the generator at any place

	RPC_XML_DECLARATION=50, //!<Places XML Declaration

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
 * Return values of the __generateXML function
 */
typedef enum
{
	GEN_RETURN_GO_AHEAD = 0,
	GEN_LEVEL_UP,
	GEN_STOP
}ros_rpc_gen_return;


/**
 * This function generates a xmlrpc message
 * @param[out] message_buffer The output buffer for the message
 * @param[in] gen_array The command array for generating messages.
 * @param[in] custom_string_array The array for custom str
 * ings (topics, params etc.)
 * @param gen_index The index for the gen_array, set to 0 at start, increased by each call of generate_XML
 * @param buf_index The index for the output buffer
 * @return Message length
 */
char __generateXML(char* message_buffer, unsigned int* gen_array, char **custom_string_array, unsigned int *gen_index, unsigned int *buf_index);

/**
 * This function copies a string into a buffer.
 * The starting point inside the string is given with the index pointer,
 * which is also increased. This is used to be able to add multiple strings
 * behind each other in the same buffer.
 * <b>Be sure your buffer is big enough, there are no checks for
 * the buffer size for increasing performance!</b>
 *
 * @param index The current index
 * @param buffer The buffer to write the string to.
 * @param str The <b>terminated</b> string to write into the buffer
 */
void str2buf(unsigned int *index, char* buffer, char* str);

#define GENERATE_XML(BUFFER,GEN_ARRAY,CUSTOM_STR_ARR)\
	unsigned int __gen_index=0;\
    unsigned int __buf_index=0;\
	__generateXML(BUFFER,GEN_ARRAY,CUSTOM_STR_ARR,&__gen_index,&__buf_index);\
	BUFFER[__buf_index]='\0';\


#endif /* ROSXMOSNODE_H_ */
