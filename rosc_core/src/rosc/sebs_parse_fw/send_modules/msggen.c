
#include <rosc/sebs_parse_fw/send_modules/msggen.h>

sebs_parse_return_t sebs_msggen(sebs_parser_data_t* pdata)
{
	sebs_msggen_t *fdata=(sebs_msggen_t *)pdata->current_parser.parser_data;

	if(pdata->function_init)
	{
		pdata->function_init=false;
	}

	switch (fdata->) {
		case value:

			break;
		default:
			break;
	}



//	msg_gen(pdata->additional_storage,rosc_static_socket_additional_data_size)
}
