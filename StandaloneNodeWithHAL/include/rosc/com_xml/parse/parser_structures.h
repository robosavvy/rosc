
#ifndef PARSER_STRUCTURES_H_
#define PARSER_STRUCTURES_H_

#include <rosc/rosc.h>
#include <rosc/com_xml/parse/sub/http_header_parsing.h>
#include <rosc/com_xml/parse/sub/xml_parsing.h>
#include <rosc/com_xml/parse/sub/parseurl.h>

/**
 * This enum contains the two main modes of the parser
 */
typedef enum
{
	PARSE_MODE_HEADER,//!< PARSE_MODE_HEADER is when parsing the header
	PARSE_MODE_XML,   //!< PARSE_MODE_XML is when parsing the xml
}parse_mode_t;

/**
 * This enum contains the values for different commands from the handler function back to the parser
 */
typedef enum
{
	PARSE_NONE_GO_AHEAD,  //!< PARSE_NONE_GO_AHEAD means that the parser just should go on
	PARSE_SEEKSTRING,     //!< PARSE_SEEKSTRING Seek a string inside a string array
	PARSE_NUMBERPARSE,    //!< PARSE_NUMBERPARSE Parse a unsigned integer number
	PARSE_COPY2BUFFER,    //!< PARSE_COPY2BUFFER Copy a string to the buffer
	PARSE_SKIPWHOLEMESSAGE//!< PARSE_SKIPWHOLEMESSAGE Truncate whole message (for errors)
}parse_ctrl_t;

/**
 * This enum contains the states for the current submode
 */
typedef enum
{
	PARSE_SUBMODE_NONE,            //!< PARSE_SUBMODE_NONE means that no submode is active
	PARSE_SUBMODE_SEEKSTRING,      //!< PARSE_SUBMODE_SEEKSTRING means that the parser currently seeks a string inside an array
	PARSE_SUBMODE_NUMBERPARSE,     //!< PARSE_SUBMODE_NUMBERPARSE means that currently the number parser is in operation
	PARSE_SUBMODE_COPY2BUFFER,     //!< PARSE_SUBMODE_COPY2BUFFER means that currently a string is copied into another location
	PARSE_SUBMODE_SKIPUNTILCHAR,   //!< PARSE_SUBMODE_SKIPUNTILCHAR means that currently everything is skipped till a specified char is received
	PARSE_SUBMODE_SKIPWHOLEMESSAGE,//!< PARSE_SUBMODE_SKIPWHOLEMESSAGE
}parse_submode_t;


/**
 * Definition for handler function type
 */
typedef struct parse_act_t
{
	void (*handler_fkt)(/*parse_act_t pact*/);
	uint32_t xml_length;
	uint32_t xml_curChr;
	parse_mode_t mode;
	parse_submode_t submode;
	parse_ctrl_t ctrl_command;

	union
	{
		struct
		{
			parse_http_state_t state;
			parse_http_desc_t descriptor;
		}http;

		struct
		{
			uint16_t depth;
			parse_xml_tags_t current_tag;
			parse_xml_state_t state;
		}xml;
	}mode_data;

	union
	{
		struct
		{
			char **stringlist;
			uint16_t fit_min;
			uint16_t fit_max;
			uint16_t result;
		}seekString;

		struct
		{
			uint8_t figure_number;
			uint8_t figure_max;
			uint32_t result;

		}numberParse;

		struct
		{
			char* buffer;
			uint16_t max_len;
		}copy2Buffer;

		struct
		{
			char* chrlst;
			bool negative;
		}skipUntilChr;

		struct
		{
			char hostname_ip_char[__HOSTNAME_MAX_LEN__];
			uint16_t curLen;
			uint16_t IPv6[8];
			uint8_t IPv4[4];
			parse_url_match_t what;
		}parseURL;

	}submode_data;

}parse_act_t;


/**
 * Definition for handler function type
 */
typedef void (*parse_handler_fct)(parse_act_t *pact);

#endif /* PARSER_STRUCTURES_H_ */
