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


//void narf(void *arg)
//{
//	char **gnampf=(char **)arg;
//
//	printf("%s",gnampf[0]);
//	printf("%s",gnampf[1]);
//	printf("%s",gnampf[2]);
//}

int main(void)
{
	char *custom_str[7] =
	{
			"narf\0",
			"gnampf",
			"ddd"
	};

	printf("0: %p\n",custom_str[0]);
	printf("1: %p\n",custom_str[1]);
	printf("2: %p\n",custom_str[2]);

	char buffer[1000];
	typedef char* mstr;
	unsigned int gen_array[]={
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
			RPC_CLOSE_TAG
			};
	GENERATE_XML(buffer,gen_array,custom_str);

	return 0;
}
