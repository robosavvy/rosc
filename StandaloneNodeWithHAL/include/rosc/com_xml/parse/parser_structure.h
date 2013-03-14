
#ifndef PARSER_STRUCTURE_H_
#define PARSER_STRUCTURE_H_

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
	PARSE_SUBMODE_SKIPWHOLEMESSAGE,//!< PARSE_SUBMODE_SKIPWHOLEMESSAGE means that the parser is currently skipping the whole message
}parse_submode_t;


/**
 * Definition for handler function type
 */
typedef struct parse_act_t
{
	void (*handler_fkt)(/*parse_act_t pact*/); //!< This function handles the parser events
	void *handler_data_storage;	//!< This is a pointer for different handlers data storage
	uint32_t xml_length; //!< xml_length stores the length parsed from the http header
	uint32_t xml_curChr; //!< xml_curChr keeps the current xml char number
	parse_mode_t mode; //!< mode saves contains the current main mode, xml or http header parsing
	parse_submode_t submode; //!< submode is the current sub mode the parser is using
	parse_ctrl_t ctrl_command; //!< ctrl_command contains current command from the handler to the parser

	/**
	 * The mode_data union stores the http and the xml data inside the same memory location.
	 * This is used for saving memory space, because the variables are not used the same time.
	 */
	union
	{
		struct
		{
			parse_http_state_t state; //!< state contains the current state of the http parser
			parse_http_desc_t descriptor; //!< descriptor contains the current known descriptor
		}http;

		struct
		{
			uint16_t depth;	//!< depth stores the depth of the current location of the parser
			parse_xml_tags_t current_tag; //!< current_tag contains the current known xml tag
			parse_xml_state_t state; //!< contains the current state of the xml parser
		}xml;
	}mode_data;

	/**
	 * submode_data contains the data for the different submodes
	 */
	union
	{

		struct
		{
			char **stringlist;	//!< The stringlist
			char * endchrs;		//!< endchrs is a list of characters (string) that will end the seek like "<" when reading inside tags
			uint16_t curChr;	//!< The char number since the start of seekString
			uint16_t fit_min;	//!< The beginning of the range with possibly matching strings
			uint16_t fit_max;	//!< The end of the range with possibly matching strings
			uint16_t result;	//!< This is the string found in the array, if no string was found this is -1.
		}seekString;

		struct
		{
			uint8_t cur_place; //!< cur_place stores the number of the current figure of the number
			uint8_t figure_max; //!< figure_max stores the maximum of figures to be allowed
			uint32_t result; //!< storing the result of the parsing and the steps between.
		}numberParse;

		struct
		{
			char* buffer; //!< buffer points to the place where the chars have to be stored in memory.
			uint16_t cur_len; //!< cur_len stores the amount of already copied chars.
			uint16_t max_len; //!< max_len is the maximum length to be used for the buffer.
			uint8_t result; //TODO create type
		}copy2Buffer;

		struct
		{
			char* chrs; //!< chrs to skip/ to stop skip, according to negative
			bool negative; //!< negative, if true the chars inside chrs will stop skipping
		}skipUntilChr;

		struct
		{
			char hostname_ip_char[__HOSTNAME_MAX_LEN__]; //!< The text form of the hostname/IP
			uint16_t curLen; //!< curLen The current size of the copied chars
			uint16_t IPv6[8];//!< storage for an IPv6 address
			uint8_t IPv4[4]; //!< storage for an IPv4 address or the resolving end of IPv6
			parse_url_match_t what; //!< what specifies what kind of address is given
		}parseURL;

	}submode_data;

}parse_act_t;


/**
 * Definition for handler function type
 */
typedef void (*parse_handler_fct)(parse_act_t *pact);

#endif /* PARSER_STRUCTURE_H_ */
