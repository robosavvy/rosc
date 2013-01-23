
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

#include <rosc/com_xml/rpc_request_msg.h>
#include <rosc/com_base/ports.h>

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
				RPC_SINGLE_CHAR,
			RPC_CT RPC_CUSTOM_TAG +0,
		RPC_CT RPC_TAG_PARAM ,
	RPC_CT RPC_TAG_PARAMS,
RPC_CT RPC_TAG_METHODCALL,
RPC_GENERATOR_FINISH
};


ros_rpc_gen_command msg[]=
{
		RPC_STDTXT_XML_DEF,
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


ROSC_SYSTEM_SETTING("/rosc_node",IP_ADDR(192,168,101,59),IP_ADDR(192,168,101,59),11311);

int main()
{
	char buf[1024];
	port_processing_t a,b;
	a.nextPort=&b;
	a.portID=2;
	b.nextPort=&a;

	uint16_t localport;

	port_id_t id_in_pub=listenPort(50101);
	port_id_t id_out=connectServer(master_ip,11311, &localport);

	char *topic="/lala";
	char *messagetype="std_msgs/UInt8";

	const char *msg_strings[]=XMLRPC_MSGSTR_REGISTERPUBSUB_CUSTOM_STRING_ARRAY(topic, messagetype);



	http_head_gen_command http_header[]=XMLRPC_HTTP_HEADER_REQUEST(192,168,0,1,11311);
	ros_rpc_gen_command xml_message[]=XMLRPC_MSG_REGISTERPUBLISHER(50101);


	memset(buf,0,1024);
	sendXMLMessage(id_out,xml_message, http_header,msg_strings);
	if(receiveFromPort(id_out,buf,1024)>0)			printf("OUT: %s",buf);

	topic="/lala2";

	const char *msg_strings2[]=XMLRPC_MSGSTR_REGISTERPUBSUB_CUSTOM_STRING_ARRAY(topic, messagetype);


	closeConnection(id_out);
	id_out=connectServer(master_ip,11311, &localport);

	ros_rpc_gen_command xml_message2[]=XMLRPC_MSG_REGISTERSUBSCRIBER(50101);

	memset(buf,0,1024);
	sendXMLMessage(id_out,xml_message2, http_header,msg_strings2);
	if(receiveFromPort(id_out,buf,1024)>0)			printf("OUT: %s",buf);
	closeConnection(id_out);

	sleep(1);






		while(1)
		{

			port_id_t con=acceptConnectionOnPort(id_in_pub);
			memset(buf,0,1024);
			if(con>0)
			{
				printf("connection!\n");
				int rec=receiveFromPort(con,buf,1024);
				if(rec>0)
				{
					printf("IN_pub\n\n\n\n >>>>>>>>>  %s",buf);

					sendToPort(con,"T",1);
				}
				printf("%i\n",rec);
				if(rec==0)
				{
					printf("Connection Close\n");
					close(con);
				}
			}
			usleep(100);
		}

	return 0;




}


