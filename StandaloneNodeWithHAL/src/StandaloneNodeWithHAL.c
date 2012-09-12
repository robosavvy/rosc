
#include <stdio.h>
#include <stdlib.h>
#include "rosCnode/msg_strings.h"
#include "rosCnode/rosCnode.h"
#include "debug/debugutilities.h"

int main(void)
{
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


	char buf_header[150];
	char buf_msg[850];

	ros_rpc_gen_command msg_gen_array[]=
	{
	RPC_XML_DECLARATION, 						//<?xml version="1.0" ?>
		RPC_TAG_METHODCALL, 					//<methodcall>
			RPC_TAG_METHODNAME, 	  			//<methodname>
				RPC_STDTEXT_HASPARAM, 			//hasParam
			RPC_CT RPC_TAG_METHODNAME,			//</methodname>
			RPC_TAG_PARAMS,   					//<params>
				RPC_TAG_PARAM,					//<param>
					RPC_CUSTOM_TAG+0,			//<custom_tag>
						RPC_CUSTOM_TEXT+1,		//custom_text
						RPC_UINT_NUMBER+900,    //900
					RPC_CT RPC_CUSTOM_TAG+0,    //</custom_tag>
				RPC_CT RPC_TAG_PARAM ,			//</param>
			RPC_CT RPC_TAG_PARAMS,				//</params>
		RPC_CT RPC_TAG_METHODCALL,				//</param>
		RPC_GENERATOR_FINISH					//Stop generator
	};

	unsigned int msglen=generateXML(buf_msg, msg_gen_array, custom_msg_str);

	/*
	 * I suggest using the following structure:
	 * Descriptor
	 * \t Value,
	 *
	 * and for custom values:
	 * Descriptor,
	 * \t Custom text or int
	 * \t Custom text or int
	 * \t Custom text end
	 *
	 * as seen here
	 */
	http_head_gen_command http_gen_array[]=
	{
		HTTP_HEADER_GEN_DESC_USER_AGENT,			//User-Agent:
			HTTP_HEADER_GEN_VAL_XMLRPC_ROSC_NODELIB,//XMLRPC ROSc-NodeLib

		//Hosturl with port number,
		HTTP_HEADER_GEN_DESC_HOST,
			HTTP_HEADER_GEN_VAL_CUSTOM+0, 		  //"http://myHost:" from custom array position 0
			HTTP_HEADER_GEN_VAL_UINT_NUMBER+11311,//generate string number 11311
			HTTP_HEADER_GEN_CUSTOM_TEXT_END, 	  //header line end

		HTTP_HEADER_GEN_DESC_CONTENT_TYPE,			//Content Type:
			HTTP_HEADER_GEN_VAL_TEXT_XML,			//text/xml

		HTTP_HEADER_GEN_DESC_CONTENT_LENGTH, 		//Content Length:
			HTTP_HEADER_GEN_VAL_UINT_NUMBER+msglen,	//Length of Message (generated:150)
			HTTP_HEADER_GEN_CUSTOM_TEXT_END, 		//header line end

		HTTP_HEADER_GEN_DESC_CUSTOM+1,				//custom_desc:
			HTTP_HEADER_GEN_VAL_CUSTOM+2,			//custom_val
			HTTP_HEADER_GEN_CUSTOM_TEXT_END, 		//header line end

		HTTP_HEADER_GEN_END							//Empty Line(Header End)
	};

	/*
	 * If the array for the custom strings isn't const add a cast to (const char**)
	 */
	unsigned int headerlen=generateHTTPHeader(buf_header,http_gen_array,(const char**)custom_header_str);

	//Just to have some output
	printbuffer(buf_header,headerlen);
	printbuffer(buf_msg,msglen);

	return 0;
}
