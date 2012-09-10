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
#include "rosxmosNode.h"

int main(void)
{
	char *custom_str[7] =
	{
			"narf\0",
			"gnampf",
			"ddd"
	};

	char buffer[1000];
	typedef char* mstr;
	ros_rpc_gen_command gen_array[]=
	{
	RPC_XML_DECLARATION,
		RPC_TAG_METHODCALL,
			RPC_TAG_METHODNAME,
				RPC_STDTEXT_HASPARAM,
				RPC_CLOSE_TAG,
			RPC_TAG_PARAMS,
				RPC_TAG_PARAM,
					RPC_CUSTOM_TAG+0,
						RPC_CUSTOM_TEXT+1,
					RPC_CLOSE_TAG,
				RPC_CLOSE_TAG,
			RPC_CLOSE_TAG,
		RPC_CLOSE_TAG,
		RPC_GENERATOR_FINISH
	};

	http_head_gen_command gen_array2[]=
	{
			HTTP_HEADER_GEN_DESC_USER_AGENT,
			HTTP_HEADER_GEN_VAL_XMLRPC_ROSC_NODELIB,
			HTTP_HEADER_GEN_DESC_CUSTOM+0,
			HTTP_HEADER_GEN_VAL_CUSTOM+1,
			HTTP_HEADER_GEN_DESC_CONTENT_LENGTH,
			HTTP_HEADER_VAL_UINT_NUMBER+56202,
			HTTP_HEADER_GEN_END
	};

	unsigned int buf_index=0;
	unsigned int gen_index=0;


	generateHeader(buffer,gen_array2,custom_str, &buf_index);
	generateXML(buffer, gen_array, custom_str, &gen_index, &buf_index);

	printf("%s",buffer);

	return 0;




}
