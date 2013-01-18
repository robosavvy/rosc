
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

typedef enum
{
	SEND_STATUS_OK,
	SEND_STATUS_ERROR,
}send_status_t;

typedef enum
{
	XML_SM_OPTAIN_XML_SIZE,
	XML_SM_SEND_HTTP_HEADER,
	XML_SM_SEND_XML,
	XML_SM_SEND_FINISHED
}XML_SM_SEND_State;
typedef enum
{
	HTTP_SEND_CONTENT_LEN_DESC,
	HTTP_SEND_CONTENT_LEN_VALUE,
	HTTP_SEND_HEADER_SEPARATOR,
}HTTP_SEND_LEN_State;




int output(char c)
{
	printf("%c",c);
	return 1;
}

int output2(char c, int port)
{
	printf("%c",c);
	return 1;
}


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


send_status_t sendXMLMessage(port_id portID, const ros_rpc_gen_command* xml_gen_array, const http_head_gen_command* http_gen_array, const char **custom_string_array)
{
	XML_SM_SEND_State xml_state=XML_SM_OPTAIN_XML_SIZE;
	HTTP_SEND_LEN_State http_state=HTTP_SEND_CONTENT_LEN_DESC;
	const char *outstring;
	int outmode;
	unsigned int number;
	unsigned int xml_len=0;
	ros_rpc_gen_command* xml_gen_ptr=xml_gen_array;
	int header_command=*http_gen_array; //TODO REMOVE THIS AND USE THE DIRECT VARIABLE

	while(xml_state != XML_SM_SEND_FINISHED)
	{
		switch(xml_state)
		{
		case XML_SM_OPTAIN_XML_SIZE:
		case XML_SM_SEND_XML:
		{

			int command=*xml_gen_ptr;
			/*
			 * UNSIGNED INTEGER
			 */
			if(command>=RPC_UINT_NUMBER)
			{
				number=command-RPC_UINT_NUMBER;
				outmode=-1;
			}
			/*
			 * TEXT FIELD
			 */
			else if(command>=__RPC_STDTEXT_START-1) //Add text
			{
				if(command>=RPC_CUSTOM_TEXT)
				{
					outstring=custom_string_array[command-RPC_CUSTOM_TEXT];
					outmode=S2B_NORMAL;
				}
				else
				{
					outstring=rpc_xml_stdtext[command-__RPC_STDTEXT_START-1];
					outmode=S2B_NORMAL;
				}
			}
			/*
			 * CLOSING TAG
			 */
			else if(command>=RPC_CLOSE_TAG)
			{
				if(command>=RPC_CUSTOM_TAG+RPC_CLOSE_TAG)
				{
					outstring=custom_string_array[command-RPC_CUSTOM_TAG-RPC_CLOSE_TAG];
					outmode=S2B_CTAG;
				}
				else
				{
					outstring=rpc_xml_tag_strings[command-__RPC_TAGS_START-1-RPC_CLOSE_TAG];
					outmode=S2B_CTAG;
				}
			}
			/*
			 * OPENING TAG
			 */
			else if(command>=__RPC_TAGS_START-1)
			{
				if(command>=RPC_CUSTOM_TAG)
				{
					outstring=custom_string_array[command-RPC_CUSTOM_TAG];
					outmode=S2B_TAG;
				}
				else
				{
					outstring=rpc_xml_tag_strings[command-__RPC_TAGS_START-1];
					outmode=S2B_TAG;
				}
			}



			if(command==RPC_GENERATOR_FINISH)
			{
				if(xml_state==XML_SM_OPTAIN_XML_SIZE)
				{
					//Reset it array pointer back to the start for generation
					xml_gen_ptr=xml_gen_array;
					xml_state=XML_SM_SEND_HTTP_HEADER;
				}
				else if(xml_state==XML_SM_SEND_XML)
				{
					xml_state=XML_SM_SEND_FINISHED;
				}

				continue;
			}

			xml_gen_ptr++;
			command=*xml_gen_ptr;
		}


		break;
	case XML_SM_SEND_HTTP_HEADER:



			if(header_command == HTTP_HEADER_GEN_CUSTOM_TEXT_END)
			{
				outmode=S2B_NORMAL;
				outstring="\n";
			}
			else if(header_command>=HTTP_HEADER_GEN_VAL_UINT_NUMBER)
			{
				number=header_command-HTTP_HEADER_GEN_VAL_UINT_NUMBER;
				//int2buf(message_buffer,&buf_index,number);
				outmode=-1;
			}
			else if(header_command>=HTTP_HEADER_GEN_VAL_CUSTOM) //Print custom value, no newline...
			{
				//str2buf(&buf_index,message_buffer,custom_string_array[header_command-HTTP_HEADER_GEN_VAL_CUSTOM],S2B_NORMAL);
				outstring=custom_string_array[header_command-HTTP_HEADER_GEN_VAL_CUSTOM];
				outmode=S2B_NORMAL;
			}
			else if(header_command>=__HTTP_HEADER_GEN_VAL_START) //Print std value
			{
				//str2buf(&buf_index,message_buffer,http_header_stdtext[header_command-HTTP_HEADER_VALUE_BEGIN],S2B_HTTP_HEAD_FIELD);
				outstring=http_header_stdtext[header_command-__HTTP_HEADER_GEN_VAL_START-1];
				outmode=S2B_HTTP_HEAD_FIELD;
			}
			else if(header_command>=HTTP_HEADER_GEN_DESC_CUSTOM) //Print custom descriptor
			{
				//str2buf(&buf_index,message_buffer,custom_string_array[header_command-HTTP_HEADER_GEN_DESC_CUSTOM],S2B_HTTP_HEAD_FIELD_DESC);
				outstring=custom_string_array[header_command-HTTP_HEADER_GEN_DESC_CUSTOM];
				outmode=S2B_HTTP_HEAD_FIELD_DESC;
			}
			else if(header_command>=__HTTP_HEADER_GEN_DESC_START) //Print std descriptor
			{
				//str2buf(&buf_index,message_buffer,http_header_descriptors[header_command-HTTP_HEADER_DESC_BEGIN],S2B_HTTP_HEAD_FIELD_DESC);
				outstring=http_header_descriptors[header_command-__HTTP_HEADER_GEN_DESC_START-1];
				outmode=S2B_HTTP_HEAD_FIELD_DESC;
			}



			if(header_command==HTTP_HEADER_GEN_END)
			{
				switch (http_state) {
					case HTTP_SEND_CONTENT_LEN_DESC:
						outstring=http_header_descriptors[HTTP_HEADER_GEN_DESC_CONTENT_LENGTH-__HTTP_HEADER_GEN_DESC_START-1];
						outmode=S2B_HTTP_HEAD_FIELD_DESC;
						http_state=HTTP_SEND_CONTENT_LEN_VALUE;
						break;
					case HTTP_SEND_CONTENT_LEN_VALUE:
						number=xml_len;
						outmode=-1;
						http_state=HTTP_SEND_HEADER_SEPARATOR;
						break;
					case HTTP_SEND_HEADER_SEPARATOR:
						outstring="\n\n";
						outmode=HTTP_HEADER_GEN_VAL_CUSTOM;
						xml_state=XML_SM_SEND_XML;
						break;

					default:
						break;
				}

			}
			else
			{
				http_gen_array++;
				header_command=*http_gen_array;
			}
		break;

	}

	if(outmode<0)
	{
		unsigned int a=10;
		while(number/a)a*=10;
		a/=10;
		while(a>0)
		{
			if(xml_state==XML_SM_OPTAIN_XML_SIZE) xml_len++;
			else output((char)(number/a)+48); //TODO change this to sending function with port
			number%=a;
			a/=10;
		}
	}
	else
	{
			int s;
			char *strs[3];
			strs[1]=(char*)outstring;
			switch(outmode)
			{
			case S2B_TAG:
				strs[0]="<";
				strs[2]=">";
				break;
			case S2B_CTAG:
				strs[0]="</";
				strs[2]=">";
				break;
			case S2B_HTTP_HEAD_FIELD_DESC:
				strs[0]="";
				strs[2]=": ";
				break;
			case S2B_HTTP_HEAD_FIELD:
				strs[0]="";
				strs[2]="\n";
				break;
			case S2B_NORMAL:
				strs[0]="";
				strs[2]="";
				break;
			}
			for(s=0;s<3;++s)
			{
				while (*strs[s] != '\0')
				{
					if(xml_state==XML_SM_SEND_XML || xml_state==XML_SM_SEND_HTTP_HEADER) output(*strs[s]); //TODO change this to sending function with port
					else xml_len++;
					strs[s]++;
				}
			}
	}







	}
	return SEND_STATUS_OK;
}



int main()
{
	printf("\n %i",sendXMLMessage(0,msg_gen_array,http_gen_array,custom_msg_str));
	return 0;
}


