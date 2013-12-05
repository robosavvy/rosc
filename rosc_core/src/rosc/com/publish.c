#include <rosc/com/ros_handler.h>
#include <rosc/system/endian.h>
#include <string.h>

uint32_t publisherfill(iface_t *interface, void const *msg, socket_t* cur)
{
	ros_iface_init_t* init = interface->init_data;
	if (interface->handler_function == &ros_handler)
	{
		if (init->ros_type != ROS_HANDLER_TYPE_TOPIC_PUBLISHER)
		{
			ROSC_FATAL("  -- FATAL -- The given interface is NOT a PUBLISHER!");
		}
	}


		//Marshalling message data into first found ready publisher socket.
		const ros_buildup_type_t* buildup = init->buildup;
		char * buf = cur->pdata.additional_storage;
		const size_t* memory_offsets = init->memory_offsets;
		const size_t* array_lengths = init->array_lengths + 1;
		const size_t* submessage_sizes = init->submessage_sizes + 1;
		uint32_t submessage_depth = 0;
		rosc_msg_submessage_state_t *submessage_state_array =
				cur->pdata.additional_storage + init->submessage_states_offset;
		void *current_message_start = (void *)msg;

		uint32_t stream_pos = 4; //begin with 4 - because of the overall message size, this will be subtracted later
		uint32_t message_depth = 0;
		uint32_t buildup_no = 0;
		uint32_t offset_no = 1;

		uint32_t message_size;

		uint32_t null = 0;

		void *current_value_address;
		uint8_t current_value_byte_size;
		uint32_t array_size = 0;
		uint32_t amount = 0;

		uint32_t array_length_no = 0;
		uint32_t submessage_size_no = 0;

		bool array_is_dynamic = false;

		bool finish = false;
		bool isString = false;
		uint32_t string_size = 0;

		while (!finish)
		{
			switch (buildup[buildup_no])
			{
			case ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY:
			case ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY_UL:
			case ROS_MSG_BUILDUP_TYPE_SUBMESSAGE:
			{
				submessage_state_array[message_depth].parent_message_start =
						current_message_start;
				submessage_state_array[message_depth].submessage_buildup_start =
						buildup_no;
				submessage_state_array[message_depth].submessage_array_lengths_start =
						array_length_no;
				submessage_state_array[message_depth].submessage_submessage_size_start =
						submessage_size_no;

				switch (buildup[buildup_no])
				{
				case ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY:
				case ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY_UL:

					submessage_state_array[message_depth].submessages_remaining =
							*(((char*) current_message_start)
									+ memory_offsets[offset_no + 0] //Struct
									+ memory_offsets[offset_no + 1]); //Size
					submessage_state_array[message_depth].submessage_byte_size =
							submessage_sizes[submessage_size_no];

					//Dynamic array?
					if (buildup[buildup_no]
							== ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY_UL)
					{
						if (submessage_state_array[message_depth].submessages_remaining
								> array_lengths[array_length_no])
						{
							submessage_state_array[message_depth].submessages_remaining =
									array_lengths[array_length_no];
						}
						current_message_start = (((char*) current_message_start)
								+ memory_offsets[offset_no + 0] //Struct
								+ memory_offsets[offset_no + 3]); //Data
						offset_no += 4;

					}
					else
					{
						if (submessage_state_array[message_depth].submessages_remaining
								!= array_lengths[array_length_no])
						{
							submessage_state_array[message_depth].submessages_remaining =
									array_lengths[array_length_no];
						}
						current_message_start = (((char*) current_message_start)
								+ memory_offsets[offset_no + 0] //Struct
								+ memory_offsets[offset_no + 2]); //Data
						offset_no += 3;
					}
					submessage_state_array[message_depth].submessage_offset_start =
							offset_no;

					array_length_no++;
					submessage_size_no++;

					//Insert the array size into the out stream
					current_value_address =
							&(submessage_state_array[message_depth].submessages_remaining);
					current_value_byte_size = 4;
					amount = 1;

					submessage_state_array[message_depth].is_submessage_array =
							true;
					submessage_state_array[message_depth].is_dynamic_array =
							(buildup[buildup_no]
									== ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY_UL);

					break;
				case ROS_MSG_BUILDUP_TYPE_SUBMESSAGE:
					submessage_state_array[message_depth].is_submessage_array =
							false;
					submessage_state_array[message_depth].is_dynamic_array =
							false;
					submessage_state_array[message_depth].submessages_remaining =
							0;
					amount = 0;
					break;
				}
				message_depth++;
			}
				break;

			case ROS_MSG_BUILDUP_TYPE_ARRAY:
			case ROS_MSG_BUILDUP_TYPE_ARRAY_UL:
				array_size = *((uint32_t*) ((char*) current_message_start
						+ memory_offsets[offset_no] + //struct
						memory_offsets[offset_no + 1])); //size
				amount = 0;
				if (buildup[buildup_no] == ROS_MSG_BUILDUP_TYPE_ARRAY_UL)
				{
					if (array_size > array_lengths[array_length_no])
						array_size = array_lengths[array_length_no];
					array_is_dynamic = true;
				}
				else
				{
					if (array_size != array_lengths[array_length_no])
						array_size = array_lengths[array_length_no];
					array_is_dynamic = false;
				}
				array_length_no++;

				break;

			case ROS_MSG_BUILDUP_TYPE_STRING:
				isString = true;
				current_value_address = ((char *) current_message_start
						+ memory_offsets[offset_no]);
				current_value_byte_size = 4;
				amount = 1;
				break;

			case ROS_MSG_BUILDUP_TYPE_CHAR:
			case ROS_MSG_BUILDUP_TYPE_INT8:
			case ROS_MSG_BUILDUP_TYPE_UINT8:
			case ROS_MSG_BUILDUP_TYPE_BYTE:
			case ROS_MSG_BUILDUP_TYPE_BOOL:
				current_value_address = ((char *) current_message_start
						+ memory_offsets[offset_no]);
				offset_no++;
				current_value_byte_size = 1;
				amount = 1;
				break;

			case ROS_MSG_BUILDUP_TYPE_INT16:
			case ROS_MSG_BUILDUP_TYPE_UINT16:
				current_value_address = ((char *) current_message_start
						+ memory_offsets[offset_no]);
				offset_no++;
				current_value_byte_size = 2;
				amount = 1;
				break;

			case ROS_MSG_BUILDUP_TYPE_INT32:
			case ROS_MSG_BUILDUP_TYPE_UINT32:
			case ROS_MSG_BUILDUP_TYPE_FLOAT32:
				current_value_address = ((char *) current_message_start
						+ memory_offsets[offset_no]);
				offset_no++;
				current_value_byte_size = 4;
				amount = 1;
				break;

			case ROS_MSG_BUILDUP_TYPE_INT64:
			case ROS_MSG_BUILDUP_TYPE_UINT64:
			case ROS_MSG_BUILDUP_TYPE_FLOAT64:
				current_value_address = ((char *) current_message_start
						+ memory_offsets[offset_no]);
				offset_no++;
				current_value_byte_size = 8;
				amount = 1;
				break;

			case ROS_MSG_BUILDUP_TYPE_TIME:
			case ROS_MSG_BUILDUP_TYPE_DURATION:
				current_value_address = ((char *) current_message_start
						+ memory_offsets[offset_no]);
				offset_no += 2;
				current_value_byte_size = 4;
				amount = 2;
				break;
			case ROS_MSG_BUILDUP_TYPE_MESSAGE_END:
				if (message_depth != 0)
				{
					if (submessage_state_array[message_depth - 1].is_submessage_array)
					{
						submessage_state_array[message_depth - 1].submessages_remaining--;
						if (submessage_state_array[message_depth - 1].submessages_remaining)
						{
							buildup_no = submessage_state_array[message_depth
									- 1].submessage_buildup_start;
							offset_no =
									submessage_state_array[message_depth - 1].submessage_offset_start;
							array_length_no =
									submessage_state_array[message_depth].submessage_array_lengths_start;
							submessage_size_no =
									submessage_state_array[message_depth].submessage_submessage_size_start;
							current_message_start =
									(char*) current_message_start
											+ submessage_state_array[message_depth
													- 1].submessage_byte_size;
						}
						else
						{
							current_message_start =
									submessage_state_array[message_depth - 1].parent_message_start;
							message_depth--;
						}
					}
				}
				else
				{
					finish = true;
				}

				break;

			}

			//output array size to buffer
			if (array_size && !amount)
			{
				current_value_address = &array_size;
				amount = 1;
				current_value_byte_size = 4;
			}
			else if (amount)
			{
				if (array_size) //Do we have an array?
				{
					amount *= array_size;
					array_size = 0; //Set to no array
					if (array_is_dynamic)
					{
						offset_no += 3; //with oversize field
						current_value_address += memory_offsets[offset_no];
					}
					else
					{
						offset_no += 2; //without oversize field
						current_value_address += memory_offsets[offset_no];
					}
				}
			}
			else if (finish)
			{
				message_size = stream_pos - 4;
				stream_pos = 0;
				current_value_address = &message_size;
				amount = 1;
				current_value_byte_size = 4;
			}

			const int8_t *correct_array;
			const char single = 0;
			switch (current_value_byte_size)
			{
			case 1:
				correct_array = &single;
				printf("%x \n", (int) *((char *) current_value_address));
				break;
			case 2:
				correct_array = g_byte_order_correction_to_network->SIZE_2_B;
				printf("%x \n", (int) *((uint16_t *) current_value_address));
				break;

			case 4:
				correct_array = g_byte_order_correction_to_network->SIZE_4_B;
				printf("%x \n", (int) *((uint32_t *) current_value_address));
				break;

			case 8:
				correct_array = g_byte_order_correction_to_network->SIZE_8_B;
				printf("%x \n", (int) *((uint64_t *) current_value_address));
				break;
			}

			if (isString) //A string
			{
				int i;
				for (i = 0; i < amount; i++)
				{
					int b;
					//increase offset to string size

					uint32_t usergiven_size =
							*((uint32_t*) ((char *) current_value_address
									+ memory_offsets[offset_no + 1]));
					uint32_t string_size;

					//Check if the user has entered a to big size ...
					if (usergiven_size > array_lengths[array_length_no])
						usergiven_size = array_lengths[array_length_no];

					//save the start of the string
					char *string = ((char *) current_value_address
							+ memory_offsets[offset_no + 3]);

					//copying the string into the output buffer, determining real size if the terminator
					//comes before the users given size the string will be cut there
					for (string_size = 0;
							string_size < usergiven_size
									&& string[string_size] != '\0';
							++string_size)
					{
						((char*) cur->pdata.additional_storage)[stream_pos + 4 /* skipping the size for setting it later*/
								+ string_size] = string[string_size];
					}

					//Insert the determined size
					correct_array =
							g_byte_order_correction_to_network->SIZE_4_B;
					for (b = 0; b < 4; b++)
					{
						((char*) cur->pdata.additional_storage)[stream_pos
								+ correct_array[b]] = ((char*) &string_size)[b];
						stream_pos++;
					}

					//Update the current stream position
					stream_pos += string_size;

					//If this is not the last string to insert increase current value address by the string struct size
					if (i + 1 < amount)
						current_value_address = ((char*) current_value_address)
								+ submessage_sizes[submessage_size_no];

				}
				if (amount > 1)
				{
					array_length_no++;
					offset_no += 4;
					submessage_size_no++;
					amount = 0;
				}
			}
			else
				while (amount)
				{
					int i;

					for (i = 0; i < current_value_byte_size; i++)
					{
						((unsigned char*) cur->pdata.additional_storage)[stream_pos
								+ correct_array[i]] =
								((char*) current_value_address)[i];
						stream_pos++;
					}

					if (finish)
					{
						printf("finish!\n");
						return (stream_pos + message_size);
					}

					amount--;
				}

			if (isString)
				isString = false;

			buildup_no++;
		}

}

