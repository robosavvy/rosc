
#ifndef PARSEURL_H_
#define PARSEURL_H_

/**
 *	Contains the result states parse url submode
 */
typedef enum
{
	PARSE_URL_MATCH_HOSTNAME,   //!< PARSE_URL_MATCH_HOSTNAME - means that the current content inside the buffer will only match a hostname
	PARSE_URL_MATCH_IPv4,       //!< PARSE_URL_MATCH_IPv4 - means that the content is a IPv4 address
	PARSE_URL_MATCH_IPv6,       //!< PARSE_URL_MATCH_IPv6 - means that the content is a IPv6 address
	PARSE_URL_MATCH_IPv6_RESOLV,//!< PARSE_URL_MATCH_IPv6_RESOLV - means that the content is a IPv6 address with a IPv4 network resolving addition
}parse_url_match_t;


#endif /* PARSEURL_H_ */
