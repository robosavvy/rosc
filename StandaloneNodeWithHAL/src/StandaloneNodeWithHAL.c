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
	unsigned int gen_array[]=
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
	//GENERATE_XML(buffer,gen_array,custom_str);
	unsigned int gen_index=0;
	unsigned int buf_index=0;
	generateXML(buffer, gen_array, custom_str, &gen_index, &buf_index);


//#define GENERATE_XML(BUFFER,BUFFER_INDEX,GEN_ARRAY,CUSTOM_STR_ARR)\
//	unsigned int __gen_index=0;\
//	__generateXML(BUFFER,GEN_ARRAY,CUSTOM_STR_ARR,&__gen_index,BUFFER_INDEX);\
//	printf("%s",buffer);
	return 0;
}
