#include <rosc/sebs_parse_fw/gen_modules/msg_gen_handler.h>

typedef enum
{
	MSG_GEN_MODE_TYPE, MSG_GEN_MODE_BUFFER_FILL, MSG_GEN_MODE_NUMBER_TO_STRING,
} msg_gen_mode_t;

typedef enum
{
	MSG_GEN_SIZE_TYPE_NONE,
	MSG_GEN_SIZE_TYPE_PAYLOAD,
	MSG_GEN_SIZE_TYPE_ROSFIELD,
}msg_gen_size_type;

typedef enum
{
	MSG_GEN_NUMBERTYPE_FLOAT, MSG_GEN_NUMBERTYPE_INT, MSG_GEN_NUMBERTYPE_UINT,
} msg_gen_state_numbertype_t;

#define GET_PAYLOAD_SIZE\
	size.active=true;\
	size.data=data;\
	size.type=type;\
	size.start=true

#define GET_ROSRPC_FIELD_SIZE\
	size.active=true;\
	size.data=data;\
	size.type=type;\
	size.start=true

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
		submode=MSG_GEN_MODE_BUFFER_FILL;\
		out.curPos=0;\
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

void send_rpc(uint8_t * const buffer, uint32_t buffer_size,
		const msg_gen_command_t *def)
{
	msg_gen_mode_t submode = MSG_GEN_MODE_TYPE;
	uint8_t def_state = 0;
	const msg_gen_type_t *type = def->header;
	const void** data = def->data;

	struct
	{
		const char* data;
		uint32_t size;
		uint32_t curPos;
		union
		{
			char single_chr;
			uint8_t correct;
			uint8_t digits;
			msg_gen_state_numbertype_t ntype;
		};
	} out;

	struct
	{
		bool start;
		bool active;
		bool payload_size_available;
		const msg_gen_type_t *type;
		const void **data;
		uint32_t payload_size;
		uint32_t rosrpc_size;
		uint32_t *s;
	} size =
	{ false };

	struct
	{
		uint32_t size;
		uint8_t *ptr;
	} buf =
	{ buffer_size, buffer };

	while (1)
	{

		switch(submode)
		{
			case MSG_GEN_MODE_BUFFER_FILL:
			{

				int8_t const *correct;
				if (out.size > 1)
					if (size.active) //If we only want the size
					{
						size.s += out.size; //add it ...
						submode = MSG_GEN_MODE_TYPE; //and exit
						break;
					}

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
						//ROSC_FATAL("BUFFER_FILL: NO CORRECTION FOR THAT BYTE SIZE!!!!");
						break;
				}

				while (buf.size > 0)
				{
					if (correct && out.size > 0) //==0 would be string terminated with 0
					{
						if (out.size > 0)
						{
							*buf.ptr = *((char *) (out.data
									+ (out.curPos / out.correct)
									+ correct[out.curPos % out.correct]));
							BUFFER_NEXT_BYTE
							;
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
							if (size.active)
							{
								++*size.s;
							}
							else
							{
								*buf.ptr = *((char *) out.data);
								BUFFER_NEXT_BYTE
								;
							}

							++out.data;
							++out.curPos;
							;
						}
						else
						{
							submode = MSG_GEN_MODE_TYPE;
							NEXT_CONTENT;
							break;
						}
					}

				}

			}
				break;

			case MSG_GEN_MODE_NUMBER_TO_STRING:
			{
				uint8_t bytesize, i;
				bool sign = false;
				uint64_t var_uint;

				if (out.curPos == 0) //Check if the current Position is zero (beginning)
				{

					if (*size.type > __MSG_TYPE_UINT_STRING)
					{
						bytesize = (1
								<< (*size.type - __MSG_TYPE_UINT_STRING - 1));
					}
					else
					{
						sign = true;
						bytesize = (1
								<< (*size.type - __MSG_TYPE_INT_STRING - 1));
					}

					switch(bytesize)
					//transfer the value to our 64 bit variable
					{
						case 1:
							var_uint = *((uint8_t *) (*data));
							break;
						case 2:
							var_uint = *((uint16_t *) (*data));
							break;
						case 4:
							var_uint = *((uint32_t *) (*data));
							break;
						case 8:
							var_uint = *((uint64_t *) (*data));
							break;
						default:
							break;
					}

					if (sign) //is it signed?
					{
						if (var_uint & (1UL << ((bytesize * 8) - 1))) //is it negative?
						{

							if (size.active)
							{
								*size.s += 1; //add 1 to size for the minus
							}
							else
							{
								*buf.ptr = '-';
								BUFFER_NEXT_BYTE
								;
							}

							for (i = bytesize; i < 8; ++i)
							{
								var_uint |= (0xFFUL << i * 8); //set the upper bytes to FF
							}
							*((int64_t *) &var_uint) *= (-1); //interpred value as int64, multiply by -1 to get rid of the negative value
						}
					}

					out.digits = 0;
					do //get the number of digits for the string output
					{
						++out.digits;
					} while (var_uint /= 10);
					++out.curPos;
				}

				if (size.active) //If we are looking for the size
				{
					*size.s += out.digits; //Add the digits and leave this mode
					submode = MSG_GEN_MODE_TYPE;
					NEXT_CONTENT;
				}
				else //Otherwise we want to convert and copy the digits to buffer
				{
					while (buf.size > 0)
					{
						uint8_t i;
						uint64_t conv = var_uint;

						for (i = 0; i < out.digits - out.curPos; ++i)
						{
							conv /= 10;
						}
						*buf.ptr = conv - 48;
						++out.curPos;
						BUFFER_NEXT_BYTE
						;

						if ((out.digits - out.curPos) == 0) //if there are no more digits to convert leave
						{
							submode = MSG_GEN_MODE_TYPE;
							NEXT_CONTENT;
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
						if (!size.payload_size_available) //Was the payload size already calculated?
						{ //No, then do it now...
							GET_PAYLOAD_SIZE
							;
						}
						else
						{
							if (*type == MSG_TYPE_PAYLOAD_SIZE_BINARY)
							{
								DATA_TO_BUFFER(&size.payload_size, 4, 4);
							}
							else
							{
								NUMBER_TO_BUFFER(&size.payload_size,
										MSG_GEN_NUMBERTYPE_UINT, 4);
							}
						}
						break;

					case MSG_TYPE_STRING:
						STRING_TO_BUFFER(data)
						;
						NEXT_BUILDUP;
						NEXT_CONTENT;
						break;

					case MSG_TYPE_DESCRIPTOR_END:
						BYTE_TO_BUFFER('\n')
						;
						NEXT_BUILDUP;
						break;

					case MSG_TYPE_DESCRIPTOR_HTTP_HEADER_END:
						BYTE_TO_BUFFER('\n')
						;
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
						if (*type > __MSG_TYPE_ROSRPC_FIELD_STRINGS)
						{
							switch(def_state)
							{
								case 0: //Determine field size
									def_state = 1;
									GET_ROSRPC_FIELD_SIZE
									;
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
						else if (*type > __MSG_TYPE_XMLRPC_CLOSE_TAGS
								|| *type > __MSG_TYPE_XMLRPC_OPEN_TAGS)
						{
							switch(def_state)
							{
								case 0: // '<'
									if (*type > __MSG_TYPE_XMLRPC_CLOSE_TAGS)
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
									if (*type > __MSG_TYPE_XMLRPC_CLOSE_TAGS)
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
													- __MSG_TYPE_DESCRIPTORS - 1])
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
						else if (*type > __MSG_TYPE_UINT_STRING)
						{
							NUMBER_TO_BUFFER(*data, MSG_GEN_NUMBERTYPE_UINT,
									*type - __MSG_TYPE_UINT_STRING - 1);
						}
						else if (*type > __MSG_TYPE_INT_STRING)
						{
							NUMBER_TO_BUFFER(*data, MSG_GEN_NUMBERTYPE_INT,
									*type - __MSG_TYPE_INT_STRING - 1);
						}
						else if (*type > __MSG_TYPE_FLOAT_STRING)
						{
							NUMBER_TO_BUFFER(*data, MSG_GEN_NUMBERTYPE_FLOAT,
									*type - __MSG_TYPE_FLOAT_STRING - 1);
						}
						else if (*type > __MSG_TYPE_BINARY_OUT)
						{
							DATA_TO_BUFFER(*data,
									*type - __MSG_TYPE_BINARY_OUT - 1,
									*type - __MSG_TYPE_BINARY_OUT - 1);
						}
						else
						{
							DEBUG_PRINT(INT,"msg_gen_handler UNHANDLED STATE:", *type);
						}
						break;

				}
				break;
			}

		}
	}
}
