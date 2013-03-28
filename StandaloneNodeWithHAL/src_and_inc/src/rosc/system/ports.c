/*
 * ports.c
 *
 *  Created on: 04.02.2013
 *      Author: cyborg-x1
 */

#include <rosc/system/ports.h>
#include <string.h>
#include <stdio.h>//TODO Remove

port_info ports_info[__PORT_MAXIMUM__];

port_id_t listenPort(port_t port, port_user_type_t port_user_type, port_uin_t port_user_number)
{
	int id=__listenPort(port);

	if(id>0)
	{
		if(id<__PORT_MAXIMUM__)
		{
			ports_info[id].port_user_type = port_user_type;
			ports_info[id].user_number = port_user_number;
		}
		else
		{
			id= -1000;
		}
	}
	return id;
}

void closePort(port_id_t port_id)
{
	if(port_id>=0 && port_id<__PORT_MAXIMUM__)
	{
		ports_info[port_id].port_user_type = PORT_USER_TYPE_UNUSED;
	}
	else
	{
		DEBUG_PRINT(INT,"Error! Can not close port id",  port_id);
	}
}

inline void ports_info_init()
{
	int i=0;
	for (i = 0; i < __PORT_MAXIMUM__; ++i)
	{
		memset(ports_info,0,sizeof(port_info)*__PORT_MAXIMUM__);
	}
}
