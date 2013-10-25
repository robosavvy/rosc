#include <rosc/sebs_parse_fw/send_modules/socket_connect.h>


sebs_parse_return_t socket_connect(sebs_parser_data_t* pdata)
{
	socket_connect_info_t* fdata=pdata->current_parser.parser_data;




	return (SEBS_PARSE_RETURN_GO_AHEAD);
}
