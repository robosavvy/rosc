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
 *  xmlrpc.h created by Christian Holl
 */

#ifndef XMLRPC_H_
#define XMLRPC_H_


#include <rosc/system/spec.h>
#include <rosc/sebs_parse_fw/adv_modules/sebs_parse_http.h>
#include <rosc/sebs_parse_fw/adv_modules/sebs_parse_xml.h>
#include <rosc/sebs_parse_fw/std_modules/sebs_parse_copy2buffer.h>
#include <rosc/sebs_parse_fw/std_modules/sebs_parse_parseurl.h>
#include <rosc/sebs_parse_fw/std_modules/sebs_parse_numberparse.h>
#include <rosc/sebs_parse_fw/std_modules/sebs_parse_seekstring.h>
#include <rosc/sebs_parse_fw/std_modules/sebs_parse_skipwholemessage.h>



typedef enum
{
	__XMLRPC_HTTP_DESCRIPTORS(XMLRPC)
}xmlrpc_header_descriptors_t;

typedef enum
{
	__XMLRPC_HTTP_ACTIONS(XMLRPC)
}xmlrpc_server_actions_t;

typedef enum
{
	HTTP_METHOD_NOT_SET=-1,
	__XMLRPC_HTTP_METHODS(XMLRPC)
}xmlrpc_server_method_t;



typedef struct
{

	sebs_parser_data_t parser_data;
	union
	{
		sebs_parse_http_data_t http;
		sebs_parse_xml_data_t xml;
	}main_module_data;

	union
	{
		sebs_parse_seekstring_data_t seekstring;
		sebs_parse_numberparse_data_t numberparse;
		sebs_parse_parseurl_data_t parseurl;
	}sub_module_data;

}xmlrpc_server_data_t;

bool xmlrpc_server(xmlrpc_server_data_t *data, void** parser_data_ptr);

#endif /* XMLRPC_H_ */
