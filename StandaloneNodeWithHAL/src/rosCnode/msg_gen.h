/*
 *  Copyright by Synapticon GmbH (www.synapticon.com)  ©2012
 *
 *  msg_gen.h
 *
 *  This file is part of ROScNode Library.
 *
 *  ROScNode Library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ROScNode Library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with ROScNode Library. If not, see <http://www.gnu.org/licenses/>.
 *
 *	File created by Christian Holl on 11.09.2012
 */


/**
 *  @example rpcmsg_generation.c
 *	This example will show how to generate a RPC messages with the functions generateHTTPHeader
 *  and generateXML
 *
 *  A RPC message consists of two parts an HTTP Header, telling receiver information about
 *  the other side and a xml message.
 *
 *  For generating such a message there are two functions \ref generateHTTPHeader and \ref generateXML.
 *
 *  The most usefull tags or strings needed for interacting with a server are already predefined,
 *  but sometimes it might be necessary to have additional strings which are inserted into
 *  the message.
 *
 *  For this custom string arrays can be generated, either one for each function or one for both functions.
 *  In this example we are using seperate arrays:
 *
 *  \snippet examples/rpcmsg_generation.c Generating arrays for custom strings
 *
 *	As seen here the arrays can be constant arrays or not. But both functions normally require
 *	const char**, otherwise a compiler warning is generated when compiling the code. To
 *	surpress the warning my insert a cast like here:
 *
 *	\snippet examples/rpcmsg_generation.c Generating the Header
 *
 *	For storing the header and the message for later usage, it's necessary to have output buffers:
 *
 *	\snippet examples/rpcmsg_generation.c Creating the buffers
 *
 *	First of all the XML message has to be generated, because the header generator needs to know it's
 *	size for the Content-Length parameter.
 *
 *	For the header and message generation we use command arrays. Command arrays are integer arrays
 *	holding specific commands for the generator. They can contain each tag as often as they won't,
 *	while the string for the tag itself is only stored once. This is necessary because many platforms
 *	like microcontrollers do not offer much memory, often less than a megabyte.
 *
 *	The command sets for the XML generator and Header Generator differ because according to the HTTP
 *	standard a header field is closed with a line feed and can't be nested, while XML tags are closed
 *	by a closing tag and allow nesting.
 *
 * The next snippet shows the generator commands for the XML generator.
 * \snippet examples/rpcmsg_generation.c Commands for the XML generation
 *
 * As you can see it's a array of the type \ref ros_rpc_gen_command which is a enum type and should be
 * translated to integer.
 *
 * The array starts with \ref RPC_XML_DECLARATION, which inserts the string
 * \verbatim<?xml version="1.0" ?>\endverbatim
 *
 * We skip the first open tag \verbatim<methodcall>\endverbatim which is created by \ref RPC_TAG_METHODCALL
 * and look at the next tag \verbatim<methodname>\endverbatim created by \ref RPC_TAG_METHODNAME
 * because it's more suitable for the explanation.
 *
 * \snippet examples/rpcmsg_generation.c Standard Tag and Text
 *
 * As seen RPC_TAG_ commands create a open standard tag. The following command \ref RPC_STDTEXT_HASPARAM,
 * creates the standard string hasParam. To close a tag it's necessary to call the same command again but
 * place the \ref RPC_CT macro infront of it. As seen in the last snippet.
 *
 * In the next snippet we see the the usage of custom tags and texts and the generation of a string of a number.
 *  *
 * \snippet examples/rpcmsg_generation.c Custom Tag and Text
 *
 * To create a tag which uses a string out of the custom string array, \ref RPC_CUSTOM_TAG used and added
 * to a number, the number specifies the string which will be used to created the tag. Closing the
 * tag works the same like for standard tags by adding a \ref RPC_CT in front of it.
 *
 * The \ref RPC_CUSTOM_TEXT command works like the same as the \ref RPC_CUSTOM_TAG command. The
 * only difference is, that the string is directly inserted into the message without brackets.
 *
 * Another feature, executed with command \ref RPC_UINT_NUMBER allows to place different
 * unsigned integers into the message. The integer to be printed must be added to the command.
 *
 * Now we call the generateXML function and store it's return value, which is the message length,
 * into a variable.
 *
 * \snippet examples/rpcmsg_generation.c generating XML
 *
 * As mentioned before, the message length is needed for creating the header,so it can be created now.
 *
 * The next snippet shows the array of the type \ref http_head_gen_command.
 * \snippet examples/rpcmsg_generation.c Commands for the header generation
 *
 * The usage of the header generation commands is nearly the same as those for XML generation.
 *
 * \snippet examples/rpcmsg_generation.c Standard header descriptor and value
 *
 * The commands beginning with HTTP_HEADER_GEN_ like  HTTP_HEADER_GEN_DESC_USER_AGENT generate
 * header descriptors ending with a double point ("User-Agent:"). While commands beginning
 * HTTP_HEADER_GEN_VAL_ like  HTTP_HEADER_GEN_VAL_XMLRPC_ROSC_NODELIB create string values
 * ending with a line feed ("XMLRPC ROSc-NodeLib").
 *
 * Custom descriptors and values are created with HTTP_HEADER_GEN_DESC_CUSTOM and
 * HTTP_HEADER_GEN_VAL_CUSTOM.
 *
 * As seen in the next snippet they work the same like their pendants for the XML generator.
 * But if using a non standard text HTTP_HEADER_GEN_VAL_CUSTOM or the string from integer
 * command HTTP_HEADER_GEN_VAL_UINT_NUMBER, it's necessary to add a HTTP_HEADER_GEN_CUSTOM_TEXT_END,
 * to the end for creating a new line.
 *
 * \snippet examples/rpcmsg_generation.c Custom header descriptor and value
 *
 * To generate the header the function \ref generateHTTPHeader is called and the return value,
 * which is also the length, stored in another variable.
 *
 * \snippet examples/rpcmsg_generation.c Generating the Header
 *
 * At the end this examples prints the strings of header and message. Which are not terminated!
 *
 * \snippet examples/rpcmsg_generation.c Just Printing
 *
 *  \par Output:
 \verbatim
 User-Agent: XMLRPC ROSc-NodeLib
 Host: http://myHost:11311
 Content-Type: text/xml
 Content-length: 150
 custom_desc: custom_val

 <?xml version="1.0"?><methodcall><methodname>hasParam</methodname><params><param><custom_tag>custom_text900</custom_tag></param></params></methodcall>\endverbatim
 *
 *\par The whole example code:
 */

