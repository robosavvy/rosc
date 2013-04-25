


#ifndef PARSER_STRUCTURE_H_
#define PARSER_STRUCTURE_H_

#include <rosc/string_res/msg_strings.h>
#include <rosc/rosc.h>


#define __XML_MAX_DEPTH__ 20

/**
 *	Contains the result states parse url submode
 */
typedef enum
{
	PARSE_URL_MATCH_HOSTNAME,   //!< PARSE_URL_MATCH_HOSTNAME - means that the current content inside the buffer will only match a hostname
	PARSE_URL_MATCH_IPv4,       //!< PARSE_URL_MATCH_IPv4 - means that the content is a IPv4 address
	PARSE_URL_MATCH_IPv6,   	//!< PARSE_URL_MATCH_IPv6 - means that the content is a IPv6 address
	PARSE_URL_MATCH_IPv6_RESOLV,//!< PARSE_URL_MATCH_IPv6_RESOLV - means that the content is a IPv6 address with a IPv4 network resolving addition
}parse_url_match_t;


/**
 * This enum contains the state for the xml parsing
 */
typedef enum
{
	PARSE_XML_INIT,
	PARSE_XML_ROOT,
	PARSE_XML_INNER,
	PARSE_XML_TAG_START,
	PARSE_XML_TAG,
	PARSE_XML_CLOSE_TAG_START,
	PARSE_XML_EXPECT_SELFCLOSE_TAG_END,
	PARSE_XML_CDATA,
	PARSE_XML_CDATA_START,
	PARSE_XML_CDATA_EXPECT_OPEN_BRACKET,
	PARSE_XML_CDATA_FIRST_BRACKET, //TODO RENAME _CLOSE_
	PARSE_XML_CDATA_SECOND_BRACKET,//TODO RENAME _CLOSE_
	PARSE_XML_QMTAG_EXPECT_CLOSE,
	PARSE_XML_COMMENT_START_1ST_DASH,
	PARSE_XML_COMMENT,
	PARSE_XML_COMMENT_END_1ST_DASH,
	PARSE_XML_COMMENT_END_2ND_DASH,
	PARSE_XML_ATTRIBUTE_WAIT_EQUAL,
	PARSE_XML_ATTRIBUTE_WAIT_VAL_BOUND,
	PARSE_XML_ATTRIBUTE_INSIDE_APOSTROPHE_ATTRIB,
	PARSE_XML_ATTRIBUTE_INSIDE_QUOTES_ATTRIB,
	PARSE_XML_INNER_CONTENT,
}parse_xml_state_t;

typedef enum
{
	PARSE_XML_SUB_STATE_NONE,
	PARSE_XML_SUB_TAG_ID,
	PARSE_XML_SUB_ATTRIBUTE_ID,
	PARSE_XML_SUB_COMMENT_END_1ST_DASH,
	PARSE_XML_SUB_COMMENT_END_2ND_DASH,
	PARSE_XML_SUB_CDATA_TAG_STRING,
}parse_xml_sub_state_t;

/**
 * This enum contains the number for each known tag
 */
typedef enum
{
	XML_TAG_UNKNOWN=-2,
	XML_TAG_NONE=-1,
	__RPC_XML_TAG_STRINGS(XML) //!< imports all string numbers from autogenerated file
}parse_xml_tags_t;

/**
 * XML Attributes
 */
typedef enum
{
	XML_ATTRIBUTE_UNKNOWN=-2,
	XML_ATTRIBUTE_NONE=-1,
	__RPC_XML_ATTRIBUTE_STRINGS(XML)
}parse_xml_attrib_t;



typedef enum
{
	XML_TAG_TYPE_NORMAL,
	XML_TAG_TYPE_CLOSE,
	XML_TAG_TYPE_QUESTION_MARK,
	XML_TAG_TYPE_EXCLAMATION_MARK,
	XML_TAG_TYPE_COMMENT,
	XML_TAG_TYPE_CDATA,
}
parse_xml_tag_type_t;


/**
 * Contains events for the handler for it to know what currently happened
 */
