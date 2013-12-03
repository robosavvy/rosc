
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
		const size_t* memory_offsets=init->memory_offsets;
		const size_t* array_lengths=init->array_lengths+1;
		const size_t* submessage_sizes = init->submessage_sizes+1;
		uint32_t submessage_depth=0;
		rosc_msg_submessage_state_t *submessage_state_array=cur->pdata.additional_storage+init->submessage_states_offset;
		void* current_message_start=msg;

		uint32_t stream_pos=4; //begin with 4 - because of the overall message size, this will be subtracted later
		uint32_t message_depth=0;
		uint32_t buildup_no=0;
		uint32_t offset_no=1;

		uint32_t message_size;


		void *current_value_address;
		uint8_t current_value_byte_size;
		uint32_t array_size=0;
		uint32_t amount=0;
		bool is_dynamic=false;

		bool finish=false;

		while(!finish)
		{
			switch(buildup[buildup_no])
			{
			 case ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY:
			 case ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY_UL:
			 case ROS_MSG_BUILDUP_TYPE_SUBMESSAGE:
			 {
				 submessage_state_array[message_depth].parent_message_start=current_message_start;
				 submessage_state_array[message_depth].submessage_buildup_start=buildup_no;
				 switch(buildup[buildup_no])
				 {
				 	 case ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY:
				 	 case ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY_UL:

				 		submessage_state_array[message_depth].submessages_remaining=
				 				*(((char*)current_message_start)
				 						+memory_offsets[offset_no+0]//Struct
				 						+memory_offsets[offset_no+1]);//Size
				 		submessage_state_array[message_depth].submessage_byte_size=*submessage_sizes;

				 		//Dynamic array?
				 		if(buildup[buildup_no]==ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY_UL)
				 		{
				 			if(submessage_state_array[message_depth].submessages_remaining>*array_lengths)
				 			{
				 				submessage_state_array[message_depth].submessages_remaining=*array_lengths;
				 			}
				 			current_message_start=(((char*)current_message_start)
			 						+memory_offsets[offset_no+0]//Struct
			 						+memory_offsets[offset_no+3]);//Data
				 			offset_no+=4;

				 		}
				 		else
				 		{
				 			if(submessage_state_array[message_depth].submessages_remaining!=*array_lengths)
				 			{
				 				submessage_state_array[message_depth].submessages_remaining=*array_lengths;
				 			}
				 			current_message_start=(((char*)current_message_start)
			 						+memory_offsets[offset_no+0]//Struct
			 						+memory_offsets[offset_no+2]);//Data
				 			offset_no+=3;
				 		}
						submessage_state_array[message_depth].submessage_offset_start=offset_no;



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
				 current_value_address=((char *)current_message_start+memory_offsets[offset_no]);
				 offset_no++;
				 current_value_byte_size=1;
				 amount=1;
				 break;

			 case ROS_MSG_BUILDUP_TYPE_INT16:
			 case ROS_MSG_BUILDUP_TYPE_UINT16:
				 current_value_address=((char *)current_message_start+memory_offsets[offset_no]);
				 offset_no++;
				 current_value_byte_size=2;
				 amount=1;
				 break;

			 case ROS_MSG_BUILDUP_TYPE_INT32:
			 case ROS_MSG_BUILDUP_TYPE_UINT32:
			 case ROS_MSG_BUILDUP_TYPE_FLOAT32:
				 current_value_address=((char *)current_message_start+memory_offsets[offset_no]);
				 offset_no++;
				 current_value_byte_size=4;
				 amount=1;
				 break;

			 case ROS_MSG_BUILDUP_TYPE_INT64:
			 case ROS_MSG_BUILDUP_TYPE_UINT64:
			 case ROS_MSG_BUILDUP_TYPE_FLOAT64:
				 current_value_address=((char *)current_message_start+memory_offsets[offset_no]);
				 offset_no++;
				 current_value_byte_size=8;
				 amount=1;
				 break;

			 case ROS_MSG_BUILDUP_TYPE_TIME:
			 case ROS_MSG_BUILDUP_TYPE_DURATION:
				 current_value_address=((char *)current_message_start+memory_offsets[offset_no]);
				 offset_no+=2;
				 current_value_byte_size=4;
				 amount=2;
				 break;
			 case ROS_MSG_BUILDUP_TYPE_MESSAGE_END:
				 if(message_depth!=0)
				 {
					 if(submessage_state_array[message_depth-1].is_submessage_array)
					 {
						 submessage_state_array[message_depth-1].submessages_remaining--;
						 if(submessage_state_array[message_depth-1].submessages_remaining)
						 {
							 buildup_no=submessage_state_array[message_depth-1].submessage_buildup_start;
							 offset_no=submessage_state_array[message_depth-1].submessage_offset_start;
							 current_message_start=(char*)current_message_start+submessage_state_array[message_depth-1].submessage_byte_size;
						 }
						 else
						 {
							 current_message_start=submessage_state_array[message_depth-1].parent_message_start;
							 message_depth--;
						 }
					 }
				 }
				 else
				 {
					 finish=true;
				 }

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
			else if(finish)
			{
				message_size=stream_pos-4;
				stream_pos=0;
				current_value_address=&message_size;
				amount=1;
				current_value_byte_size=4;
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
					printf("%x \n",(int) *((char *)current_value_address));
					break;
				case 2:
					correct_array=g_byte_order_correction_to_network->SIZE_2_B;
					printf("%x \n",(int) *((uint16_t *)current_value_address));
					break;

				case 4:
					correct_array=g_byte_order_correction_to_network->SIZE_4_B;
					printf("%x \n",(int) *((uint32_t *)current_value_address));
					break;

				case 8:
					correct_array=g_byte_order_correction_to_network->SIZE_8_B;
					printf("%x \n",(int) *((uint64_t *)current_value_address));
					break;
				}

				for(i=0;i<current_value_byte_size;i++)
				{


					((unsigned char*)cur->pdata.additional_storage)[stream_pos+correct_array[i]]=((char*)current_value_address)[i];
					stream_pos++;


				}

				if(finish)
				{
					stream_pos+=message_size;
				}
				for(i=0;i<stream_pos;i++)
					printf(" %x ",((unsigned char*)cur->pdata.additional_storage)[i]);
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

