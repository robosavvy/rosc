
#include <rosc/com/publish.h>
#include <rosc/com/ros_handler.h>
#include <rosc/system/endian.h>


void publisherfill(iface_t *interface, void *msg, socket_t* cur)
{
	ros_iface_init_t* init=interface->init_data;
	if(interface->handler_function==&ros_handler)
	{
		if(init->ros_type!=ROS_HANDLER_TYPE_TOPIC_PUBLISHER)
		{
			ROSC_FATAL("  -- FATAL -- The given interface is NOT a PUBLISHER!");
		}
	}

	if(cur) //Was there a available publisher socket?
	{
		//Marshalling message data into first found ready publisher socket.
		const ros_buildup_type_t* buildup=init->buildup;
		char * buf=cur->pdata.additional_storage;
		const size_t* memory_offsets=init->memory_offsets+1;
		const size_t* array_lengths=init->array_lengths+1;
		const size_t* submessage_sizes = init->submessage_sizes+1;
		uint32_t submessage_depth=0;
		rosc_msg_submessage_state_t *submessage_state_array=cur->pdata.additional_storage+init->submessage_states_offset;
		void* current_message_start=msg;

		uint32_t overall_size=4; //begin with 4 - because of the overall message size, this will be subtracted later
		uint32_t message_depth=0;
		uint32_t buildup_no=0;
		uint32_t offset_no=0;


		void *current_value_address;
		uint8_t current_value_byte_size;
		uint32_t array_size=0;
		uint32_t amount=0;
		bool is_dynamic=false;


		while(*buildup!=ROS_MSG_BUILDUP_TYPE_MESSAGE_END || message_depth != 0)
		{
			switch(*buildup)
			{
			 case ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY:
			 case ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY_UL:
			 case ROS_MSG_BUILDUP_TYPE_SUBMESSAGE:
			 {
				 submessage_state_array[message_depth].parent_message_start=current_message_start;
				 submessage_state_array[message_depth].submessage_buildup_start=buildup_no;
				 submessage_state_array[message_depth].submessage_offset_start=offset_no;
				 switch(buildup[buildup_no])
				 {
				 	 case ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY:
				 	 case ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY_UL:
				 		offset_no++; //Go from struct to size

				 		submessage_state_array[message_depth].submessages_remaining=*((uint32_t*)current_message_start+memory_offsets[offset_no]);
				 		submessage_state_array[message_depth].submessage_byte_size=*submessage_sizes;

				 		if(buildup[buildup_no]==ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY_UL)
				 		{
				 			if(submessage_state_array[message_depth].submessages_remaining>*array_lengths)
				 			{
				 				submessage_state_array[message_depth].submessages_remaining=*array_lengths;
				 			}
				 			offset_no++; //Skip oversize field;
				 		}
				 		else
				 		{
				 			if(submessage_state_array[message_depth].submessages_remaining!=*array_lengths)
				 			{
				 				submessage_state_array[message_depth].submessages_remaining=*array_lengths;
				 			}
				 		}
				 		array_lengths++;
				 		submessage_sizes++;

				 		//Insert the array size into the out stream
				 		current_value_address=&(submessage_state_array[message_depth].submessages_remaining);
				 		current_value_byte_size=4;
				 		amount=1;

						submessage_state_array[message_depth].is_submessage_array=true;
						submessage_state_array[message_depth].is_dynamic_array=
								 (buildup[buildup_no]==ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY_UL);

				 		 break;
				 	 case ROS_MSG_BUILDUP_TYPE_SUBMESSAGE:
						 submessage_state_array[message_depth].is_submessage_array=false;
						 submessage_state_array[message_depth].is_dynamic_array=false;
						 submessage_state_array[message_depth].submessages_remaining=0;
						 amount=0;
				 		break;
				 }
				 message_depth++;
			 }
			 break;



			 case ROS_MSG_BUILDUP_TYPE_ARRAY:
			 case ROS_MSG_BUILDUP_TYPE_ARRAY_UL:
				 offset_no++; //Go from struct to size
				 	array_size=*((uint32_t*)memory_offsets[offset_no]);//TODO set

				 	if(buildup[buildup_no]==ROS_MSG_BUILDUP_TYPE_ARRAY_UL)
					{
				 		if(array_size>*array_lengths)
				 			array_size=*array_lengths;
						offset_no++; //Skip oversize;
					}
					else
					{
				 		if(array_size!=*array_lengths)
				 			array_size=*array_lengths;
					}
				 	array_lengths++;
				 break;





			 case ROS_MSG_BUILDUP_TYPE_STRING:





			 case ROS_MSG_BUILDUP_TYPE_CHAR:
			 case ROS_MSG_BUILDUP_TYPE_INT8:
			 case ROS_MSG_BUILDUP_TYPE_UINT8:
			 case ROS_MSG_BUILDUP_TYPE_BYTE:
			 case ROS_MSG_BUILDUP_TYPE_BOOL:
				 current_value_byte_size=1;
				 amount=1;
				 break;

			 case ROS_MSG_BUILDUP_TYPE_INT16:
			 case ROS_MSG_BUILDUP_TYPE_UINT16:
				 current_value_byte_size=2;
				 amount=1;
				 break;

			 case ROS_MSG_BUILDUP_TYPE_INT32:
			 case ROS_MSG_BUILDUP_TYPE_UINT32:
			 case ROS_MSG_BUILDUP_TYPE_FLOAT32:
				 current_value_byte_size=4;
				 amount=1;
				 break;

			 case ROS_MSG_BUILDUP_TYPE_INT64:
			 case ROS_MSG_BUILDUP_TYPE_UINT64:
			 case ROS_MSG_BUILDUP_TYPE_FLOAT64:
				 current_value_byte_size=8;
				 amount=1;
				 break;

			 case ROS_MSG_BUILDUP_TYPE_TIME:
			 case ROS_MSG_BUILDUP_TYPE_DURATION:
				 current_value_byte_size=4;
				 amount=2;
				 break;
			 case ROS_MSG_BUILDUP_TYPE_MESSAGE_END:


				 break;


			}

			//output array size to buffer
			if(array_size && !amount)
			{
				current_value_address=&array_size;
				amount=1;
				current_value_byte_size=4;
			}
			else if(amount)
			{
				if(array_size)
				{
					amount*=array_size;
					array_size=0;
				}
			}

			while(amount)
			{
				int i;
				const int8_t *correct_array;
				const char single=0;

				switch(current_value_byte_size)
				{
				case 1:
					correct_array=&single;
					break;
				case 2:
					correct_array=g_byte_order_correction_to_network->SIZE_2_B;
					break;

				case 4:
					correct_array=g_byte_order_correction_to_network->SIZE_4_B;
					break;

				case 8:
					correct_array=g_byte_order_correction_to_network->SIZE_8_B;
					break;
				}
				printf("%i \n",(int)*((int*)current_value_address));
				for(i=0;i<current_value_byte_size;i++)
				{
					printf("%x \n",(int)((char*)current_value_address)[i]);


					((char*)cur->pdata.additional_storage)[overall_size+correct_array[i]]=((char*)current_value_address)[i];
					overall_size++;


				}
				for(i=0;i<overall_size;i++)
					printf("%x",((char*)cur->pdata.additional_storage)[i]);
				printf("\n");
				amount--;
			}



			buildup_no++;
		}
}


void publish(iface_t *interface, void *msg)
{
	ros_iface_init_t* init=interface->init_data;
	if(interface->handler_function==&ros_handler)
	{
		if(init->ros_type!=ROS_HANDLER_TYPE_TOPIC_PUBLISHER)
		{
			ROSC_FATAL("  -- FATAL -- The given interface is NOT a PUBLISHER!");
		}
	}

	socket_t *cur=socket_list_start;
	socket_t *first=0;
	//Obtain first ready socket for the current interface
	while(cur != 0)
	{
		if(cur->iface == interface)
		{
			ros_handler_data_t *cur_handler_data=cur->pdata.handler_data;
			if(!cur_handler_data->publisher_ready)
			{
				first=cur;
				break;
			}
		}
		cur=cur->next;
	}





#if 0
		//Copy data to other subscribers
		while(cur)
		{
			ros_handler_data_t *cur_handler_data=cur->pdata.handler_data;
			if(cur->iface == interface && cur_handler_data->publisher_ready)
			{
				uint32_t s;
				cur->pdata.out_buf=cur->pdata.additional_storage;
				cur->pdata.out_len=overall_size;
				for(s=0;s<overall_size;++s)
				{
					cur->pdata.additional_storage[s]=first->pdata.additional_storage[s];
				}
				break;
			}
			cur=cur->next;
		}
#endif
	}
}