void publish(iface_t *interface, void const *msg)
{

	uint32_t overall_size;
	ros_iface_init_t* init = interface->init_data;
	ros_handler_data_t *cur_handler_data;

	DEBUG_PRINT_STR("PUBLISH!");
	if (interface->handler_function == &ros_handler)
	{
		if (init->ros_type != ROS_HANDLER_TYPE_TOPIC_PUBLISHER)
		{
			ROSC_FATAL("  -- FATAL -- The given interface is NOT a PUBLISHER!");
		}
	}

	socket_t *cur = socket_list_start;
	socket_t *first = 0;
	//Obtain first ready socket for the current interface
	while (cur != 0)
	{
		if (cur->iface == interface)
		{
			cur_handler_data = cur->pdata.handler_data;
			if (cur_handler_data->publisher_ready)
			{
				first = cur;
				break;
			}
		}
		cur = cur->next;
	}


	//TODO integrate function here
	if (cur) //Was there a available publisher socket?
	{
		overall_size = publisherfill(interface, msg, cur);
		cur->pdata.out_len=overall_size;
		cur_handler_data->publisher_ready=false;
		cur = cur->next;
		while (cur)
		{
			cur_handler_data = cur->pdata.handler_data;
			if (cur->iface == interface && cur_handler_data->publisher_ready)
			{
				uint32_t s;
				cur->pdata.out_buf = cur->pdata.additional_storage;
				cur_handler_data->publisher_ready=false;
				cur->pdata.out_len = overall_size;
				for (s = 0; s < overall_size; ++s)
				{
					((char*) cur->pdata.additional_storage)[s] =
							((char*) first->pdata.additional_storage)[s];
				}
				break;
			}
			cur = cur->next;
		}
	}




}