#ifndef MSG_GEN_H_
#define MSG_GEN_H_

#include <stdio.h>
#include <string.h>
#include <stdbool.h>


#define CUSTOM_STRINGS_MAX 50

/**
 * This macro advises the generator to print a close tag instead of opening a new one.
 * \n Example:  ... RPC_CT RPC_TAG_ARRAY, ...
 */
#define RPC_CT RPC_CLOSE_TAG+

/**
 * This enum holds the commands for the rpc XML generator
 */
typedef enum
{
	RPC_GENERATOR_FINISH, //!<Stops the generator at any place

	RPC_XML_DECLARATION, //!<Places XML Declaration

	//Note to editors: If you add something here make sure its also available (in the <b>SAME ORDER</b>)
	//in ros_rpc_tag_strings (make it alphabetically ordered, maybe necessary!)
	RPC_TAGS_START,				//!< Marks the start of the tags
	RPC_TAG_ARRAY,				//!< place tag array
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
	RPC_CUSTOM_TAG,


	RPC_CLOSE_TAG=RPC_CUSTOM_TAG +CUSTOM_STRINGS_MAX,

	//Note to editors: If you add something here make sure its also available (in the <b>SAME ORDER</b>)
    //in ros_rpc_stdtext (make it alphabetically ordered, maybe necessary!)
	RPC_STDTEXT_START=RPC_CLOSE_TAG +CUSTOM_STRINGS_MAX,	//!< Marks the start of the standard text strings
	RPC_STDTEXT_HASPARAM, 					//!< place text "hasParam" 				in the current open tag
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

	/**
	 * This command inserts a unsigned integer into the message
	 */
	RPC_UINT_NUMBER=RPC_CUSTOM_TEXT +CUSTOM_STRINGS_MAX,
} ros_rpc_gen_command;


/**
 * Command enum for the xmlrpc message generator
 */

	//Starting number for field descriptions

#define HTTP_HEADER_DESC_BEGIN 	HTTP_HEADER_GEN_DESC_SERVER
#define HTTP_HEADER_VALUE_BEGIN HTTP_HEADER_GEN_VAL_POST_HTTP_1_1

typedef enum
{
	HTTP_HEADER_GEN_END=0, //!< This is required on any header generation array end.
	HTTP_HEADER_GEN_CUSTOM_TEXT_END, //!< This adds a linefeed after custom values
	///@{
	/**
	 * Values specifying most used standard field descriptors
	 *
	 *
	 */
	//To Editors: Define HTTP_HEADER_DESC_BEGIN must point to the first enum value in this section
	HTTP_HEADER_GEN_DESC_SERVER,
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
	//To Editors: Define HTTP_HEADER_VALUE_BEGIN must point to the first enum value in this section
	//To Editors: The value HTTP_HEADER_GEN_DESC_CUSTOM+CUSTOM_STRINGS_MAX must be assigned
	//to the first value!!!
	HTTP_HEADER_GEN_VAL_POST_HTTP_1_1 = HTTP_HEADER_GEN_DESC_CUSTOM+CUSTOM_STRINGS_MAX,
	HTTP_HEADER_GEN_VAL_XMLRPC_ROSC_NODELIB,
	HTTP_HEADER_GEN_VAL_BASEHTTP_ROSC_NODELIB,
	HTTP_HEADER_GEN_VAL_TEXT_XML,
	HTTP_HEADER_GEN_VAL_HTTP_1_0,
	HTTP_HEADER_GEN_VAL_OK,
	///@}

	/**
	 * This command will use a custom text array as value
	 */
	HTTP_HEADER_GEN_VAL_CUSTOM = HTTP_HEADER_GEN_VAL_OK+CUSTOM_STRINGS_MAX,


	/**
	 * This command will add a number to the
	 * header.
	 *
	 * If HTTP_HEADER_UINT_NUMBER + X
	 * will result in "Content-Length: X", where
	 * the supplied X can be a unsigned integer.
	 *
	 */
	HTTP_HEADER_GEN_VAL_UINT_NUMBER = HTTP_HEADER_GEN_VAL_CUSTOM+CUSTOM_STRINGS_MAX
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
void str2buf(unsigned int *index, char* buffer, const char* str, str2buf_modes mode);

/**
 *	This function can be used to generate headers for a HTTP message.
 * 	@param[out] message_buffer The output buffer for the message
 * 	@param[in] gen_array The command array for generating messages.
 * 	@param[in] custom_string_array The array for custom strings
 * 	@return Message length
 */
int generateHTTPHeader(char* message_buffer, const http_head_gen_command* gen_array, const char **custom_string_array);

/**
 * This function generates a xmlrpc message
 * @param[out] message_buffer The output buffer for the message
 * @param[in] gen_array The command array for generating messages.
 * @param[in] custom_string_array The array for custom strings (topics, params etc.)
 * @return Message length
 */
int generateXML(char* message_buffer, const ros_rpc_gen_command* gen_array, const char **custom_string_array);


#endif /* MSG_GEN_H_ */