typedef enum
{
	//Errors
	PARSE_EVENT_ERROR_CONTENT_LENGTH_TOO_LONG=-1,
	PARSE_EVENT_ERROR_CONTENT_LENGTH=-2,
	PARSE_EVENT_ERROR_NOT_FOUND_404=-3,
	PARSE_EVENT_ERROR_VERSION_NOT_SUPPORTED_505=-4,
	PARSE_EVENT_ERROR_HTTP_BAD_REQUEST=-5,
	PARSE_EVENT_ERROR_LENGTH_REQUIRED=-6,
	PARSE_EVENT_ERROR_HTTP_METHOD_NOT_ALLOWED=-7,
	PARSE_EVENT_ERROR_HTTP_CONTENT_TYPE=-8,
	PARSE_EVENT_ERROR_HTTP_CONTENT_ENCODING=-9,
	PARSE_EVENT_ERROR_XML_DEPTH=-10,
	PARSE_EVENT_ERROR_MALFORMED_XML=-11,



	PARSE_EVENT_NONE=0,
	PARSE_EVENT_HANDLER_INIT,
	PARSE_EVENT_HTTP_METHOD_PARSED,
	PARSE_EVENT_HTTP_ACTION_PARSED,
	PARSE_EVENT_HTTP_HEADER_FIELD_CONTENT,


	//XML
	PARSE_EVENT_TAG,
	PARSE_EVENT_INSIDE_TAG,
	PARSE_EVENT_TAG_INSIDE,
	PARSE_EVENT_HANDLER_CALLED_SUBMODE_FINISHED,

	PARSE_EVENT_CONTENT_START,
}parse_event_t;





/**
 * This enum contains the number of each known http method
 */
typedef enum
{
	__HTTP_METHODS(ENUM) //!< imports all string numbers from autogenerated file
}parse_http_methods_t;


/**
 * Contains the header descriptors
 */
typedef enum
{
	__HTTP_HEADER_DESCRIPTORS(PARSE_HTTP)//!< __HTTP_HEADER_DESCRIPTORS inserts auto generated list of header descriptors
}parse_http_desc_t;

/**
 * Substates for handling the results of called submodes in http parser
 */
typedef enum
{
	/*Special states for parsing a request (server)*/
	PARSE_HTTP_STATE_REQUEST_METHOD,
	PARSE_HTTP_STATE_REQUEST_ACTION,
	PARSE_HTTP_STATE_REQUEST_HTTP_VER,

	/*Special states for parsing a response (client)*/
	PARSE_HTTP_STATE_RESPONSE_HTTP_VER,
	PARSE_HTTP_STATE_RESPONSE_CODE,
	PARSE_HTTP_STATE_RESPONSE_STRING,

	/*Common states*/
	PARSE_HTTP_STATE_HEADLINE_WAIT_END,
	PARSE_HTTP_STATE_DESCRIPTOR_OR_HEADER_END,
	PARSE_HTTP_DESCRIPTOR_FIELD_SEPARATOR,
	PARSE_HTTP_STATE_FIELD,
	PARSE_HTTP_STATE_FIELD_CONTENT,
}parse_http_state_t;


typedef enum
{
	PARSE_HTTP_SUB_STATE_NONE,
	PARSE_HTTP_SUB_CHECK_METHOD,
	PARSE_HTTP_SUB_CHECK_ACTION,
	PARSE_HTTP_SUB_CHECK_REQUEST_HTTP_VER,
	PARSE_HTTP_SUB_CHECK_DESCRIPTOR_ID,
	PARSE_HTTP_SUB_CHECK_CONTENT_LENGTH,
	PARSE_HTTP_SUB_CHECK_CONTENT_TYPE,
	PARSE_HTTP_SUB_CHECK_RESPONSE_HTTP_VER,
	PARSE_HTTP_SUB_CHECK_RESPONSE_CODE,
	PARSE_HTTP_SUB_CHECK_RESPONSE_STATE,

}parse_http_sub_state_t;


/**
 * This enum contains the two main modes of the parser
 */
