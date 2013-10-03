#include <rosc/com/msg_gen.h>
#include <rosc/system/setup.h>
#include <rosc/system/status.h>
#include <rosc/system/eth.h>

#define DATA_TO_BUFFER(DATA, SIZE, CORRECTION)\
		def->submode=MSG_GEN_MODE_BUFFER_FILL;\
		def->out.data=(const char *) ((const void*)DATA);\
		def->out.curPos=0;\
		def->out.size=SIZE;\
		def->out.correct=CORRECTION;\

#define STRING_TO_BUFFER(DATA)\
		def->submode=MSG_GEN_MODE_BUFFER_FILL;\
		def->out.curPos=0;\
		def->out.size=0;\
		def->out.data=(const char *) DATA;\

#define NUMBER_TO_BUFFER(DATA, TYPE, BYTESIZE)\
		def->submode=MSG_GEN_MODE_NUMBER_TO_STRING;\
		def->out.curPos=0;\
		def->out.data=(const char *) DATA;\
		def->out.ntype=TYPE;\
		def->out.size=BYTESIZE;

#define STRING_SIZE(DATA)\
		def->submode=MSG_GEN_MODE_STRING_SIZE;\
		def->out.data=(const char *) DATA;

#define BUFFER_NEXT_BYTE\
		--*buffer_size;\
		++buffer


#define BYTE_TO_BUFFER(BYTE)\
	def->submode=MSG_GEN_MODE_BUFFER_FILL;\
	def->out.single_chr=BYTE;\
	def->out.correct=0;\
	def->out.data=&def->out.single_chr;\
	def->out.size=1;\
	def->out.curPos=0


#define NEXT_CONTENT ++def->data
#define NEXT_BUILDUP ++def->type




