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
 * msg_gen.h
 *
 * Created on: 11.09.2012
 *     Author: Christian Holl
 */


/**
 *  TODO CHANGE DOCUMENTATION FOR NEW FUNCTION
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
#include <rosc/string_res/msg_strings.h>
#include <rosc/string_res/msg_strings.h>
#include <rosc/com_base/eth.h>

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

	__RPC_TAGS_START,
	__RPC_XML_TAG_STRINGS(RPC), //!< Places the available tag numbers inside this enum
	/**
	 * Use this number for selecting custom tag strings where RPC_CUSTOM_TAG is the first (0)
	 * string of your custom array and RPC_CUSTOM_TAG+X, tag X in your array
	 */
	RPC_CUSTOM_TAG,


	RPC_CLOSE_TAG=RPC_CUSTOM_TAG +CUSTOM_STRINGS_MAX,

	__RPC_STDTEXT_START=RPC_CLOSE_TAG +CUSTOM_STRINGS_MAX,	//!< Marks the start of the standard text strings
	__RPC_XML_STDTEXT(RPC), //!< Places the available standard text numbers inside this enum

	/**
	 * Use this number for selecting custom text strings where RPC_CUSTOM_TEXT is the first (0)
	 * string of your custom array and RPC_CUSTOM_TAG+X, tag X in your array
	 */
	RPC_CUSTOM_TEXT,

	/**
	 * This commands inserts a single char (useful for generating an IP address)
	 */
    RPC_SINGLE_CHAR,
    __RPC_SINGLE_CHAR_END=RPC_SINGLE_CHAR+256,


	/**
	 * This command inserts a unsigned integer into the message
	 */
	RPC_UINT_NUMBER=RPC_CUSTOM_TEXT +CUSTOM_STRINGS_MAX,


} ros_rpc_gen_command;


/**
 * Command enum for the xmlrpc message generator
 */
typedef enum
{
	HTTP_HEADER_GEN_END=0, //!< This is required on any header generation array end.

	__HTTP_HEADER_GEN_DESC_START,


	/**
	 * Values specifying most used standard field descriptors
	 */
	__HTTP_HEADER_DESCRIPTORS(HTTP_HEADER_GEN), //!< Places the available standard descriptor numbers inside this enum

	/**
	 * This specifies to use a text in the custom text array as descriptor
	 */
	HTTP_HEADER_GEN_DESC_CUSTOM, //!< Marks the start of the custom strings


	__HTTP_HEADER_GEN_VAL_START = HTTP_HEADER_GEN_DESC_CUSTOM+CUSTOM_STRINGS_MAX,

	/**
	 * Values specifying most used standard text pieces
	 */
	__HTTP_HEADER_STDTEXT(HTTP_HEADER_GEN), //!< Places the available standard value numbers inside this enum

	/**
	 * This command will use a custom text array as value
	 */
	HTTP_HEADER_GEN_VAL_CUSTOM,



	/**
	 * This command will insert a single char
	 */
	HTTP_HEADER_GEN_SINGLE_CHAR=HTTP_HEADER_GEN_VAL_CUSTOM+CUSTOM_STRINGS_MAX,

	/**
	 * This command will add a number to the
	 * header.
	 *
	 * If HTTP_HEADER_UINT_NUMBER + X
	 * will result in "Content-Length: X", where
	 * the supplied X can be a unsigned integer.
	 *
	 */
	HTTP_HEADER_GEN_VAL_UINT_NUMBER=HTTP_HEADER_GEN_SINGLE_CHAR+256,
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

typedef enum
{
	SEND_STATUS_OK,
	SEND_STATUS_ERROR,
}send_status_t;


send_status_t sendXMLMessage(port_id_t portID, const ros_rpc_gen_command* xml_gen_array, const http_head_gen_command* http_gen_array, const char **custom_string_array);


#endif /* MSG_GEN_H_ */
