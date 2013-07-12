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
#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>
#include <rosc/msg/rosc_linux_test/test_builtin.h>
#include <rosc/rosc.h>



ROSC_STATIC_MSG_BUILDUP__rosc_linux_test__test_builtin();

ROSC_STATIC_MSG_USER_DEF__rosc_linux_test__test_builtin(test,3,3,3,3,3,3,3,3,3,3,3,3,3);


ROSC_STATIC_SYSTEM_MESSAGE_TYPE_LIST_BEGIN
	ROSC_SIZE_LIST_ENTRY__rosc_linux_test__test_builtin(test)
ROSC_STATIC_SYSTEM_MESSAGE_TYPE_LIST_END


ROSC_STATIC_CALLBACK_HEAD__rosc_linux_test__test_builtin__(test,cb)
{

}




int main()
{

	rosc_init();

	int i=ROS_MSG_BUILDUP_TYPE_STRING;
	for (i = 1; i <= rosc_static_msg_memory_offsets__rosc_linux_test__test_builtin__test[0]; ++i) {

		printf("%i, ",(int) rosc_static_msg_memory_offsets__rosc_linux_test__test_builtin__test[i]);
	}
	printf("\n");
}
