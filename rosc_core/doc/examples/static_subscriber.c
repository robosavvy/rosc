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
 *  main.c created by Christian Holl
 */


/* Include rosc.h */
#include <rosc/rosc.h>

/* Include the header files from the messages you want to use */
#include <rosc/msg/rosc_linux_test/simple1.h>
#include <rosc/msg/rosc_linux_test/simple2.h>

/*
 * Import their buildup information with these macros
 * The name is like the following:
 * ROSC_STATIC_MSG_BUILDUP__PACKAGENAME__MESSAGETYPE
 */
ROSC_STATIC_MSG_BUILDUP__rosc_linux_test__simple1();
ROSC_STATIC_MSG_BUILDUP__rosc_linux_test__simple2();

/*
 * Create your special userdefined static topic.
 * First of all this step is required for all topic types!
 * Second, if the topic type has dynamic arrays,
 * after the user type of your topic there will be the list of
 * the sizes the included arrays will have at a maximum!
 * If you receive more than this value for an array,
 * those entries will be skipped!
 * ROSC_STATIC_MSG_USER_DEF__PACKAGENAME__MESSAGETYPE(USER_TYPE, ... DYN. ARRAY SIZES ...);
 */
ROSC_STATIC_MSG_USER_DEF__rosc_linux_test__simple1(sim1, 4);
ROSC_STATIC_MSG_USER_DEF__rosc_linux_test__simple2(sim2);

/*
 * This is the message list, it generates the reserved memory for each
 * socket.
 */
ROSC_STATIC_SYSTEM_MESSAGE_TYPE_LIST_BEGIN
	/*
	 * This is an entry of the message type list.
	 * ROSC_SIZE_LIST_ENTRY__PACKAGENAME_MESSAGETYPE(USER_TYPE)
	 */
	ROSC_SIZE_LIST_ENTRY__rosc_linux_test__simple1(sim1);
	ROSC_SIZE_LIST_ENTRY__rosc_linux_test__simple2(sim2);

	/* The memory defined here is also used as output buffer, if you want to have a
	 * bigger memory to process incoming data faster, you can increase it with this
	 * macro. This is optional.
	 * ROSC_SIZE_LIST_ENTRY_MIN_XMLRPC_OUTPUT_BUFFER(MINIMUM_BYTE_SIZE);
	 */
	ROSC_SIZE_LIST_ENTRY_MIN_XMLRPC_OUTPUT_BUFFER(100);
ROSC_STATIC_SYSTEM_MESSAGE_TYPE_LIST_END

/*
 * This is the callback function for your first topic
 * it generates the header for your callback function for you.
 * ROSC_STATIC_CALLBACK_HEAD__PACKAGENAME__MESSAGETYPE(USER_TYPE, TOPIC_REFERENCE)
 */
ROSC_STATIC_CALLBACK_HEAD__rosc_linux_test__simple1(sim1,simpleTopic1)
	printf("simple1 callback: %i\n", msg->EightArray.data[0]);
}

ROSC_STATIC_CALLBACK_HEAD__rosc_linux_test__simple2(sim2,simpleTopic2)
	printf("simple2 callback\n");
}


/*
 * This macro creates the data structure of the subscriber itself.
 * ROSC_STATIC_SUBSCRIBER_INIT__PACKAGENAME__MESSAGETYPE(USER_TYPE, TOPIC_REFERENCE, TOPIC_STRING)
 */
ROSC_STATIC_SUBSCRIBER_INIT__rosc_linux_test__simple1(sim1, simpleTopic1,"/simple1")
ROSC_STATIC_SUBSCRIBER_INIT__rosc_linux_test__simple2(sim2, simpleTopic2,"/simple2")

/*
 * This is a internal list for various hostnames and their IP adresses.
 * This is required, if you do not need that, you can leave it empty.
 * But this is mandatory!
 */
ROSC_STATIC_LOOKUP_TABLE_HEAD()
	ROSC_STATIC_LOOKUP_ENTRY(localhost,IP(127,0,0,1))
	ROSC_STATIC_LOOKUP_ENTRY(Computer,IP(192,168,0,3))
ROSC_STATIC_LOOKUP_TABLE_END

/*
 * This macro creates the string for your master URI
 */
MASTER_URI_STATIC("http://localhost:11311");

/*
 * And this macro specifies your node name, remember, this must be unique in your network!
 */
NODE_NAME("roscnode");

int main()
{
	/*
	 * This function initializes rosc stuff
	 */
	rosc_init();


	iface_list_insert(ROSC_STATIC_SUBSCRIBER__rosc_linux_test__simple1(sim1, simpleTopic1));
	iface_list_insert(ROSC_STATIC_SUBSCRIBER__rosc_linux_test__simple2(sim2, simpleTopic2));


	//register function still missing
	(ROSC_STATIC_SUBSCRIBER__rosc_linux_test__simple1(sim1, simpleTopic1))->state=IFACE_STATE_DO_REGISTER;
	(ROSC_STATIC_SUBSCRIBER__rosc_linux_test__simple2(sim2, simpleTopic2))->state=IFACE_STATE_DO_REGISTER;

	rosc_spin();
}
