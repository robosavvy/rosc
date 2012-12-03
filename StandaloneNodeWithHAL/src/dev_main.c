
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
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




//
//inline bool parseHTTPMethod2(const char **buffer)
//{
//	skipSpace(buffer);
//	int result;
//
//	result=seekString(buffer,http_methods,HTTP_METHODS_LEN,parse_separators[PARSE_METHOD_SEP_METHOD],1);
//
//	switch(result)
//	{
//	case HTTP_METHOD_METHOD_POST:
//		printf("POST\n");
//	break;
//	default:
//		printf("Method not supported\n");
//		return false;
//	break;
//	}
//	skipSpace(buffer);
//	if(**buffer!='/')
//	{
//		printf("Header malformed!\n");
//		return false;
//	}
//	else
//	{
//		(*buffer)++;
//	}
//	skipSpace(buffer);
//
//	result=seekString(buffer,http_methods,HTTP_METHODS_LEN,parse_separators[PARSE_METHOD_SEP_METHOD],1);
//	if(result==HTTP_METHOD_METHOD_HTTP)
//		printf("HTTP\n");
//	skipSpace(buffer);
//	if(**buffer!='/')
//	{
//		printf("Header malformed!\n");
//		return false;
//	}
//	else
//	{
//		(*buffer)++;
//	}
//	result=parseStringUInt(buffer,0);
//
//	if(result==1)
//	{
//		printf("HTTP Major Release 1\n");
//	}
//	else
//	{
//		printf("HTTP Release ERROR\n");
//		return 1;
//	}
//	if(**buffer!='.')
//	{
//		printf("Header malformed!\n");
//		return false;
//	}
//	else
//	{
//		(*buffer)++;
//	}
//
//	result=parseStringUInt(buffer,0);
//
//	if(result>0)
//	{
//		printf("HTTP Minor Release: %i",result);
//	}
//	else
//	{
//		printf("HTTP Release ERROR\n");
//		return 1;
//	}
//	skipSpace(buffer);
//
//	if(**buffer!='\n')
//	{
//		printf("Header malformed!\n");
//		return false;
//	}
//	else
//	{
//		(*buffer)++;
//	}
//
//	return true;
//}



int main()
{
	char* str=
			    "POST / HTTP/1.1\n"
	    		"User-Agent:askfhasdf\n"
	    		"Host: sdfd-10: 34534\n"
	    		"Content-Type: text/xml\n"
	    		"Content-Length:289\n\n"

	    		"<?xml version=\"1.0\"?>\n"
	    		"<methodCall><methodName>registerPublisher</methodName>\n"
	    		"<params><param><value>/PublishSubscribeTest</value></param><param><value>/rosout</value></param><param><value>rosgraph_msgs/Log</value></param><param><value>http://ROS:35552/</value></param></params></methodCall>";

	printf(" narf %i\n",parseHTTPMethod(&str));

	return 0;
}


typedef enum
{
	__XML_PARSE_EVENT_NO_EVENT=0, //! This is just for internal processing and should never occur

	XML_PARSE_EVENT_DOCUMENT_START, //!< Initialize event for result computing function

	__XML_PARSE_EVENT_TAG_OPEN_START,
	__RPC_XML_TAG_STRINGS(XML_PARSE_EVENT_OPEN), //!< one of these values will be transmitted if a known tag is found
	XML_PARSE_EVENT_TAG_OPEN_UNKNOWN, //!< If this is transmitted there is a new open

	__XML_PARSE_EVENT_TAG_CLOSE_START,
	__RPC_XML_TAG_STRINGS(XML_PARSE_EVENT_CLOSE), //!< one of these values will be transmitted if a known close tag is found
	XML_PARSE_EVENT_TAG_CLOSE_UNKNOWN,

	__XML_PARSE_EVENT_TAG_EMPTY,
	__RPC_XML_TAG_STRINGS(XML_PARSE_EVENT_EMPTY), //!< one of these values will be transmitted if a known empty tag (<tag/>) is found
	XML_PARSE_EVENT_TAG_EMPTY_UNKNOWN,

	__XML_PARSE_EVENT_KNOWN_ATTRIBUTES_START,
	__RPC_XML_ATTRIBUTE_STRINGS(XML_PARSE_EVENT), //!< one of these values will be transmitted if a known attribute ist found
	__XML_PARSE_EVENT_KNOWN_ATTRIBUTES_END,
	XML_PARSE_EVENT_UNKNOWN_ATTRIBUTE,

	XML_PARSE_EVENT_STRING, //!< This is sent when a string was found inside a tag



}parseEvents;


