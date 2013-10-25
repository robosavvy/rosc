#include <rosc/sebs_parse_fw/send_modules/socket_connect.h>


sebs_parse_return_t socket_connect(sebs_parser_data_t* pdata)
{
	socket_connect_info_t* fdata=pdata->current_parser.parser_data;

	if(pdata->function_init)
	{
		pdata->function_init=false;
		switch(fdata->data_state)
		{
		case CONNECT_DATA_STATE_URL:
			{
				fdata->hostname
			}
			break;

		case CONNECT_DATA_STATE_RESOLVE:
			break;
		case CONNECT_DATA_STATE_IP:
			break;
		}
	}



	return (SEBS_PARSE_RETURN_GO_AHEAD);
}
