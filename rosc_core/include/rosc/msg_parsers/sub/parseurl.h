/*
 *	Copyright (c) 2013, Synapticon GmbH
 *	All rights reserved.
 *
 *	Redistribution and use in source and binary forms, with or without
 *	modification, are permitted provided that the following conditions are met:
 *
 *	1. Redistributions of source code must retain the above copyright notice, this
 *	   list of conditions and the following disclaimer.
 *	2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *	WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *	DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 *	ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *	(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *	LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *	ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *	SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *	The views and conclusions contained in the software and documentation are those
 *	of the authors and should not be interpreted as representing official policies,
 *	either expressed or implied, of the FreeBSD Project.
 *
 *  parseurl.h created by Christian Holl
 */

#ifndef PARSEURL_H_
#define PARSEURL_H_


/**
 *	Contains the result states parse url submode
 */
typedef enum
{
	PARSEURL_MATCH_HOSTNAME,//!< PARSEURL_MATCH_HOSTNAME - means that the current content inside the buffer will only match a hostname
	PARSEURL_MATCH_IPV4,    //!< PARSEURL_MATCH_IPv4 - means that the content is a IPv4 address
	PARSEURL_MATCH_IPV6,   	//!< PARSEURL_MATCH_IPv6 - means that the content is a IPv6 address
	PARSEURL_MATCH_IPv6_RESOLV,//!< PARSEURL_MATCH_IPv6_RESOLV - means that the content is a IPv6 address with a IPv4 network resolving addition
}parseurl_match_t;

/**
 * This struct stores the data for the parseurl submode
 */
typedef struct
{
	char hostname_ip_char[__HOSTNAME_MAX_LEN__]; //!< The text form of the hostname/IP
	uint16_t cur_pos; //!< curLen The current size of the copied chars
	uint16_t IPv6[8];//!< storage for an IPv6 address
	uint8_t IPv4[4]; //!< storage for an IPv4 address or the resolving end of IPv6
	uint16_t port; //!< storage for a port number
	parse_url_match_t what; //!< what specifies what kind of address is given
}parseurl_data_t;

/**
 * This function parses a URL from a stream
 * @param buf A pointer to the storage of the buffer
 * @param len The variable pointing to the length variable of the current buffer
 * @param data the function data storage, must be initialized in the beginning!
 * @return true when finished
 *
 * @TODO implement parseurl, needs more state information, protocol string list etc...
 */
void parseurl(char **buf, int32_t *len, numberParse_data_t *data);

#endif /* PARSEURL_H_ */
