/*
 *  Copyright by Synapticon GmbH (www.synapticon.com)  ©2012
 *
 *  rosCnode.h
 *
 *  This file is part of ROScNode Library.
 *
 *  ROScNode Library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ROScNode Library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ROScNode Library. If not, see <http://www.gnu.org/licenses/>.
 *
 *	File created by Christian Holl on 11.09.2012
 */

#include <stdio.h>
#include <stdlib.h>
#include "rosCnode/msg_strings.h"
#include "rosCnode/rosCnode.h"
#include "debug/debugutilities.h"

int main(void)
{

	//! [Generating arrays for custom strings]
	const char *custom_msg_str[] =
	{
		 "custom_tag",
		 "custom_text",
	};

	 char *custom_header_str[] =
	{
		"http://myHost:",
		"custom_desc",
		"custom_val"
	};
	//! [Generating arrays for custom strings]



	//! [Creating the buffers]
	char buf_header[150];
	char buf_msg[850];
	//! [Creating the buffers]

	//! [Commands for the XML generation]
	ros_rpc_gen_command msg_gen_array[]=
	{
	RPC_XML_DECLARATION, 					//<?xml version="1.0" ?>

	RPC_TAG_METHODCALL, 					//<methodcall>
		//! [Standard Tag and Text]
		RPC_TAG_METHODNAME, 	  			//<methodname>
			RPC_STDTEXT_HASPARAM, 			//hasParam
		RPC_CT RPC_TAG_METHODNAME,			//</methodname>
		//! [Standard Tag and Text]
		RPC_TAG_PARAMS,   				//<params>
			RPC_TAG_PARAM,				//<param>
				//! [Custom Tag and Text]
				RPC_CUSTOM_TAG +0,		//<custom_tag>
					RPC_CUSTOM_TEXT +1,	//custom_text
					RPC_UINT_NUMBER +900,   //900
				RPC_CT RPC_CUSTOM_TAG +0,   	//</custom_tag>
				//! [Custom Tag and Text]
			RPC_CT RPC_TAG_PARAM ,			//</param>
		RPC_CT RPC_TAG_PARAMS,				//</params>
	RPC_CT RPC_TAG_METHODCALL,				//</param>
	RPC_GENERATOR_FINISH					//Stop generator
	};
	//! [Commands for the XML generation]


	//![generating XML]
	unsigned int msglen=generateXML(buf_msg, msg_gen_array, custom_msg_str);
	//![generating XML]


	//![Commands for the header generation]
	http_head_gen_command http_gen_array[]=
	{
		//![Standard header descriptor and value]
		HTTP_HEADER_GEN_DESC_USER_AGENT,			//User-Agent:
			HTTP_HEADER_GEN_VAL_XMLRPC_ROSC_NODELIB,	//XMLRPC ROSc-NodeLib
		//![Standard header descriptor and value]

		HTTP_HEADER_GEN_DESC_HOST,
			HTTP_HEADER_GEN_VAL_CUSTOM +0, 		   	//"http://myHost:" from custom array position 0
			HTTP_HEADER_GEN_VAL_UINT_NUMBER +11311,		//generate string number 11311
			HTTP_HEADER_GEN_CUSTOM_TEXT_END, 	   	//header line end

		HTTP_HEADER_GEN_DESC_CONTENT_TYPE,			//Content Type:
			HTTP_HEADER_GEN_VAL_TEXT_XML,			//text/xml

		HTTP_HEADER_GEN_DESC_CONTENT_LENGTH, 			//Content Length:
			HTTP_HEADER_GEN_VAL_UINT_NUMBER +msglen,	//Length of Message (generated:150)
			HTTP_HEADER_GEN_CUSTOM_TEXT_END, 		//header line end

		//![Custom header descriptor and value]
		HTTP_HEADER_GEN_DESC_CUSTOM +1,				//custom_desc:
			HTTP_HEADER_GEN_VAL_CUSTOM +2,			//custom_val
			HTTP_HEADER_GEN_CUSTOM_TEXT_END, 		//header line end
		//![Custom header descriptor and value]

		HTTP_HEADER_GEN_END					//Empty Line(Header End)
	};
	//![Commands for the header generation]

	/*
	 * If the array for the custom strings isn't const add a cast to (const char**)
	 */
	//! [Generating the Header]
	unsigned int headerlen=generateHTTPHeader(buf_header,http_gen_array,(const char**)custom_header_str);
	//! [Generating the Header]



	//Just to have some output print the buffers to stdout
	//! [Just Printing]
	printbuffer(buf_header,headerlen);
	printbuffer(buf_msg,msglen);
	//! [Just Printing]
	return 0;
}
