/*
 *	Copyright (c) 2013, Synapticon GmbH
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions are met:

 *
 *	1. Redistributions of source code must retain the above copyright notice, this
 *	   list of conditions and the following disclaimer. 
 *	2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution. 
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *	ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *	The views and conclusions contained in the software and documentation are those
 *	of the authors and should not be interpreted as representing official policies, 
 *	either expressed or implied, of the FreeBSD Project.
 *
 *  msg_gen_handler.c created by Christian Holl
 */

#include <rosc/sebs_parse_fw/gen_modules/msg_gen_handler.h>
#include <rosc/sebs_parse_fw/gen_modules/size_deter.h>
#include <rosc/sebs_parse_fw/gen_modules/buffer_fill.h>

typedef enum
{
	MSG_GEN_STATE_PROGRESS_BUILDUP,
	MSG_GEN_STATE_DATA_TO_BUFFER,
	MSG_GEN_STATE_STRING_NUMBER_TO_BUFFER,
	MSG_GEN_STATE_GET_PAYLOAD_SIZE,
	MSG_GEN_STATE_GET_ROSRPC_FIELD_SIZE,
	MSG_GEN_STATE_END
}msg_gen_state_t;


typedef enum
{
	MSG_GEN_NUMBERTYPE_FLOAT,
	MSG_GEN_NUMBERTYPE_INT,
	MSG_GEN_NUMBERTYPE_UINT,
}msg_gen_state_numbertype_t;

#define GET_PAYLOAD_SIZE\
	state=MSG_GEN_STATE_GET_PAYLOAD_SIZE;\
	size.data=data;\
	size.type=type

#define GET_ROSRPC_FIELD_SIZE\
	state=MSG_GEN_STATE_GET_ROSRPC_FIELD_SIZE;\
	size.data=data;\
	size.type=type

#define DATA_TO_BUFFER(DATA, SIZE, CORRECTION)\
		state=MSG_GEN_STATE_DATA_TO_BUFFER;\
		out.data=(const char *) ((const void*)DATA);\
		out.curByte=0;\
		out.size=SIZE;\
		out.correct=CORRECTION;\

#define STRING_TO_BUFFER(DATA)\
		state=MSG_GEN_STATE_DATA_TO_BUFFER;\
		out.curByte=0;\
		out.size=0;\
		out.data=(const char *) DATA;\

#define NUMBER_TO_BUFFER(DATA, TYPE, BYTESIZE)\
		state=MSG_GEN_STATE_STRING_NUMBER_TO_BUFFER;\
		out.curByte=0;\
		out.data=(const char *) DATA;\
		out.ntype=TYPE;\
		out.size=BYTESIZE;

#define BUFFER_NEXT_BYTE\
		--buf.size;\
		++buf.ptr

#define BUFFER_RESET\
		buf.size=buffer_size;\
		buf.ptr=buffer

#define BYTE_TO_BUFFER(BYTE)\
	out.single_chr=BYTE;\
	out.data=&out.single_chr;\
	out.size=1;\


#define NEXT_CONTENT ++data
#define NEXT_BUILDUP ++type


