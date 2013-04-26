#ifndef XMLRPC_SERVER_H_
#define XMLRPC_SERVER_H_

#include <rosc/com_xml/parse/parser_structure.h>
#include <rosc/com_xml/parse/handler/xmlrpc_string_id.h>



typedef struct xmlrpc_server_data_t
{
	xmlrpc_server_method_t method;
	xmlrpc_server_target_t target;
	xmlrpc_slave_api_method_t rpcmethod;
	uint8_t value_number;
	uint32_t current_value_tag;
	uint8_t array_value;
	bool fetch_content;
	bool content_is_mandatory;
	uint32_t content_cnt;


	uint8_t respond_code;

	bool error_message;

	union
	{
		struct
		{
			char caller_id[__NODENAME_MAX_LEN__];
			char parameter_key[__ROS_PARAMETER_MAX_LEN__];
		}paramUpdate;

		struct
		{
			char caller_id[__NODENAME_MAX_LEN__];
			char parameter_key[__ROS_PARAMETER_MAX_LEN__];
		}publisherUpdate;

		struct
		{
			char caller_id[__NODENAME_MAX_LEN__];
			char parameter_key[__ROS_PARAMETER_MAX_LEN__];
		}requestTopic;

	};



}xmlrpc_server_data_t;


void xmlrpc_server_handler(parse_act_t * pact);

#endif /* XMLRPC_SERVER_H_ */
