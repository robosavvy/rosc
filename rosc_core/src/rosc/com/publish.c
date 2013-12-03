
#include <rosc/com/publish.h>

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


	if(cur) //Was there a available publisher socket?
	{


		//Marshalling message data into first found ready publisher socket.
		ros_buildup_type_t* buildup=init->buildup;
		size_t* const memory_offsets=init->memory_offsets;
		uint32_t size=0;
		uint32_t message_depth=0;

		while(*buildup!=ROS_MSG_BUILDUP_TYPE_MESSAGE_END || message_depth != 0)
		{
			switch(*buildup)
			{
			 case ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY:
			 case ROS_MSG_BUILDUP_TYPE_SUBMESSAGEARRAY_UL:
			 case ROS_MSG_BUILDUP_TYPE_SUBMESSAGE:
			 case ROS_MSG_BUILDUP_TYPE_ARRAY:
			 case ROS_MSG_BUILDUP_TYPE_ARRAY_UL:

			 case ROS_MSG_BUILDUP_TYPE_STRING:

			 case ROS_MSG_BUILDUP_TYPE_CHAR:
			 case ROS_MSG_BUILDUP_TYPE_INT8:
			 case ROS_MSG_BUILDUP_TYPE_UINT8:
			 case ROS_MSG_BUILDUP_TYPE_BYTE:
			 case ROS_MSG_BUILDUP_TYPE_BOOL:

			 case ROS_MSG_BUILDUP_TYPE_INT16:
			 case ROS_MSG_BUILDUP_TYPE_UINT16:

			 case ROS_MSG_BUILDUP_TYPE_INT32:
			 case ROS_MSG_BUILDUP_TYPE_UINT32:
			 case ROS_MSG_BUILDUP_TYPE_FLOAT32:

			 case ROS_MSG_BUILDUP_TYPE_INT64:
			 case ROS_MSG_BUILDUP_TYPE_UINT64:
			 case ROS_MSG_BUILDUP_TYPE_FLOAT64:

			 case ROS_MSG_BUILDUP_TYPE_TIME:
			 case ROS_MSG_BUILDUP_TYPE_DURATION:

			 case ROS_MSG_BUILDUP_TYPE_MESSAGE_END:
			}
			buildup++;
		}

		//Set output data for socket
		if(!size)
			ROSC_FATAL("--- FATAL --- PUBLISHED OUTPUT DATA WOULD BE ZERO LENGTH --- this is not possible! ---")
		cur->pdata.out_len=size;
		//Copy data to other subscribers
		while(cur)
		{
			ros_handler_data_t *cur_handler_data=cur->pdata.handler_data;
			if(cur->iface == interface && cur_handler_data->publisher_ready)
			{
				uint32_t s;
				cur->pdata.out_buf=cur->pdata.additional_storage;
				cur->pdata.out_len=size;
				for(s=0;s<size;++s)
				{
					cur->pdata.additional_storage[s]=first->pdata.additional_storage[s];
				}
				break;
			}
			cur=cur->next;
		}
	}
}

