#ifndef XMLRPC_SERVER_H_
#define XMLRPC_SERVER_H_

#include <rosc/com_xml/parse/parser_structure.h>
#include <rosc/com_xml/parse/handler/xmlrpc_string_id.h>
#include <rosc/string_res/msg_strings.h>


typedef struct xmlrpc_server_data_t
{
	xmlrpc_server_method_t method;
	xmlrpc_server_target_t target;
	xmlrpc_slave_api_method_t rpcmethod;

	union rpcmethodData
	{
		struct getBusStats
		{
			char caller_id[__NODENAME_MAX_LEN__];
		};

		struct getBusInfo
		{
			char caller_id[__NODENAME_MAX_LEN__];
		};

		struct getMasterUri
		{
			char caller_id[__NODENAME_MAX_LEN__];
		};

		struct shutdown
		{
			char caller_id[__NODENAME_MAX_LEN__];
		};

		struct getPid
		{
			char caller_id[__NODENAME_MAX_LEN__];
		};

		struct getSubscriptions
		{
			char caller_id[__NODENAME_MAX_LEN__];
		};

		struct getPublications
		{
			char caller_id[__NODENAME_MAX_LEN__];
		};

		struct paramUpdate
		{
			char caller_id[__NODENAME_MAX_LEN__];
			char parameter_key[__ROS_PARAMETER_MAX_LEN__];
		};

		struct publisherUpdate
		{
			char caller_id[__NODENAME_MAX_LEN__];
			char parameter_key[__ROS_PARAMETER_MAX_LEN__];
		};

		struct requestTopic
		{
			char caller_id[__NODENAME_MAX_LEN__];
			char parameter_key[__ROS_PARAMETER_MAX_LEN__];
		};

	};
}xmlrpc_server_data_t;


void xmlrpc_server_handler(parse_act_t * pact);

#endif /* XMLRPC_SERVER_H_ */
