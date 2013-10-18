
#include <rosc/sebs_parse_fw/send_modules/msggen.h>

sebs_parse_return_t sebs_msggen(sebs_parser_data_t* pdata)
{
	sebs_msggen_t *fdata=(sebs_msggen_t *)pdata->current_parser.parser_data;

	if(pdata->function_init)
	{
		pdata->function_init=false;
	}

	switch (fdata->type) {
		case MSGGEN_MESSAGE_TYPE_ERROR:

			break;
		default:
			break;
	}

	pdata->out_len=fdata->buffer_size;
	msg_gen(pdata->additional_storage,&pdata->out_len,&fdata->cmds);

	return (SEBS_PARSE_RETURN_GO_AHEAD);
}