/**
 * The positive values given in this enum are the controlling signals for the XML
 * Parser. Any negative value will stop the parser and return the parsing function+
 * the negative value.
 */
typedef enum
{
	XML_PARSE_ACT_RETURN_GENERAL_FAILURE=0, //!< This will stop the parser and make it return 0

	XML_PARSE_ACT_GO_AHEAD=1, //!< This will advice the parser to go on with parsing

	 /**
	  * When this is sent to the parser it will skip
	  * everything which is inside the current tag
	  * (if this is not called after a tag, it has the
	  * same effect as GO_AHEAD)
	  */
	XML_PARSE_ACT_SKIP_TAG,      //!< XML_PARSE_ACT_SKIP_TAG

	/**
	 *  When this is sent to the parser it will skip the current attribute
	 *  (if no attribute was found before, this has the same effect as GO_AHEAD)
	 */
	XML_PARSE_ACT_SKIP_ATTRIBUTE,//!< XML_PARSE_ACT_SKIP_ATTRIBUTE

}parserAction;

void getParserState(unsigned int parseEvent, const char *buffer, int *parseAction)
{

}

unsigned int parseXML(const char **buffer, void (*parseHandler)(unsigned int parseEvent, const char *buffer, int *parserAction))
{

	bool parse=true;
	int event=__XML_PARSE_EVENT_NO_EVENT;
	int action=XML_PARSE_ACT_GO_AHEAD;
	char curchr;

	for(;parse;++(*buffer))
	{
		curchr=**buffer;
		if(curchr>0x1F)
		{

		}
		else
		{
			if(curchr>0x00) //Unusual white space character
			{

			}
			else
			{

			}
		}

		if(event!=__XML_PARSE_EVENT_NO_EVENT)
		{

			parseHandler(event, buffer, &action);
			event=__XML_PARSE_EVENT_NO_EVENT;
		}



	}

	return 0;
}




/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

int handle(int event)
{
	printf("Handler received %i\n", event);
	return true;
}

int main2()
{
	   char* str=
			    "POST / HTTP/1.1\n"
	    		"User-Agent:askfhasdf\n"
	    		"Host: sdfd-10: 34534\n"
	    		"Content-Type: text/xml\n"
	    		"Content-Length:289\n\n"

	    		"<?xml version=\"1.0\"?>\n"
	    		"<methodCall><methodName>registerPublisher</methodName>\n"
	    		"<params><param><value>/PublishSubscribeTest</value></param><param><value>/rosout</value></param><param><value>rosgraph_msgs/Log</value></param><param><value>http://ROS:35552/</value></param></params></methodCall>";

	unsigned int len=strlen(str);



	//stringParse(str,len, &handle, PARSER_BEGIN);

	const char *lala[]=
	{
			"ass",
			"dnampf",
			"gnampf1",
			"gnampf2",
			"narf",
			"sampf"
	};


	const char *buffer="narf";
	const char *buffer2="\n";


	int ret=seekString(&buffer,lala,sizeof(lala)/sizeof(char*),"<>\n ",true);
	printf("Word %i %c \n",ret, *(buffer));


	ret=seekString(&buffer2,lala,sizeof(lala)/sizeof(char*),"<>\n ",false);
	printf("Word %i %c \n",ret, *(buffer));


	return 0;
}

int main3(void)
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
	RPC_STDTXT_XML_DEF,

	RPC_TAG_METHODCALL,

		RPC_TAG_METHODNAME,
			RPC_STDTXT_HASPARAM,
		RPC_CT RPC_TAG_METHODNAME,

		RPC_TAG_PARAMS,

			RPC_TAG_PARAM,
				RPC_TAG_VALUE,
					RPC_CUSTOM_TEXT+0,
				RPC_CT RPC_TAG_VALUE,
			RPC_CT RPC_TAG_PARAM ,

			RPC_TAG_PARAM,
				RPC_TAG_VALUE,
					RPC_STDTXT_HASPARAM,
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








//	exit(0);


	   char* teststr=
			    "POST / HTTP/1.1\n"
	    		"User-Agent:askfhasdf\n"
	    		"Host: sdfd-10: 34534\n"
	    		"Content-Type: text/xml\n"
	    		"Content-Length:289\n\n"

	    		"<?xml version=\"1.0\"?>\n"
	    		"<methodCall><methodName>registerPublisher</methodName>\n"
	    		"<params><param><value>/PublishSubscribeTest</value></param><param><value>/rosout</value></param><param><value>rosgraph_msgs/Log</value></param><param><value>http://ROS:35552/</value></param></params></methodCall>";



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
