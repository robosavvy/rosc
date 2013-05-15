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
 *  iface.c created by Christian Holl
 */

#include <rosc/com_ifaces/iface.h>
#include <rosc/system/spec.h>

static iface_t interface_list_hub;


void rosc_init_interface_list()
{
	interface_list_hub.type=IFACE_TYPE_LIST_HUB;
	interface_list_hub.name=0;
	interface_list_hub.next=0;
}


void register_interface_static(iface_t *interface, const char *topic_service_name, const iface_definition_t* iface_def)
{
	iface_t* cur=&interface_list_hub;
	//Go to the end of the list
	while(cur->next != 0 && cur->next != interface) cur=cur->next;

	if(cur->next != interface && cur->next == 0)
	{
		cur->next=interface;
	}
}


void remove_interface_static(iface_t *interface)
{
	iface_t* cur=&interface_list_hub;
	iface_t* last;
	//Go to the entry of the list
	while(cur && cur != interface)
	{
		last=cur;
		cur=cur->next;
	}

	if(cur==interface)
	{
		last->next=cur->next;
	}
}
