#include <rosc/com/msg_gen.h>
#include <rosc/system/status.h>

typedef enum
{
	MSG_GEN_MODE_TYPE,
	MSG_GEN_MODE_BUFFER_FILL,
	MSG_GEN_MODE_NUMBER_TO_STRING,
	MSG_GEN_MODE_STRING_SIZE,
} msg_gen_mode_t;

typedef enum
{
	MSG_GEN_SIZE_MODE_NONE = 0,
	MSG_GEN_SIZE_MODE_PAYLOAD_START,
	MSG_GEN_SIZE_MODE_ROSFIELD_START,
	MSG_GEN_SIZE_MODE_PAYLOAD,
	MSG_GEN_SIZE_MODE_ROSFIELD,
} msg_gen_size_mode;

typedef enum
{
	MSG_GEN_NUMBERTYPE_FLOAT, MSG_GEN_NUMBERTYPE_INT, MSG_GEN_NUMBERTYPE_UINT,
} msg_gen_state_numbertype_t;

#define DATA_TO_BUFFER(DATA, SIZE, CORRECTION)\
		submode=MSG_GEN_MODE_BUFFER_FILL;\
		out.data=(const char *) ((const void*)DATA);\
		out.curPos=0;\
		out.size=SIZE;\
		out.correct=CORRECTION;\

#define STRING_TO_BUFFER(DATA)\
		submode=MSG_GEN_MODE_BUFFER_FILL;\
		out.curPos=0;\
		out.size=0;\
		out.data=(const char *) DATA;\

#define NUMBER_TO_BUFFER(DATA, TYPE, BYTESIZE)\
		submode=MSG_GEN_MODE_NUMBER_TO_STRING;\
		out.curPos=0;\
		out.data=(const char *) DATA;\
		out.ntype=TYPE;\
		out.size=BYTESIZE;

#define STRING_SIZE(DATA)\
		submode=MSG_GEN_MODE_STRING_SIZE;\
		out.data=(const char *) DATA;

#define BUFFER_NEXT_BYTE\
		--buf.size;\
		++buf.ptr

#define BUFFER_RESET\
		buf.size=buffer_size;\
		buf.ptr=buffer

#define BYTE_TO_BUFFER(BYTE)\
	submode=MSG_GEN_MODE_BUFFER_FILL;\
	out.single_chr=BYTE;\
	out.correct=0;\
	out.data=&out.single_chr;\
	out.size=1;\
	out.curPos=0


#define NEXT_CONTENT ++data
#define NEXT_BUILDUP ++type

