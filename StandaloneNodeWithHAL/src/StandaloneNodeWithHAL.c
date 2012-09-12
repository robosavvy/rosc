/*
 ============================================================================
 Name        : StandaloneNodeWithHAL.c
 Author      : Christian Holl
 Version     :
 Copyright   : SNCN_C
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "rosCnode/msg_strings.h"
#include "rosCnode/rosCnode.h"


int main(void)
{
	const char *custom_xml_str[] =
	{
		 "custom_desc",
		 "custom_val",
		 "custom_tag",
		 "custom_text",
	};

	 char *custom_header_str[] =
	{
			"http://myHost:",
	};


	char buf_header[150];
	char buf_xml[850];

	ros_rpc_gen_command xml_gen_array[]=
	{
	RPC_XML_DECLARATION,
		RPC_TAG_METHODCALL,
			RPC_TAG_METHODNAME,
				RPC_STDTEXT_HASPARAM,
			RPC_CT RPC_TAG_METHODNAME,
			RPC_TAG_PARAMS,
				RPC_TAG_PARAM,
					RPC_CUSTOM_TAG+2,
						RPC_CUSTOM_TEXT+3,
						RPC_UINT_NUMBER+900,
					RPC_CT RPC_CUSTOM_TAG+2,
				RPC_CT RPC_TAG_PARAM ,
			RPC_CT RPC_TAG_PARAMS,
		RPC_CT RPC_TAG_METHODCALL,
		RPC_GENERATOR_FINISH
	};



	http_head_gen_command http_gen_array[]=
	{
			HTTP_HEADER_GEN_DESC_USER_AGENT,     HTTP_HEADER_GEN_VAL_XMLRPC_ROSC_NODELIB,
			HTTP_HEADER_GEN_DESC_HOST,           HTTP_HEADER_GEN_VAL_CUSTOM+0, HTTP_HEADER_VAL_UINT_NUMBER+11311,HTTP_HEADER_CUSTOM_TEXT_END,
			HTTP_HEADER_GEN_DESC_CONTENT_TYPE,   HTTP_HEADER_GEN_VAL_TEXT_XML,
			HTTP_HEADER_GEN_DESC_CONTENT_LENGTH, HTTP_HEADER_VAL_UINT_NUMBER+generateXML(buf_xml, xml_gen_array, custom_xml_str),
			HTTP_HEADER_GEN_END
	};



	generateHTTPHeader(buf_header,http_gen_array,(const char**)custom_header_str);
	printf("%s\n",buf_header);
	printf("%s\n",buf_xml);

	return 0;




}
