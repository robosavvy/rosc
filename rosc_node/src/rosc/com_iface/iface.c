
#include <rosc/com_ifaces/iface.h>

iface_t interface_list_hub;


void init_hub_node()
{
	interface_list_hub.type=IFACE_TYPE_LIST_HUB;
}


void registerInterface_static(iface_t *interface, const char *topic_service_name, const iface_definition_t* iface_def)
{
	iface_t* cur=&interface_list_hub;
	//Go to the end of the list
	while(cur->next != 0 && cur->next != interface) cur=cur->next;

	if(cur->next != interface && cur->next == 0)
	{
		cur->next=interface;
	}

}


void removeInterface_static(iface_t *interface)
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
