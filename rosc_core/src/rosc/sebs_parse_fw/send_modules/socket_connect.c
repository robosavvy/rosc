#include <rosc/sebs_parse_fw/send_modules/socket_connect.h>
#include <rosc/debug/debug_out.h>

sebs_parse_return_t socket_connect(sebs_parser_data_t* pdata)
{
	socket_connect_data_t* fdata = pdata->current_parser.parser_data;

	if (pdata->function_init)
	{

		pdata->function_init = false;
		switch (fdata->state)
		{
		case CONNECT_DATA_STATE_URL:
		{
			fdata->state = SOCKET_CONNECT_STATE_URL_SCHEME;
		}
			break;

		default:
			fdata->state = SOCKET_CONNECT_STATE_CONNECT;
			break;
		}
	}

	char *url = fdata->connect_data->url;
	while (fdata->state != SOCKET_CONNECT_STATE_ERROR
			&& fdata->state != SOCKET_CONNECT_STATE_CONNECT)
	{
		switch (fdata->state)
		{
		case SOCKET_CONNECT_STATE_URL_SCHEME:
			//Maybe seek for string http / rosrpc (we skip that here for now)
			while (1)
			{
				if (*url < 48)
					break;
				else if (*url > '9' && *url < 'A')
					break;
				else if (*url > 'Z' && *url < 'a')
					break;
				else if (*url > 'z')
					break;
				else
					//Next char
					url++;
			}

			bool ok = false;
			if (*url == ':')
				if (*(url + 1) == '/')
					if (*(url + 2) == '/')
					{
						ok = true;
						url += 3;
						if (*url == '[')
						{
							url++;
							fdata->state = SOCKET_CONNECT_STATE_URL_IPV6;
						}
						else
						{
							fdata->state =
									SOCKET_CONNECT_STATE_URL_IPV4_HOSTNAME;
						}
						fdata->connect_data->hostname_size = 0;
						fdata->connect_data->hostname = url;
					}

			if (!ok)
				fdata->state = SOCKET_CONNECT_STATE_ERROR;
			break;

		case SOCKET_CONNECT_STATE_URL_IPV6:
			//NOT SUPPORTED YET
			fdata->state = SOCKET_CONNECT_STATE_ERROR;
			break;

		case SOCKET_CONNECT_STATE_URL_IPV4_HOSTNAME:
		{
			fdata->connect_data->data_state=CONNECT_DATA_STATE_IPV4;
			uint8_t digit=0, dot, *ip = fdata->connect_data->remote_ip;
			while (1)
				if ((*url >= '0' && *url <= '9') || *url == '.')
				{

					if (*url == '.')
					{
						dot++;
						if (dot == 4)
						{
							fdata->state = SOCKET_CONNECT_STATE_URI_HOSTNAME;
							break;
						}
						digit = 0;
						ip++;
					}
					else
					{
						digit++;
						if (digit == 4)
						{
							fdata->state = SOCKET_CONNECT_STATE_URI_HOSTNAME;
							break;
						}
						else
						{
							if (digit > 1)
							{
								(*ip) *= 10;
							}
							(*ip) += *url - 48;

						}

					}
					url++;
					fdata->connect_data->hostname_size++;
				}
				else
				{
					if (*url == ':')
					{
						if (dot == 3)
						{
							fdata->state = SOCKET_CONNECT_STATE_URL_PORT;
							url++;
						}
						else
						{
							fdata->state=SOCKET_CONNECT_STATE_ERROR;
						}
					}
					else
					{
						fdata->state = SOCKET_CONNECT_STATE_URI_HOSTNAME;
					}
					break;
				}

			break;
		}


		case SOCKET_CONNECT_STATE_URL_PORT:
		{
			uint8_t digit=0;
			fdata->connect_data->remote_port=0;
			while(1)
				if(*url >= '0' && *url <= '9')
				{
					if(digit!=0)
					{
						fdata->connect_data->remote_port*=10;
					}
					fdata->connect_data->remote_port+=*url-48;
					digit++;
					url++;
				}
				else
				{

					if((*url=='\0' || *url=='/') && digit!=0)
					{
						fdata->state=SOCKET_CONNECT_STATE_CONNECT;
					}
					else
					{
						fdata->state=SOCKET_CONNECT_STATE_ERROR;
						break;
					}
					break;
				}
		}
			break;

		case SOCKET_CONNECT_STATE_URI_HOSTNAME:
			while (1)
			{
				fdata->connect_data->data_state=CONNECT_DATA_STATE_RESOLVE;
				if (((*url < 48) || (*url > '9' && *url < 'A')
						|| (*url > 'Z' && *url < 'a') || (*url > 'z'))
						&& *url != '_'
						&& *url != '-'
						&& *url != '.')
				{
					if (*url == ':')
					{
						fdata->state = SOCKET_CONNECT_STATE_URL_PORT;

						url++;
						break;
					}
					else
					{
						fdata->state = SOCKET_CONNECT_STATE_ERROR;
						break;
					}
				}
				else
				{
					fdata->connect_data->hostname_size++;
					url++;
				}
			}
			break;
		}
	}

	if(fdata->state == SOCKET_CONNECT_STATE_CONNECT)
	{


		DEBUG_PRINT_STR("Connecting to:");
		DEBUG_PRINT(INT,"Port",fdata->connect_data->remote_port);
		DEBUG_PRINT(INT,"IP",fdata->connect_data->remote_ip[0]);
		DEBUG_PRINT(INT,"IP",fdata->connect_data->remote_ip[1]);
		DEBUG_PRINT(INT,"IP",fdata->connect_data->remote_ip[2]);
		DEBUG_PRINT(INT,"IP",fdata->connect_data->remote_ip[3]);


		pdata->out_len=SOCKET_SIG_CONNECT;


		printf("Hostname %.*s\n", fdata->connect_data->hostname_size, fdata->connect_data->hostname);
		return (SEBS_PARSE_RETURN_GO_AHEAD);
	}
	else
	{
		DEBUG_PRINT_STR("ERROR in connection details!");
		fdata->state=CONNECT_DATA_STATE_ERROR;
		pdata->sending=false;

	}

	return (SEBS_PARSE_RETURN_GO_AHEAD);
}