void send_rpc(uint8_t * const buffer, uint32_t buffer_size,
		const msg_gen_command_t *def)
{
	msg_gen_mode_t submode = MSG_GEN_MODE_TYPE;
	uint8_t def_state = 0;
	const msg_gen_type_t *type = def->header;
	const void** data = def->header_data;

	struct
	{
		uint32_t curPos;
		const char* data;
		union
		{

			struct
			{
				uint32_t size;
				uint8_t correct;
				char single_chr;
			};

			struct
			{
				uint64_t int_number;
				uint8_t digits;
			};
			msg_gen_state_numbertype_t ntype;
		};
	} out =
	{ 0, 0, 0 };

	struct
	{
		msg_gen_size_mode mode;
		bool payload_size_available;
		const msg_gen_type_t *type;
		const void **data;
		uint32_t payload_size;
		uint32_t rosrpc_size;
		uint32_t *selectedSize;

	} size =
	{ MSG_GEN_SIZE_MODE_NONE };

	struct
	{
		uint32_t size;
		uint8_t *ptr;
	} buf =
	{ buffer_size, buffer };

	while (1)
	{

		if (buf.size == 0 || (*type==MSG_TYPE_MESSAGE_END && !size.mode && submode == MSG_GEN_MODE_TYPE))
		{
			int byte;
			printf("\n##paket===============================================##\n");
			for (byte = 0; byte < buffer_size - buf.size; ++byte)
			{
				if ((buffer[byte] >= ' ' && buffer[byte] <= '~')
						|| buffer[byte] == '\n')
				{
					printf("%c", buffer[byte]);
				}
				else
				{
					printf("[%x]", (unsigned int) buffer[byte]);
				}
			}
			printf("\n##===================================================##\n");
			BUFFER_RESET;

			if(*type==MSG_TYPE_MESSAGE_END)
			{
				break;
			}
		}

		switch(submode)
		{
			case MSG_GEN_MODE_STRING_SIZE:
				DEBUG_PRINT_STR("Counting String: ")
				DEBUG_PRINT_STR(out.data);
				while (*out.data != 0)
				{
					++*size.selectedSize;
					++out.data;
				}
				submode = MSG_GEN_MODE_TYPE;
				break;

			case MSG_GEN_MODE_BUFFER_FILL:
			{

				int8_t const *correct;

				switch(out.correct)
				{
					case 2:
						correct = g_byte_order_correction_to_system->SIZE_2_B; //to_system because we catch it from a systems variable with the correction
						break;		//if I am wrong change to network :-)
					case 4:
						correct = g_byte_order_correction_to_system->SIZE_4_B;
						break;
					case 8:
						correct = g_byte_order_correction_to_system->SIZE_8_B;
						break;
					default:
						break;
				}

				while (buf.size > 0)
				{
					if (out.correct && out.size > 0) //==0 would be string terminated with 0
					{
						if (out.size > out.curPos )
						{
							*buf.ptr = *((char *) (out.data + out.curPos
									+ correct[out.curPos % out.correct]));
							BUFFER_NEXT_BYTE
							;
							out.curPos++;
						}
						else
						{
							submode = MSG_GEN_MODE_TYPE;
							break;
						}
					}
					else
					{
						if (out.curPos < out.size
								|| (out.size == 0
										&& (*((char *) out.data) != '\0')))
						{

							*buf.ptr = *((char *) out.data);
							BUFFER_NEXT_BYTE
							;

							++out.data;
							++out.curPos;
							;
						}
						else
						{
							submode = MSG_GEN_MODE_TYPE;
							break;
						}
					}

				}

			}
				break;

			case MSG_GEN_MODE_NUMBER_TO_STRING:
			{
				uint8_t bytesize, i;
				uint64_t conv;
				bool sign = false;

				if (out.curPos == 0) //Check if the current Position is zero (beginning)
				{

					if (*type > __MSG_TYPE_UINT_STRING)
					{
						bytesize = (1 << (*type - __MSG_TYPE_UINT_STRING - 1));
					}
					else if (*type > __MSG_TYPE_INT_STRING)
					{
						sign = true;
						bytesize = (1 << (*type - __MSG_TYPE_INT_STRING - 1));
					}
					else if(*type == MSG_TYPE_PAYLOAD_SIZE_STRING)
					{
						bytesize = 4;
					}

					switch(bytesize)
					//transfer the value to our 64 bit variable
					{
						case 1:
							out.int_number = *((uint8_t *) (out.data));
							break;
						case 2:
							out.int_number = *((uint16_t *) (out.data));
							break;
						case 4:
							out.int_number = *((uint32_t *) (out.data));
							break;
						case 8:
							out.int_number = *((uint64_t *) (out.data));
							break;
						default:
							break;
					}

					if (sign) //is it signed?
					{
						if (out.int_number & (1UL << ((bytesize * 8) - 1))) //is it negative?
						{

							if (size.mode != MSG_GEN_SIZE_MODE_NONE)
							{
								*size.selectedSize += 1; //add 1 to size for the minus
							}
							else
							{
								*buf.ptr = '-';
								BUFFER_NEXT_BYTE
								;
							}

							for (i = bytesize; i < 8; ++i)
							{
								out.int_number |= (0xFFUL << i * 8); //set the upper bytes to FF
							}
							*((int64_t *) &out.int_number) *= (-1); //interpred value as int64, multiply by -1 to get rid of the negative value
						}
					}

					out.digits = 0;
					conv=out.int_number;
					do //get the number of digits for the string output
					{
						++out.digits;
					} while (conv /=10);
					++out.curPos;
				}

				if (size.mode != MSG_GEN_SIZE_MODE_NONE) //If we are looking for the size
				{
					*size.selectedSize += out.digits; //Add the digits and leave this mode
					submode = MSG_GEN_MODE_TYPE;
					NEXT_BUILDUP;
				}
				else //Otherwise we want to convert and copy the digits to buffer
				{
					while (buf.size > 0)
					{
						uint8_t i;
						conv=1;

						for (i = 0; i < out.digits - out.curPos; ++i)
						{
							conv *= 10;
						}
						*buf.ptr = out.int_number/conv + 48;
						out.int_number-=out.int_number/conv * conv;
						++out.curPos;
						BUFFER_NEXT_BYTE
						;

						//note: +1 because the sign is the first
						if ((out.digits+1 - out.curPos) == 0) //if there are no more digits to convert leave
						{
							submode = MSG_GEN_MODE_TYPE;
							NEXT_BUILDUP;
							break;
						}
					}
				}
			}
				break;

			case MSG_GEN_MODE_TYPE:
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
						if (size.mode == MSG_GEN_SIZE_MODE_NONE) //Are we already finding the size?
							if (!size.payload_size_available) //Was the payload size already calculated?
							{ //No, then do it now...
								size.mode = true;
								size.data = data;
								size.type = type;
								size.selectedSize = &size.payload_size;
								type = def->payload;
								data = def->payload_data;
								size.mode = MSG_GEN_SIZE_MODE_PAYLOAD;
								size.payload_size_available = true;
							}
							else
							{
								if (*type == MSG_TYPE_PAYLOAD_SIZE_BINARY)
								{
									DATA_TO_BUFFER(&size.payload_size, 4, 4);
									NEXT_BUILDUP;
								}
								else
								{
									NUMBER_TO_BUFFER(&size.payload_size,
											MSG_GEN_NUMBERTYPE_UINT, 4);
								}
							}
						break;

					case MSG_TYPE_STRING:
						if (size.mode)
						{
							STRING_SIZE(*data);
						}
						else
						{
							STRING_TO_BUFFER(*data);
						}
						NEXT_CONTENT;
						NEXT_BUILDUP;
						break;

					case MSG_TYPE_DESCRIPTOR_HTTP_HEADER_END:
							BYTE_TO_BUFFER('\n');
							type = def->payload;
							data = def->payload_data;
						break;


					case MSG_TYPE_DESCRIPTOR_END:
							BYTE_TO_BUFFER('\n');
							NEXT_BUILDUP;
						break;

					case MSG_TYPE_ROSRPC_FIELD_END:
						if (size.mode == MSG_GEN_SIZE_MODE_ROSFIELD)
						{
							type = size.type;
							data = size.data;
							size.mode = MSG_GEN_SIZE_MODE_NONE;
						}
						else
						{
							NEXT_BUILDUP;
						}
						break;

					case MSG_TYPE_HEADER_END:
						//DEBUG - things never should happen in release...
						if (size.mode == MSG_GEN_SIZE_MODE_PAYLOAD
								|| size.mode == MSG_GEN_SIZE_MODE_ROSFIELD)
						{
							DEBUG_PRINT_STR("ERROR, message output generator, header end in PAYLOAD or ROSFIELD");
						}
						type = def->payload;
						data = def->payload_data;
						break;

					case MSG_TYPE_MESSAGE_END:
						if (size.mode == MSG_GEN_SIZE_MODE_PAYLOAD)
						{
							type = size.type;
							data = size.data;
							size.mode = MSG_GEN_SIZE_MODE_NONE;
						}
						else if (size.mode == MSG_GEN_SIZE_MODE_ROSFIELD)
						{
							//Only interesting in debug...
							DEBUG_PRINT_STR("ERROR: unexpected message end ... in ROSFIELD");
						}
						break;

					default:
						//Do not change the order here! Otherwise the order must be change in msg_gen_type_t as well!
						if(*type > MSG_TYPE_SKIP_ENTRIES)
						{
							uint8_t skip=*type - MSG_TYPE_SKIP_ENTRIES;
							while(skip>0)
							{
								++type;
								--skip;
							}
						}
						else if(*type > __MSG_TYPE_STRINGS)
						{
							const char *ptr;
							if(*type > __MSG_TYPE_HTTP_HEADER_STDTEXT)
							{
								ptr=http_header_stdtext[*type
										- __MSG_TYPE_HTTP_HEADER_STDTEXT
										- 1];
							}
							else if(*type > __MSG_TYPE_XMLRPC_STDTEXT)
							{
								ptr=xmlrpc_stdtext[*type
										- __MSG_TYPE_XMLRPC_STDTEXT
										- 1];
							}
							else if(*type > __MSG_TYPE_XMLRPC_SLAVE_METHODNAMES)
							{
								ptr=xmlrpc_slave_methodnames[*type
										- __MSG_TYPE_XMLRPC_SLAVE_METHODNAMES
										- 1];
							}
							else if(*type > __MSG_TYPE_HTTP_ACTIONS)
							{
								ptr=xmlrpc_http_actions[*type
										- __MSG_TYPE_HTTP_ACTIONS
										- 1];
							}




							if (size.mode)
							{
								STRING_SIZE(ptr);
							}
							else
							{
								STRING_TO_BUFFER(ptr);
							}
							NEXT_CONTENT;
							NEXT_BUILDUP;
						}
						else if (*type > __MSG_TYPE_ROSRPC_FIELD_STRINGS)
						{
							if (size.mode == MSG_GEN_SIZE_MODE_NONE)
							{
								switch(def_state)
								{
									case 0: //Determine field size
											def_state = 1;
											size.data = data;
											size.type = type;
											size.selectedSize = &size.rosrpc_size;
											size.rosrpc_size=0;
											size.mode =
													MSG_GEN_SIZE_MODE_ROSFIELD;
										break;
									case 1: //Write field size
										def_state = 2;
										DATA_TO_BUFFER(&size.rosrpc_size, 4, 4)
										;
										break;
									case 2: //id string
										def_state = 3;
										STRING_TO_BUFFER(
												ros_field_strings[*type
														- __MSG_TYPE_ROSRPC_FIELD_STRINGS
														- 1])
										;
										break;
									case 3: //equal
										def_state = 0;
										BYTE_TO_BUFFER('=')
										;
					 					NEXT_BUILDUP;
										break;
								}
							}
							else
							{
								if (size.mode == MSG_GEN_SIZE_MODE_PAYLOAD)
									*size.selectedSize += 4;

								++*size.selectedSize; // '='
								STRING_SIZE(
										ros_field_strings[*type
												- __MSG_TYPE_ROSRPC_FIELD_STRINGS
												- 1]);
								NEXT_BUILDUP;
							}
						}
						else if (*type > __MSG_TYPE_XMLRPC_CLOSE_TAGS
								|| *type > __MSG_TYPE_XMLRPC_OPEN_TAGS)
						{
							if (size.mode != MSG_GEN_SIZE_MODE_NONE)
							{
								if (*type > __MSG_TYPE_XMLRPC_CLOSE_TAGS)
								{ //'</>'
									*size.selectedSize += 3;
								}
								else //'<>'
								{
									*size.selectedSize += 2;
								}

								//Get tag string size
								if (*type > __MSG_TYPE_XMLRPC_CLOSE_TAGS)
								{
									STRING_SIZE(
											xmlrpc_tag_strings[*type
													- __MSG_TYPE_XMLRPC_CLOSE_TAGS
													- 1]);
								}
								else
								{
									STRING_SIZE(
											xmlrpc_tag_strings[*type
													- __MSG_TYPE_XMLRPC_OPEN_TAGS
													- 1]);
								}
								NEXT_BUILDUP;
							}
							else
								switch(def_state)
								{
									case 0: // '<'
										if (*type
												> __MSG_TYPE_XMLRPC_CLOSE_TAGS)
										{
											def_state = 1;
										}
										else
										{
											def_state = 2;
										}

										BYTE_TO_BUFFER('<')
										;
										break;
									case 1: //   '/'
										def_state = 2;
										BYTE_TO_BUFFER('/')
										;
										break;
									case 2: //id string
										def_state = 3;
										if (*type
												> __MSG_TYPE_XMLRPC_CLOSE_TAGS)
										{
											STRING_TO_BUFFER(
													xmlrpc_tag_strings[*type
															- __MSG_TYPE_XMLRPC_CLOSE_TAGS
															- 1]);
										}
										else
										{
											STRING_TO_BUFFER(
													xmlrpc_tag_strings[*type
															- __MSG_TYPE_XMLRPC_OPEN_TAGS
															- 1]);
										}
										break;
									case 3: // '>'
										def_state = 0;
										BYTE_TO_BUFFER('>')
										;
										NEXT_BUILDUP;
										break;
								}
						}

						else if (*type > __MSG_TYPE_DESCRIPTORS)
						{
								switch(def_state)
								{
									case 0: // DESCRIPTOR
										def_state = 1;
										STRING_TO_BUFFER(
												xmlrpc_http_descriptors[*type
														- __MSG_TYPE_DESCRIPTORS
														- 1])
										;
										break;

									case 1: // ':'
										def_state = 2;
										BYTE_TO_BUFFER(':')
										;
										break;

									case 2: // ' '
										def_state = 0;
										BYTE_TO_BUFFER(' ')
										;
										NEXT_BUILDUP;
										break;
								}
						}
						else if (*type > __MSG_TYPE_UINT_STRING
								|| *type > __MSG_TYPE_INT_STRING)
						{
							submode = MSG_GEN_MODE_NUMBER_TO_STRING;\
							out.curPos = 0;
							out.data = (const char *) *data;
							NEXT_CONTENT;
						}
						else if (*type > __MSG_TYPE_FLOAT_STRING)
						{

						}
						else if (*type > __MSG_TYPE_BINARY_OUT)
						{
							DATA_TO_BUFFER(*data,
									*type - __MSG_TYPE_BINARY_OUT - 1,
									*type - __MSG_TYPE_BINARY_OUT - 1);
							NEXT_BUILDUP;
						}
						else
						{
							DEBUG_PRINT(INT,"msg_gen_handler UNHANDLED STATE:", *type);
							while (1)
								;
						}
						break;

				}

				break;
		}

	}

}

