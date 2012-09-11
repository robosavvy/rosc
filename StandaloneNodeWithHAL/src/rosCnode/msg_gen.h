/*
 *  Copyright by Synapticon GmbH (www.synapticon.com)  ©2012
 *
 *  msg_gen.h
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

#ifndef MSG_GEN_H_
#define MSG_GEN_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

/**
 * Command enum for the xmlrpc message generator
 */
#define RPC_TAG_START 	   100 // Number where the enum values for tags start
#define RPC_STDTEXT_START 150 // Number where the enum values for texts start



/**
 * This enum holds the commands for the rpc XML generator
 */
typedef enum
{

	RPC_CLOSE_TAG=0, //!< Close current tag

	RPC_GENERATOR_FINISH, //!<Stops the generator at any place

	RPC_XML_DECLARATION, //!<Places XML Declaration

	//Note to editors: If you add something here make sure its also available (in the <b>SAME ORDER</b>)
	//in ros_rpc_tag_strings (make it alphabetically ordered, maybe necessary!)
	RPC_TAG_ARRAY=RPC_TAG_START,//!< place tag array
	RPC_TAG_BOOLEAN,			//!< place tag boolean
	RPC_TAG_DATA,				//!< place tag data
	RPC_TAG_INT,				//!< place tag int
	RPC_TAG_METHODCALL,			//!< place tag methodcall
	RPC_TAG_METHODNAME,			//!< place tag methodname
	RPC_TAG_METHODRESPONSE,		//!< place tag methodresponse
	RPC_TAG_PARAM,				//!< place tag param
	RPC_TAG_PARAMS,				//!< place tag params
	RPC_TAG_STRING,				//!< place tag string
	RPC_TAG_VALUE,				//!< place tag value
	/**
	 * Use this number for selecting custom tag strings where RPC_CUSTOM_TAG is the first (0)
	 * string of your custom array and RPC_CUSTOM_TAG+X, tag X in your array
	 */
	RPC_CUSTOM_TAG, //!< RPC_CUSTOM_TAG

	//Note to editors: If you add something here make sure its also available (in the <b>SAME ORDER</b>)
    //in ros_rpc_stdtext (make it alphabetically ordered, maybe necessary!)
	RPC_STDTEXT_HASPARAM=RPC_STDTEXT_START, //!< place text "hasParam" 				in the current open tag
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
	RPC_CUSTOM_TEXT,//!< RPC_CUSTOM_TEXT

} ros_rpc_gen_command;


/**
 * Command enum for the xmlrpc message generator
 */

#define HTTP_HEADER_DESC_BEGIN 50	//Starting number for field descriptions
#define HTTP_HEADER_VALUE_BEGIN 150

typedef enum
{
	HTTP_HEADER_GEN_END=0, //!< This is required on any header generation array end.

	///@{
	/**
	 * Values specifying most used standard field descriptors
	 */
	HTTP_HEADER_GEN_DESC_SERVER=HTTP_HEADER_DESC_BEGIN,
	HTTP_HEADER_GEN_DESC_USER_AGENT,
	HTTP_HEADER_GEN_DESC_DATE,
	HTTP_HEADER_GEN_DESC_HOST,
	HTTP_HEADER_GEN_DESC_CONTENT_TYPE,
	HTTP_HEADER_GEN_DESC_CONTENT_LENGTH,
	HTTP_HEADER_GEN_DESC_ACCEPTED_CODING,
	///@}

	/**
	 * This specifies to use a text in the custom text array as descriptor
	 */
	HTTP_HEADER_GEN_DESC_CUSTOM,

	///@{
	/**
	 * Values specifying most used standard text pieces
	 */
	HTTP_HEADER_GEN_VAL_POST_HTTP_1_1=HTTP_HEADER_VALUE_BEGIN,
	HTTP_HEADER_GEN_VAL_XMLRPC_ROSC_NODELIB,
	HTTP_HEADER_GEN_VAL_BASEHTTP_ROSC_NODELIB,
	HTTP_HEADER_GEN_VAL_TEXT_XML,
	HTTP_HEADER_GEN_VAL_HTTP_1_0,
	HTTP_HEADER_GEN_VAL_OK,
	///@}

	/**
	 * This command will use a custom text array as value
	 */
	HTTP_HEADER_GEN_VAL_CUSTOM,


	/**
	 * This command will add a number to the
	 * header.
	 *
	 * If HTTP_HEADER_UINT_NUMBER + X
	 * will result in "Content-Length: X", where
	 * the supplied X can be a unsigned integer.
	 *
	 */
	HTTP_HEADER_VAL_UINT_NUMBER=255
}http_head_gen_command;


/**
 * Return values of the __generateXML function
 */
typedef enum
{
	GEN_RETURN_GO_AHEAD = 0,
	GEN_LEVEL_UP,
	GEN_STOP,
	GEN_ERROR
}ros_rpc_gen_return;


/**
 * These are the modes for the str2buf function
 */
typedef enum
{
	S2B_NORMAL=0, //!< This mode does just insert the given string
	S2B_TAG,	  //!< This mode will insert the string with tag brackets
	S2B_CTAG,	  //!< This mode will insert the string as closing tag
	S2B_HTTP_HEAD_FIELD_DESC, //!< This mode adds a : to the string
	S2B_HTTP_HEAD_FIELD, 	  //!< This mode adds a space infront and a newline at the back of the string
}str2buf_modes;

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
 * @param mode This specifies the mode, see str2buf_modes enum
 */
void str2buf(unsigned int *index, char* buffer, const char* str, char mode);

/**
 *	This function can be used to generate headers for a HTTP message.
 * 	@param[out] message_buffer The output buffer for the message
 * 	@param[in] gen_array The command array for generating messages.
 * 	@param[in] custom_string_array The array for custom strings
 * 	@param buf_index The index for the output buffer
 * 	@return Message length
 */
void generateHTTPHeader(char* message_buffer, http_head_gen_command* gen_array, char **custom_string_array, unsigned int *buf_index);

/**
 * This function generates a xmlrpc message
 * @param[out] message_buffer The output buffer for the message
 * @param[in] gen_array The command array for generating messages.
 * @param[in] custom_string_array The array for custom strings (topics, params etc.)
 * @param gen_index The index for the gen_array, set to 0 at start, increased by each call of generate_XML
 * @param buf_index The index for the output buffer
 * @return Message length
 */
char generateXML(char* message_buffer, ros_rpc_gen_command* gen_array, char **custom_string_array, unsigned int *gen_index, unsigned int *buf_index);



#endif /* MSG_GEN_H_ */