void send_rpc(uint8_t * const buffer, uint32_t buffer_size, const msg_gen_command_t *def)
{

	uint8_t def_state=0;
	msg_gen_state_t state=MSG_GEN_STATE_PROGRESS_BUILDUP;
	const msg_gen_type_t *type=def->type;
	const void** data=def->data;

	struct
	{
		const char* data;
		uint32_t size;
		uint32_t curByte;
		union
		{
			char single_chr;
			uint8_t correct;
			msg_gen_state_numbertype_t ntype;
		};
	}out;

	struct
	{
		bool payload_size_available;
		const msg_gen_type_t *type;
		const void **data;
		uint32_t payload_size;
		uint32_t rosrpc_size;
	}size={false};

	struct
	{
		uint32_t size;
		uint8_t *ptr;
	}buf={buffer_size,buffer};

	while(1)
	{
		if(buf.size==0 || state==MSG_GEN_STATE_END)
		{
					int byte;
					printf("current package: ");
					for (byte = 0; byte < buffer_size-buf.size; ++byte)
					{
						if((buffer[byte] >= ' ' && buffer[byte]<= '~') || buffer[byte] == '\n')
						{
							printf("%c", buffer[byte]);
						}
						else
						{
							printf("[%x]", (unsigned int) buffer[byte]);
						}
					}
					printf("\n");

				if(state==MSG_GEN_STATE_END)
				{
					//End Loop!
					break;
				}
				else
				{
					BUFFER_RESET;
				}
		};


		switch(state)
		{
			case MSG_GEN_STATE_PROGRESS_BUILDUP:
				switch(*type)
				{
					case __MSG_TYPE_NONE:
					case __MSG_TYPE_DESCRIPTORS:
					case __MSG_TYPE_BINARY_OUT:
					case __MSG_TYPE_FLOAT_STRING:
					case __MSG_TYPE_INT_STRING:
					case __MSG_TYPE_ROSRPC_FIELD_STRINGS:
					case __MSG_TYPE_UINT_STRING:
					case __MSG_TYPE_XMLRPC_CLOSE_TAGS:
					case __MSG_TYPE_XMLRPC_OPEN_TAGS:
					DEBUG_PRINT_STR("msg_gen_handler: FOUND SYMBOL NOT TO BE USED AS MESSAGE DEFINITION!");
					break;

					case MSG_TYPE_PAYLOAD_SIZE_STRING:
					case MSG_TYPE_PAYLOAD_SIZE_BINARY:
						if(!size.payload_size_available) //Was the payload size already calculated?
						{//No, then do it now...
							GET_PAYLOAD_SIZE;
						}
						else
						{
							if(*type==MSG_TYPE_PAYLOAD_SIZE_BINARY)
							{
								DATA_TO_BUFFER(&size.payload_size,4,4);
							}
							else
							{
								NUMBER_TO_BUFFER(&size.payload_size,MSG_GEN_NUMBERTYPE_UINT,4);
							}
						}
					break;

					case MSG_TYPE_PAYLOAD_SIZE_START:
						GET_PAYLOAD_SIZE;
					break;

					case MSG_TYPE_STRING:
						STRING_TO_BUFFER(data);
						NEXT_BUILDUP;
						NEXT_CONTENT;
					break;

					case MSG_TYPE_DESCRIPTOR_END:
						BYTE_TO_BUFFER('\n');
						NEXT_BUILDUP;
						break;

					case MSG_TYPE_DESCRIPTOR_HTTP_HEADER_END:
						BYTE_TO_BUFFER('\n');
						NEXT_BUILDUP;
						break;

					case MSG_TYPE_ROSRPC_FIELD_END:
						NEXT_BUILDUP;
					break;

					case MSG_TYPE_MESSAGE_END:
						//NOTHING TO DO -> END
					break;

					default:
					//Do not change the order here! Otherwise the order must be change in msg_gen_type_t as well!
					if(*type>__MSG_TYPE_ROSRPC_FIELD_STRINGS)
					{
						switch(def_state)
						{
							case 0://Determine field size
								def_state=1;
								GET_ROSRPC_FIELD_SIZE;
								break;
							case 1://Write field size
								def_state=2;
								DATA_TO_BUFFER(&size.rosrpc_size,4,4);
								break;
							case 2://id string
								def_state=3;
								STRING_TO_BUFFER(ros_field_strings[*type - __MSG_TYPE_ROSRPC_FIELD_STRINGS -1]);
								break;
							case 3://equal
								def_state=0;
								BYTE_TO_BUFFER('=');
								NEXT_BUILDUP;
								break;
						}
					}
					else if(*type>__MSG_TYPE_XMLRPC_CLOSE_TAGS ||
							*type>__MSG_TYPE_XMLRPC_OPEN_TAGS)
					{
						switch(def_state)
						{
							case 0:// '<'
								if(*type>__MSG_TYPE_XMLRPC_CLOSE_TAGS)
								{
									def_state=1;
								}
								else
								{
									def_state=2;
								}

								BYTE_TO_BUFFER('<');
								break;
							case 1://   '/'
								def_state=2;
								BYTE_TO_BUFFER('/');
								break;
							case 2://id string
								def_state=3;
								if(*type>__MSG_TYPE_XMLRPC_CLOSE_TAGS)
								{
									STRING_TO_BUFFER(xmlrpc_tag_strings[*type - __MSG_TYPE_XMLRPC_CLOSE_TAGS -1]);
								}
								else
								{
									STRING_TO_BUFFER(xmlrpc_tag_strings[*type - __MSG_TYPE_XMLRPC_OPEN_TAGS -1]);
								}
								break;
							case 3:// '>'
								def_state=0;
								BYTE_TO_BUFFER('>');
								NEXT_BUILDUP;
								break;
						}

					}

					else if(*type>__MSG_TYPE_DESCRIPTORS)
					{
						switch(def_state)
						{
							case 0:// DESCRIPTOR
								def_state=1;
								STRING_TO_BUFFER(xmlrpc_http_descriptors[*type - __MSG_TYPE_DESCRIPTORS -1]);
								break;

							case 1:// ':'
								def_state=2;
								BYTE_TO_BUFFER(':');
								break;

							case 2:// ' '
								def_state=0;
								BYTE_TO_BUFFER(' ');
								NEXT_BUILDUP;
								break;
						}
					}
					else if(*type>__MSG_TYPE_UINT_STRING)
					{
						NUMBER_TO_BUFFER(*data, MSG_GEN_NUMBERTYPE_UINT, *type - __MSG_TYPE_UINT_STRING -1);
					}
					else if(*type>__MSG_TYPE_INT_STRING)
					{
						NUMBER_TO_BUFFER(*data, MSG_GEN_NUMBERTYPE_INT, *type - __MSG_TYPE_INT_STRING -1);
					}
					else if(*type>__MSG_TYPE_FLOAT_STRING)
					{
						NUMBER_TO_BUFFER(*data, MSG_GEN_NUMBERTYPE_FLOAT,  *type - __MSG_TYPE_FLOAT_STRING -1);
					}
					else if(*type>__MSG_TYPE_BINARY_OUT)
					{
						DATA_TO_BUFFER(*data,*type - __MSG_TYPE_BINARY_OUT -1, *type - __MSG_TYPE_BINARY_OUT -1);
					}
					else
					{
						DEBUG_PRINT(INT,"msg_gen_handler UNHANDLED STATE:", *type);
					}
					break;

				}
				break;


			case MSG_GEN_STATE_DATA_TO_BUFFER:
				{
					int8_t const *correct;
					if(out.size>1)
						switch(out.correct)
						{
							case 2:
								correct=g_byte_order_correction_to_system->SIZE_2_B; //to_system because we catch it from a systems variable with the correction
								break;												 //if I am wrong change to network :-)
							case 4:
								correct=g_byte_order_correction_to_system->SIZE_4_B;
								break;
							case 8:
								correct=g_byte_order_correction_to_system->SIZE_8_B;
								break;
							default:
								//ROSC_FATAL("BUFFER_FILL: NO CORRECTION FOR THAT BYTE SIZE!!!!");
								break;
						}

					while(buf.size>0)
					{
						if(correct && out.size>0) //==0 would be string terminated with 0
						{
							if(out.size>0)
							{
								*buf.ptr=*((char *)(out.data
										               +(out.curByte/out.correct)
										               +correct[out.curByte%out.correct]));
								BUFFER_NEXT_BYTE;
							}
							else
							{
								state=MSG_GEN_STATE_PROGRESS_BUILDUP;
								break;
							}
						}
						else
						{
							if(out.curByte<out.size || (out.size==0 && (*((char *)out.data) != '\0')) )
							{
								*buf.ptr=*((char *)out.data);

								++out.data;
								++out.curByte;
								BUFFER_NEXT_BYTE;
							}
							else
							{
								state=MSG_GEN_STATE_PROGRESS_BUILDUP;
								break;
							}
						}

					}

				}
				break;


			case MSG_GEN_STATE_GET_PAYLOAD_SIZE:
			case MSG_GEN_STATE_GET_ROSRPC_FIELD_SIZE:
			{

				//Size output
				uint32_t *s;
				const char *add_string=0;

				if(state==MSG_GEN_STATE_GET_PAYLOAD_SIZE)
				{
					size.payload_size_available=true;
					s=&size.payload_size;
					while(*size.type==MSG_TYPE_PAYLOAD_SIZE_START)
						++size.type;
				}
				else
				{
					s=&size.rosrpc_size;
				}

				//loop while not the message or payload end or rosrpc field end..
				while(*size.type!=MSG_TYPE_PAYLOAD_END
				   && *size.type!=MSG_TYPE_MESSAGE_END
				   && *size.type!=MSG_TYPE_ROSRPC_FIELD_END)
				{
					switch(*size.type)
					{
						case __MSG_TYPE_NONE:
						case __MSG_TYPE_DESCRIPTORS:
						case __MSG_TYPE_BINARY_OUT:
						case __MSG_TYPE_FLOAT_STRING:
						case __MSG_TYPE_INT_STRING:
						case __MSG_TYPE_ROSRPC_FIELD_STRINGS:
						case __MSG_TYPE_UINT_STRING:
						case __MSG_TYPE_XMLRPC_CLOSE_TAGS:
						case __MSG_TYPE_XMLRPC_OPEN_TAGS:
						DEBUG_PRINT_STR("msg_gen_handler - size det: FOUND SYMBOL NOT TO BE USED AS MESSAGE DEFINITION!");
						break;

						case MSG_TYPE_PAYLOAD_SIZE_STRING:
						case MSG_TYPE_PAYLOAD_SIZE_BINARY:
							*s+=4;
						break;

						case MSG_TYPE_PAYLOAD_SIZE_START:
						case MSG_TYPE_ROSRPC_FIELD_END:
						case MSG_TYPE_MESSAGE_END:
							//Nothing to do..
						break;

						case MSG_TYPE_STRING:

						break;

						case MSG_TYPE_DESCRIPTOR_END:
							*s+=1;
							break;

						case MSG_TYPE_DESCRIPTOR_HTTP_HEADER_END:
							*s+=1;
							break;


						default:
						//Do not change the order here! Otherwise the order must be change in msg_gen_type_t as well!
						if(*size.type>__MSG_TYPE_ROSRPC_FIELD_STRINGS)
						{
							s+=5; //4 bytes size + '='
						}
						else if(*size.type>__MSG_TYPE_XMLRPC_CLOSE_TAGS || *size.type>__MSG_TYPE_XMLRPC_OPEN_TAGS)
						{
							if(*size.type>__MSG_TYPE_XMLRPC_CLOSE_TAGS)
							{
								s+=3;
								add_string=xmlrpc_tag_strings[*size.type - __MSG_TYPE_XMLRPC_CLOSE_TAGS -1];
							}
							else
							{
								s+=2;
								add_string=xmlrpc_tag_strings[*size.type - __MSG_TYPE_XMLRPC_OPEN_TAGS -1];
							}
						}

						else if(*size.type>__MSG_TYPE_DESCRIPTORS)
						{
							#if 0 	//Normally this should not be necessary because the http descriptors do not belong
									//to the payload of an http message, switch off to save memory...
									s+=2; // ' ' ':'
									add_string=xmlrpc_http_descriptors[*size.type - __MSG_TYPE_XMLRPC_CLOSE_TAGS -1];
							#endif
						}
						else if(*size.type>__MSG_TYPE_UINT_STRING || *size.type>__MSG_TYPE_INT_STRING)
						{


							uint8_t s, i;
							bool sign=false;
							uint64_t var_uint;


							if(*size.type>__MSG_TYPE_UINT_STRING)
							{
								s=(1<<*size.type-__MSG_TYPE_UINT_STRING-1);
							}
							else
							{
								sign=true;
								s=(1<<*size.type-__MSG_TYPE_INT_STRING-1);
							}





						}
						else if(*size.type>__MSG_TYPE_FLOAT_STRING)
						{

						}
						else if(*size.type>__MSG_TYPE_BINARY_OUT)
						{
							//Shift for 2^x
							//8421
							//   1<<x
							s+=(1<<*size.type - __MSG_TYPE_BINARY_OUT -1);
						}
						else
						{
							DEBUG_PRINT(INT,"msg_gen_handler UNHANDLED STATE:", *size.type);
						}
						break;
					}


					//count string size
					if(add_string!=0)
					{
						while(*add_string!='\0')
						{
							++add_string;
							++*s;
						}
						add_string=0;
					}

					//Next type
					++size.type;
				}
			}
				break;
			case MSG_GEN_STATE_END:
				break;
		}

	}


}