bool send_rpc(uint8_t * buffer, uint32_t *buffer_size,
		msg_gen_command_t *def)
{


	while (1)
	{

		if (*buffer_size == 0 || (*def->type==MSG_TYPE_MESSAGE_END && !def->size.mode && def->submode == MSG_GEN_MODE_TYPE))
		{
			if(*def->type==MSG_TYPE_MESSAGE_END)
			{
				return false;
			}
			else
			{
				return true;
			}
		}

		switch(def->submode)
		{
			case MSG_GEN_MODE_STRING_SIZE:
				DEBUG_PRINT_STR("Counting String: ")
				DEBUG_PRINT_STR(def->out.data);
				while (*def->out.data != 0)
				{
					++*def->size.selectedSize;
					++def->out.data;
				}
				def->submode = MSG_GEN_MODE_TYPE;
				break;

			case MSG_GEN_MODE_BUFFER_FILL:
			{

				int8_t const *correct;

				switch(def->out.correct)
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

				while (*buffer_size > 0)
				{
					if (def->out.correct && def->out.size > 0) //==0 would be string terminated with 0
					{
						if (def->out.size > def->out.curPos )
						{
							*buffer = *((char *) (def->out.data + def->out.curPos
									+ correct[def->out.curPos % def->out.correct]));
							BUFFER_NEXT_BYTE
							;
							def->out.curPos++;
						}
						else
						{
							def->submode = MSG_GEN_MODE_TYPE;
							break;
						}
					}
					else
					{
						if (def->out.curPos < def->out.size
								|| (def->out.size == 0
										&& (*((char *) def->out.data) != '\0')))
						{

							*buffer = *((char *) def->out.data);
							BUFFER_NEXT_BYTE
							;

							++def->out.data;
							++def->out.curPos;
							;
						}
						else
						{
							def->submode = MSG_GEN_MODE_TYPE;
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

				if (def->out.curPos == 0) //Check if the current Position is zero (beginning)
				{

					if (*def->type > __MSG_TYPE_UINT_STRING)
					{
						bytesize = (1 << (*def->type - __MSG_TYPE_UINT_STRING - 1));
					}
					else if (*def->type > __MSG_TYPE_INT_STRING)
					{
						sign = true;
						bytesize = (1 << (*def->type - __MSG_TYPE_INT_STRING - 1));
					}
					else if(*def->type == MSG_TYPE_PAYLOAD_SIZE_STRING)
					{
						bytesize = 4;
					}

					switch(bytesize)
					//transfer the value to our 64 bit variable
					{
						case 1:
							def->out.int_number = *((uint8_t *) (def->out.data));
							break;
						case 2:
							def->out.int_number = *((uint16_t *) (def->out.data));
							break;
						case 4:
							def->out.int_number = *((uint32_t *) (def->out.data));
							break;
						case 8:
							def->out.int_number = *((uint64_t *) (def->out.data));
							break;
						default:
							break;
					}

					if (sign) //is it signed?
					{
						if (def->out.int_number & (1UL << ((bytesize * 8) - 1))) //is it negative?
						{

							if (def->size.mode != MSG_GEN_SIZE_MODE_NONE)
							{
								*def->size.selectedSize += 1; //add 1 to size for the minus
							}
							else
							{
								*buffer = '-';
								BUFFER_NEXT_BYTE
								;
							}

							for (i = bytesize; i < 8; ++i)
							{
								def->out.int_number |= (0xFFUL << i * 8); //set the upper bytes to FF
							}
							*((int64_t *) &def->out.int_number) *= (-1); //interpred value as int64, multiply by -1 to get rid of the negative value
						}
					}

					def->out.digits = 0;
					conv=def->out.int_number;
					do //get the number of digits for the string output
					{
						++def->out.digits;
					} while (conv /=10);
					++def->out.curPos;
				}

				if (def->size.mode != MSG_GEN_SIZE_MODE_NONE) //If we are looking for the size
				{
					*def->size.selectedSize += def->out.digits; //Add the digits and leave this mode
					def->submode = MSG_GEN_MODE_TYPE;
					NEXT_BUILDUP;
				}
				else //Otherwise we want to convert and copy the digits to buffer
				{
					while (*buffer_size > 0)
					{
						uint8_t i;
						conv=1;

						for (i = 0; i < def->out.digits - def->out.curPos; ++i)
						{
							conv *= 10;
						}
						*buffer = def->out.int_number/conv + 48;
						def->out.int_number-=def->out.int_number/conv * conv;
						++def->out.curPos;
						BUFFER_NEXT_BYTE
						;

						//note: +1 because the sign is the first
						if ((def->out.digits+1 - def->out.curPos) == 0) //if there are no more digits to convert leave
						{
							def->submode = MSG_GEN_MODE_TYPE;
							NEXT_BUILDUP;
							break;
						}
					}
				}
			}
				break;

			case MSG_GEN_MODE_TYPE:
				switch(*def->type)
				{
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

					case MSG_TYPE_DOT:
					case MSG_TYPE_COLON:
					case MSG_TYPE_SLASH:
					case MSG_TYPE_SPACE:
						if(def->size.mode)
						{
							++*def->size.selectedSize;
						}
						else
						{
							char b;
							switch(*def->type)
							{
							case MSG_TYPE_DOT:
								b='.';
								break;

							case MSG_TYPE_COLON:
								b=':';
								break;

							case MSG_TYPE_SLASH:
								b='/';
								break;

							case MSG_TYPE_SPACE:
								b=' ';
								break;
							default:
								break;
							}
							BYTE_TO_BUFFER(b);
						}
						NEXT_BUILDUP;
						break;

					case MSG_TYPE_HOSTNAME_OR_IP:
						if (def->size.mode == MSG_GEN_SIZE_MODE_NONE)
						{
							STRING_TO_BUFFER(host_name);
						}
						else
						{
							STRING_SIZE(host_name);
						}
						NEXT_BUILDUP;
						break;

					case MSG_TYPE_NODENAME:
						if (def->size.mode == MSG_GEN_SIZE_MODE_NONE)
						{
							STRING_TO_BUFFER(node_name);
						}
						else
						{
							STRING_SIZE(node_name);
						}
						NEXT_BUILDUP;
						break;

					case MSG_TYPE_PAYLOAD_SIZE_STRING:
					case MSG_TYPE_PAYLOAD_SIZE_BINARY:
						if (def->size.mode == MSG_GEN_SIZE_MODE_NONE) //Are we already finding the size?
							if (!def->size.payload_size_available) //Was the payload size already calculated?
							{ //No, then do it now...
								def->size.mode = true;
								def->size.data = def->data;
								def->size.type = def->type;
								def->size.selectedSize = &def->size.payload_size;
								def->type = def->payload;
								def->data = def->payload_data;
								def->size.mode = MSG_GEN_SIZE_MODE_PAYLOAD;
								def->size.payload_size_available = true;
							}
							else
							{
								if (*def->type == MSG_TYPE_PAYLOAD_SIZE_BINARY)
								{
									DATA_TO_BUFFER(&def->size.payload_size, 4, 4);
									NEXT_BUILDUP;
								}
								else
								{
									NUMBER_TO_BUFFER(&def->size.payload_size,
											MSG_GEN_NUMBERTYPE_UINT, 4);
								}
							}
						break;

					case MSG_TYPE_STRING:
						if (def->size.mode)
						{
							STRING_SIZE(*def->data);
						}
						else
						{
							STRING_TO_BUFFER(*def->data);
						}
						NEXT_CONTENT;
						NEXT_BUILDUP;
						break;

					case MSG_TYPE_DESCRIPTOR_HTTP_HEADER_END:
							BYTE_TO_BUFFER('\n');
							def->type = def->payload;
							def->data = def->payload_data;
						break;


					case MSG_TYPE_DESCRIPTOR_END:
							BYTE_TO_BUFFER('\n');
							NEXT_BUILDUP;
						break;

					case MSG_TYPE_ROSRPC_FIELD_END:
						if (def->size.mode == MSG_GEN_SIZE_MODE_ROSFIELD)
						{
							def->type = def->size.type;
							def->data = def->size.data;
							def->size.mode = MSG_GEN_SIZE_MODE_NONE;
						}
						else
						{
							NEXT_BUILDUP;
						}
						break;

					case MSG_TYPE_SKIP_ENTRIES:
						while(*def->type != MSG_TYPE_SKIP_END)
						{
							++def->type;
						}
						++def->type;
						break;

					case MSG_TYPE_NONE:
					case MSG_TYPE_SKIP_END:
						++def->type;
						break;

					case MSG_TYPE_HEADER_END:
						//DEBUG - things never should happen in release...
						if (def->size.mode == MSG_GEN_SIZE_MODE_PAYLOAD
								|| def->size.mode == MSG_GEN_SIZE_MODE_ROSFIELD)
						{
							DEBUG_PRINT_STR("ERROR, message output generator, header end in PAYLOAD or ROSFIELD");
						}
						def->type = def->payload;
						def->data = def->payload_data;
						break;

					case MSG_TYPE_MESSAGE_END:
						if (def->size.mode == MSG_GEN_SIZE_MODE_PAYLOAD)
						{
							def->type = def->size.type;
							def->data = def->size.data;
							def->size.mode = MSG_GEN_SIZE_MODE_NONE;
						}
						else if (def->size.mode == MSG_GEN_SIZE_MODE_ROSFIELD)
						{
							//Only interesting in debug...
							DEBUG_PRINT_STR("ERROR: unexpected message end ... in ROSFIELD");
						}
						break;

					default:
						//Do not change the order here! Otherwise the order must be change in msg_gen_type_t as well!
						if(*def->type > __MSG_TYPE_STRINGS)
						{
							const char *ptr;
							if(*def->type > __MSG_TYPE_HTTP_HEADER_STDTEXT)
							{
								ptr=http_header_stdtext[*def->type
										- __MSG_TYPE_HTTP_HEADER_STDTEXT
										- 1];
							}
							else if(*def->type > __MSG_TYPE_XMLRPC_STDTEXT)
							{
								ptr=xmlrpc_stdtext[*def->type
										- __MSG_TYPE_XMLRPC_STDTEXT
										- 1];
							}
							else if(*def->type > __MSG_TYPE_XMLRPC_SLAVE_METHODNAMES)
							{
								ptr=xmlrpc_slave_methodnames[*def->type
										- __MSG_TYPE_XMLRPC_SLAVE_METHODNAMES
										- 1];
							}
							else if(*def->type > __MSG_TYPE_HTTP_ACTIONS)
							{
								ptr=xmlrpc_http_actions[*def->type
										- __MSG_TYPE_HTTP_ACTIONS
										- 1];
							}

							if (def->size.mode)
							{
								STRING_SIZE(ptr);
							}
							else
							{
								STRING_TO_BUFFER(ptr);
							}
							NEXT_BUILDUP;
						}
						else if (*def->type > __MSG_TYPE_ROSRPC_FIELD_STRINGS)
						{
							if (def->size.mode == MSG_GEN_SIZE_MODE_NONE)
							{
								switch(def->def_state)
								{
									case 0: //Determine field size
											def->def_state = 1;
											def->size.data = def->data;
											def->size.type = def->type;
											def->size.selectedSize = &def->size.rosrpc_size;
											def->size.rosrpc_size=0;
											def->size.mode =
													MSG_GEN_SIZE_MODE_ROSFIELD;
										break;
									case 1: //Write field size
										def->def_state = 2;
										DATA_TO_BUFFER(&def->size.rosrpc_size, 4, 4)
										;
										break;
									case 2: //id string
										def->def_state = 3;
										STRING_TO_BUFFER(
												ros_field_strings[*def->type
														- __MSG_TYPE_ROSRPC_FIELD_STRINGS
														- 1])
										;
										break;
									case 3: //equal
										def->def_state = 0;
										BYTE_TO_BUFFER('=')
										;
					 					NEXT_BUILDUP;
										break;
								}
							}
							else
							{
								if (def->size.mode == MSG_GEN_SIZE_MODE_PAYLOAD)
									*def->size.selectedSize += 4;

								++*def->size.selectedSize; // '='
								STRING_SIZE(
										ros_field_strings[*def->type
												- __MSG_TYPE_ROSRPC_FIELD_STRINGS
												- 1]);
								NEXT_BUILDUP;
							}
						}
						else if (*def->type > __MSG_TYPE_XMLRPC_CLOSE_TAGS
								|| *def->type > __MSG_TYPE_XMLRPC_OPEN_TAGS)
						{
							if (def->size.mode != MSG_GEN_SIZE_MODE_NONE)
							{
								if (*def->type > __MSG_TYPE_XMLRPC_CLOSE_TAGS)
								{ //'</>'
									*def->size.selectedSize += 3;
								}
								else //'<>'
								{
									*def->size.selectedSize += 2;
								}

								//Get tag string size
								if (*def->type > __MSG_TYPE_XMLRPC_CLOSE_TAGS)
								{
									STRING_SIZE(
											xmlrpc_tag_strings[*def->type
													- __MSG_TYPE_XMLRPC_CLOSE_TAGS
													- 1]);
								}
								else
								{
									STRING_SIZE(
											xmlrpc_tag_strings[*def->type
													- __MSG_TYPE_XMLRPC_OPEN_TAGS
													- 1]);
								}
								NEXT_BUILDUP;
							}
							else
								switch(def->def_state)
								{
									case 0: // '<'
										if (*def->type
												> __MSG_TYPE_XMLRPC_CLOSE_TAGS)
										{
											def->def_state = 1;
										}
										else
										{
											def->def_state = 2;
										}

										BYTE_TO_BUFFER('<')
										;
										break;
									case 1: //   '/'
										def->def_state = 2;
										BYTE_TO_BUFFER('/')
										;
										break;
									case 2: //id string
										def->def_state = 3;
										if (*def->type
												> __MSG_TYPE_XMLRPC_CLOSE_TAGS)
										{
											STRING_TO_BUFFER(
													xmlrpc_tag_strings[*def->type
															- __MSG_TYPE_XMLRPC_CLOSE_TAGS
															- 1]);
										}
										else
										{
											STRING_TO_BUFFER(
													xmlrpc_tag_strings[*def->type
															- __MSG_TYPE_XMLRPC_OPEN_TAGS
															- 1]);
										}
										break;
									case 3: // '>'
										def->def_state = 0;
										BYTE_TO_BUFFER('>')
										;
										NEXT_BUILDUP;
										break;
								}
						}

						else if (*def->type > __MSG_TYPE_DESCRIPTORS)
						{
								switch(def->def_state)
								{
									case 0: // DESCRIPTOR
										def->def_state = 1;
										STRING_TO_BUFFER(
												xmlrpc_http_descriptors[*def->type
														- __MSG_TYPE_DESCRIPTORS
														- 1])
										;
										break;

									case 1: // ':'
										def->def_state = 2;
										BYTE_TO_BUFFER(':')
										;
										break;

									case 2: // ' '
										def->def_state = 0;
										BYTE_TO_BUFFER(' ')
										;
										NEXT_BUILDUP;
										break;
								}
						}
						else if (*def->type > __MSG_TYPE_UINT_STRING
								|| *def->type > __MSG_TYPE_INT_STRING)
						{
							def->submode = MSG_GEN_MODE_NUMBER_TO_STRING;\
							def->out.curPos = 0;
							def->out.data = (const char *) *def->data;
							NEXT_CONTENT;
						}
						else if (*def->type > __MSG_TYPE_FLOAT_STRING)
						{

						}
						else if (*def->type > __MSG_TYPE_BINARY_OUT)
						{
							DATA_TO_BUFFER(*def->data,
									*def->type - __MSG_TYPE_BINARY_OUT - 1,
									*def->type - __MSG_TYPE_BINARY_OUT - 1);
							NEXT_BUILDUP;
						}
						else
						{
							DEBUG_PRINT(INT,"msg_gen_handler UNHANDLED STATE:", *def->type);
							while (1)
								;
						}
						break;

				}

				break;
		}

	}

}

