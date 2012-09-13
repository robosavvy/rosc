
#include <stdio.h>
#include <stdlib.h>
#include "rosCnode/msg_strings.h"
#include "rosCnode/rosCnode.h"
#include "debug/debugutilities.h"


#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>



int main(void)
{

	//Custom string arrays for the message and header generator
	//Arrays can also be non const. To surpress
	//the warning a type cast should be made when handing
	//them over to the functions.

	//custom strings (message)
	const char *custom_msg_str[] =
	{
		 "/PublishSubscribeTest",
	};

	//custom strings (header)
	 char *custom_header_str[] =
	{
		"http://localhost:",
		"custom_desc",
		"custom_val"
	};

	 //message and header output buffer
	char buf_header[150];
	char buf_msg[850];

	/* For the xml generator array I suggest using the following structure:
	 *
	 * Adding the RPC_CT macro infront of a tag creates a closing tag
	 */
//	<methodCall>
//		<methodName>hasParam</methodName>
//		<params>
//			<param>
//				<value>/PublishSubscribeTest</value>
//			</param>
//			<param>
//				<value>/tcp_keepalive</value>
//			</param>
//		</params>
//	</methodCall>
	ros_rpc_gen_command msg_gen_array[]=
	{
	RPC_XML_DECLARATION,

	RPC_TAG_METHODCALL,

		RPC_TAG_METHODNAME,
			RPC_STDTEXT_HASPARAM,
		RPC_CT RPC_TAG_METHODNAME,

		RPC_TAG_PARAMS,

			RPC_TAG_PARAM,
				RPC_TAG_VALUE,
					RPC_CUSTOM_TEXT+0,
				RPC_CT RPC_TAG_VALUE,
			RPC_CT RPC_TAG_PARAM ,

			RPC_TAG_PARAM,
				RPC_TAG_VALUE,
					RPC_STDTEXT_HASPARAM,
				RPC_CT RPC_TAG_VALUE,
			RPC_CT RPC_TAG_PARAM ,
		RPC_CT RPC_TAG_PARAMS,
	RPC_CT RPC_TAG_METHODCALL,
	RPC_GENERATOR_FINISH
	};

	unsigned int msglen=generateXML(buf_msg, msg_gen_array, custom_msg_str);

	/*
	 * And for the header generator array I suggest using the following structure:
	 * Descriptor
	 * \t Value,
	 *
	 * and for custom values:
	 * Descriptor,
	 * \t Custom text or int
	 * \t Custom text or int
	 * \t Custom text end
	 *
	 * as seen down here
	 */
	http_head_gen_command http_gen_array[]=
	{
		HTTP_HEADER_GEN_VAL_POST_HTTP_1_1,
		HTTP_HEADER_GEN_DESC_USER_AGENT,			//User-Agent:
			HTTP_HEADER_GEN_VAL_XMLRPC_ROSC_NODELIB,//XMLRPC ROSc-NodeLib

		HTTP_HEADER_GEN_DESC_HOST,
			HTTP_HEADER_GEN_VAL_CUSTOM+0, 		  //"http://myHost:" from custom array position 0
			HTTP_HEADER_GEN_VAL_UINT_NUMBER+11311,//generate string number 11311
			HTTP_HEADER_GEN_CUSTOM_TEXT_END, 	  //header line end

		HTTP_HEADER_GEN_DESC_CONTENT_TYPE,			//Content Type:
			HTTP_HEADER_GEN_VAL_TEXT_XML,			//text/xml


		HTTP_HEADER_GEN_DESC_CONTENT_LENGTH, 		//Content Length:
			HTTP_HEADER_GEN_VAL_UINT_NUMBER+msglen,	//Length of Message (generated:150)
			HTTP_HEADER_GEN_CUSTOM_TEXT_END, 		//header line end


		HTTP_HEADER_GEN_END							//Empty Line(Header End)
	};

	/*
	 * If the array for the custom strings isn't const add a cast to (const char**)
	 */
	unsigned int headerlen=generateHTTPHeader(buf_header,http_gen_array,(const char**)custom_header_str);

	//Just to have some output print the buffers to stdout
	printbuffer(buf_header,headerlen);
	printbuffer(buf_msg,msglen);



	int sockfd = 0, n = 0;
	    char recvBuff[1024];
	    struct sockaddr_in serv_addr;

	    memset(recvBuff, '0',sizeof(recvBuff));
	    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	    {
	        printf("\n Error : Could not create socket \n");
	        return 1;
	    }

	    memset(&serv_addr, '0', sizeof(serv_addr));

	    serv_addr.sin_family = AF_INET;
	    serv_addr.sin_port = htons(11311);

	    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	    {
	        printf("\n inet_pton error occured\n");
	        return 1;
	    }

	    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	    {
	       printf("\n Error : Connect Failed \n");
	       return 1;
	    }

//	    //printf("sending\n");
//	    char *teststr=buf_header;
//	    write(sockfd,teststr,headerlen);
//	    teststr=buf_msg;
//	    write(sockfd,teststr,msglen);
//
//char *teststr="POST / HTTP/1.1\n"  //CASE SENSITIVE!
//	    		"User-Agent:askfhasdf\n"  //whatever: doesn't give a shit
//	    		"Host: sdfd-10: 34534\n"    //wrong port: doesn't give a shit,
//	    		"Content-Type: text/xml\n"
//	    		"Content-Length:289\n\n"
//
//
//	    		"<?xml version=\"1.0\"?>\n";
//	    write(sockfd,teststr,strlen(teststr));

//	    printf("zzzzz!");
//	    sleep(4);

	   char* teststr="POST / HTTP/1.1\n"
	    		"User-Agent:askfhasdf\n"
	    		"Host: sdfd-10: 34534\n"
	    		"Content-Type: text/xml\n"
	    		"Content-Length:289\n\n"

	    		"<?xml version=\"1.0\"?>\n"
	    		"<methodCall><methodName>registerPublisher</methodName>\n"
	    		"<params><param><value>/PublishSubscribeTest</value></param><param><value>/rosout</value></param><param><value>rosgraph_msgs/Log</value></param><param><value>http://ROS:35552/</value></param></params></methodCall>";



	   exit(0);
	   write(sockfd,teststr,strlen(teststr));

	    printf("\n\n:::::::::::RESPONSE:::::::::::::\n\n");



	    while (1)
	    {
	    	n = read(sockfd, recvBuff, sizeof(recvBuff)-1);

	    	if(n>1)
	    	{
	    		recvBuff[n]=0;
	    		printf("%s",recvBuff);
	    		return 0;
	    	}
	    }

	    if(n < 0)
	    {
	        printf("\n Read error \n");
	    }





	return 0;
}
