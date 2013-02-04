// /////////////////////////
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
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
// /////////////////////////

#include <rosc/rosc.h>
#include <rosc/system/eth.h>
#include <rosc/examplepack/exmsg.h>


const marshalling_cmd_t marshalling[]={MARSHALLING_CMD_BOOL,MARSHALLING_CMD_FLOAT32};

void callbackTest(int *a)
{
	printf("\n-%i-\n",*a);
}
ros_topic_t t={ .topic_str="/test", .type=PUBSUB_TYPE_PUBLISHER, .state=ROS_IFACE_OFF, .callback_fct=(ros_iface_callback)&callbackTest, .marshalling_cmds=marshalling };


STATIC_HOST_LIST_HEAD
STATIC_HOST_LIST_ENTRY_MASTER( IP_ADDR(192,168,0,1), "master", 11311)
STATIC_HOST_LIST_ENTRY_LOCALHOST(IP_ADDR(192,168,0,2), "local")
STATIC_HOST_LIST_ENTRY_MACHINE(IP_ADDR(192,168,0,3), "anotherMachine")
STATIC_HOST_LIST_FOOT;





FIXED_SIZE_EXAMPLEPACK_EXMSG_STRUCT( tsdfasest, 6, 5);

PUBLISHER_SUBSCRIBER_LIST_HEAD
//		{ .topic_str="/test", .type=PUBSUB_TYPE_PUBLISHER, .state=ROS_IFACE_OFF, .callback_fct=(ros_iface_callback)&callbackTest, .marshalling_cmds=marshalling }
		//PUBLISHER("/topic", FIXED_SIZE_TYPE),
PUBLISHER_SUBSCRIBER_LIST_FOOT


int main()
{


	printf("%i\n",__listenPort(5660));


	double test=100;
	void *t=&test;
	unsigned char *p=(unsigned char *)t;


	int var;
	for ( var = 0; var < 8; ++var)
	{
		int out=p[var];
		printf("%x ",out);
	}

	rosc_init();
	while(1)
	{

		rosc_spin();
	}


	return 0;
}
