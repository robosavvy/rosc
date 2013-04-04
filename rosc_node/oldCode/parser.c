#include "parser.h"

inline void httpxml_parse_act_init(parse_act_t *pact, void *data_storage, parse_handler_fct handler_fct)
{
	pact->state=PARSE_ACT_STATE_HTTP_METHOD;
	pact->handler_data_storage=data_storage;
	pact->handler_fct=handler_fct;
}


void xmlrpc_parse(char *buf, uint32_t len, parse_act_t* pact)
{
#if 1
	printf("Chunk Length: %i\n",len);
#endif


	switch(pact->state)
	{
	//#################################################
	//#############PARSING HTTP METHOD#################
	//#################################################
	case PARSE_ACT_STATE_HTTP_METHOD:


		break;

	//#################################################
	//##########SEEK/PARSE XML DEFINITION##############
	//#################################################
	case PARSE_ACT_STATE_XML_DEF:

		break;

	}

}


void xmlrpc_handler_test_fct(parse_act_t *pact)
{

}
