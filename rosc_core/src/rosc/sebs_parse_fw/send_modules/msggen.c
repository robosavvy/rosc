
#include <rosc/sebs_parse_fw/send_modules/msggen.h>

sebs_parse_return_t sebs_msggen(sebs_parser_data_t* pdata)
{
	sebs_msggen_t *fdata=(sebs_msggen_t *)pdata->current_parser.parser_data;
	if(pdata->function_init)
	{
		pdata->sending=true;
		pdata->function_init=false;
		fdata->first_run=true;
		fdata->cmds.submode =MSG_GEN_MODE_TYPE;
		fdata->cmds.def_state = 0;
		fdata->cmds.type = fdata->cmds.header;
		fdata->cmds.data = fdata->cmds.header_data;
		fdata->cmds.size.payload_size=0;
		fdata->cmds.size.payload_size_available=false;
		fdata->cmds.out.curPos=0;
		fdata->cmds.size.mode=MSG_GEN_SIZE_MODE_NONE;

	}

	if(*pdata->len==SOCKET_SIG_DATA_SENT || fdata->first_run)
	{

		//TODO check range for reset...
		XMLRPC_REQ_RESET();

		switch (fdata->type) {
			case MSGGEN_TYPE_XMLRPC_REQ_REGISTER_PUBLISHER_TOPIC:

				break;
			case MSGGEN_TYPE_XMLRPC_ERROR:
				//nothing
				break;
			case MSGGEN_TYPE_XMLRPC_REQUEST_TOPIC:

				break;
			default:
				break;
		}

		pdata->out_len=fdata->buffer_size;
		if(!msg_gen(fdata->buffer,&pdata->out_len,&fdata->cmds))
		{
			pdata->sending=false;
			return (SEBS_PARSE_RETURN_FINISHED);
		}
	}
	else
	{
		if(*pdata->len!=SOCKET_SIG_NO_DATA)
		{
			pdata->event=SEBS_MSGGEN_EVENT_SOCKET_SIGNAL;
		}
	}
	return (SEBS_PARSE_RETURN_GO_AHEAD);
}
