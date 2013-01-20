
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

#include <rosc/com_xml/general_msg.h>
#include <rosc/com_base/ports.h>


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

	HTTP_HEADER_GEN_DESC_CONTENT_TYPE,			//Content Type:
		HTTP_HEADER_GEN_VAL_TEXT_XML,			//text/xml

	HTTP_HEADER_GEN_END					//Empty Line(Header End)
};

ROSC_SYSTEM_SETTING("rosc_node",IP_ADDR(192,168,0,1),IP_ADDR(192,168,0,2),11311);

//ros_rpc_gen_command xmlrpc_master_keepalive_msg[]=
//{
//RPC_STDTXT_XML_DEF,
//RPC_TAG_METHODCALL,
//	RPC_TAG_METHODNAME,
//		RPC_STDTXT_HASPARAM,
//	RPC_CT RPC_TAG_METHODNAME,
//	RPC_TAG_PARAMS,
//		RPC_TAG_PARAM,
//			RPC_STDTXT_TCP_KEEPALIVE,
//		RPC_CT RPC_TAG_PARAM ,
//	RPC_CT RPC_TAG_PARAMS,
//RPC_CT RPC_TAG_METHODCALL,
//RPC_GENERATOR_FINISH
//};



int main()
{

	port_processing_t a,b;
	a.nextPort=&b;
	a.portID=2;
	b.nextPort=&a;

	http_head_gen_command xmlrpc_http_std_header_2[]=XMLRPC_REQUEST_HTTP_HEADER();

	ros_rpc_gen_command xmlrpc_keepalive[]=XMLRPC_KEEPALIVE();

	auto_aquire_system_ip();
	printf("\n %i",sendXMLMessage(0,xmlrpc_keepalive, xmlrpc_http_std_header_2,custom_msg_str));
	return 0;
}


