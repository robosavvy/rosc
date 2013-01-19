
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "rosCnode/rosc_string_res/msg_strings.h"
#include "rosCnode/rosc_com_xml/msg_gen.h"
#include "rosCnode/rosc_com_base/eth.h"

const char *custom_msg_str[] =
{
	 "custom_tag",
	 "custom_text",
	 "http://myHost:"
};

ros_rpc_gen_command msg_gen_array[]=
{
RPC_STDTXT_XML_DEF,
RPC_TAG_METHODCALL,
	RPC_TAG_METHODNAME,
		RPC_STDTXT_HASPARAM,
	RPC_CT RPC_TAG_METHODNAME,
	RPC_TAG_PARAMS,
		RPC_TAG_PARAM,
			RPC_CUSTOM_TAG +0,
				RPC_CUSTOM_TEXT +1,
				RPC_UINT_NUMBER +900,
			RPC_CT RPC_CUSTOM_TAG +0,
		RPC_CT RPC_TAG_PARAM ,
	RPC_CT RPC_TAG_PARAMS,
RPC_CT RPC_TAG_METHODCALL,
RPC_GENERATOR_FINISH
};



http_head_gen_command http_gen_array[]=
{
	//![Standard header descriptor and value]
	HTTP_HEADER_GEN_DESC_USER_AGENT,			//User-Agent:
		HTTP_HEADER_GEN_VAL_XMLRPC_ROSC_NODELIB,	//XMLRPC ROSc-NodeLib
	//![Standard header descriptor and value]

	HTTP_HEADER_GEN_DESC_HOST,
		HTTP_HEADER_GEN_VAL_CUSTOM +2, 		   	//"http://myHost:" from custom array position 0
		HTTP_HEADER_GEN_VAL_UINT_NUMBER +11311,		//generate string number 11311
		HTTP_HEADER_GEN_CUSTOM_TEXT_END, 	   	//header line end

	HTTP_HEADER_GEN_DESC_CONTENT_TYPE,			//Content Type:
		HTTP_HEADER_GEN_VAL_TEXT_XML,			//text/xml

	HTTP_HEADER_GEN_END					//Empty Line(Header End)
};



int main()
{
	printf("\n %i",sendXMLMessage(0,msg_gen_array,http_gen_array,custom_msg_str));
	return 0;
}