typedef enum
{
	PARSE_MODE_HEADER,//!< PARSE_MODE_HEADER is when parsing the header
	PARSE_MODE_XML,   //!< PARSE_MODE_XML is when parsing the xml
}parse_mode_t;


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
 * Values for the state of the current submode
 */
typedef enum
{
	PARSE_SUBMODE_INIT,    //!< PARSE_SUBMODE_INIT means it was not initialized yet
	PARSE_SUBMODE_RUNNING, //!< PARSE_SUBMODE_RUNNING means that the submode is running
	PARSE_SUBMODE_FINISHED,//!< PARSE_SUBMODE_FINISHED means that the submode has finshed
}parse_submode_state_t;





/**
 * Definition for handler function type
 */
typedef struct parse_act_t
{
	void (*handler_fkt)(void *); //!< This function handles the parser events
	void *handler_data_storage;	//!< This is a pointer for different handlers data storage
	int32_t content_length; //!< xml_length stores the length parsed from the http header

	uint32_t content_curChr; //!< xml_curChr keeps the current xml char number
	parse_mode_t mode; //!< mode saves contains the current main mode, xml or http header parsing
	parse_submode_t submode; //!< submode is the current sub mode the parser is using

	parse_submode_state_t submode_state; //!< is one when submode is finished
	int16_t submode_result;	//!< contains the result code from each submode when finished
	bool submode_by_handler; //!< needs to be set when the submode is started by the handler function
	parse_event_t event; //!< tells the handler function what currently has happened.




	/**
	 * The mode_data union stores the http and the xml data inside the same memory location.
	 * This is used for saving memory space, because the variables are not used the same time.
	 */
	union
	{
		struct
		{
			parse_http_state_t state; //!< state contains the current state of the http parser
			parse_http_sub_state_t sub_state; //!< contains the substate of the http parser (analyzing results of submodes)
			parse_http_desc_t descriptor; //!< descriptor contains the current known descriptor
			uint8_t content_type_text_xml_found; //!< checks if the text/xml content type was found.

		}http;

		struct
		{
			uint16_t depth;	//!< depth stores the depth of the current location of the parser
			uint32_t processed_bytes; //!<
			parse_xml_tags_t current_tag; //!< current_tag contains the current known xml tag
			parse_xml_tag_type_t tag_type; //!< normal, ! or ? tag.
			parse_xml_attrib_t attribute; //!< shows the attribute if inside an attribute otherwise XML_ATTRIBUTE_NONE
			parse_xml_state_t state; //!< contains the current state of the xml parser
			parse_xml_sub_state_t sub_state; //!< gives additional state information
			parse_xml_tags_t tags[__XML_MAX_DEPTH__]; //!< array for checking tag validity
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
			uint16_t stringlist_len; //!< Length of the stringlist to be checked for the string
			uint16_t curChrPos;	//!< The char number since the start of seekString
			uint16_t fit_min;	//!< The beginning of the range with possibly matching strings
			uint16_t fit_max;	//!< The end of the range with possibly matching strings
			char separator; //!< the separator which was found
		}seekString;

		struct
		{
			uint8_t cur_place; //!< cur_place stores the number of the current figure of the number
			uint8_t figure_max; //!< figure_max stores the maximum of figures to be allowed
			uint32_t number; //!< contains the parsed number
		}numberParse;

		struct
		{
			char* buffer; //!< buffer points to the place where the chars have to be stored in memory.
			uint16_t cur_pos; //!< cur_pos stores the amount of already copied chars.
			uint16_t max_len; //!< max_len is the maximum length to be used for the buffer.
			char* endChrs; //!< chrs which will mark the end of the string
		}copy2Buffer;

		struct
		{
			char* chrs; //!< chrs to skip/ to stop skip, according to negative
			bool negative; //!< negative, if true the chars inside chrs will stop skipping
			char fndchr; //!< the chr which was found
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
typedef void (*parse_handler_fct_cast)(void *pact);




#endif /* PARSER_STRUCTURE_H_ */
