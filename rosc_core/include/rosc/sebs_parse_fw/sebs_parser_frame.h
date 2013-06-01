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
 *   sebs_parser_frame.h created by Christian Holl
 */

/*
 * SEBS_PARSE_FW
 * _S_tatic memory, _E_vent _B_ased, _S_tream-Parsing FrameWork
 */

#ifndef SEBS_PARSER_FRAME_H_
#define SEBS_PARSER_FRAME_H_

#include <rosc/system/types.h>



#define SEBS_PARSE_HANDLER_FUNCTION_CALL()\\
		return_to_handler

#define SEBS_PARSE_UNKNOWN  -1

#define SEBS_PARSE_EVENT_NONE 0
#define SEBS_PARSE_EVENT_HANDLER_CALL_FUNCTION_END -1
#define SEBS_PARSE_EVENT_LEN_SMALLER_ZERO -2
#define SEBS_PARSE_EVENT_MESSAGE_SECURITY_OVER_SIZE -3

//forward declaration of struct
struct sebs_parser_data_t;




/**
 * These are the return values for parsers and handler functions
 */
typedef enum
{
	SEBS_PARSE_RETURN_GO_AHEAD,//!< means that there is no more data to parse at the moment
	SEBS_PARSE_RETURN_FINISHED, //!< means that the function has finished and should be replaced by the caller function
	SEBS_PARSE_RETURN_INIT,     //!< means that the current parser- or handler-function wants to call a function (sets init_function to true)
	SEBS_PARSE_RETURN_INIT_ADV, //!< means that a advanced module is initialized from which does use events instead of returns
}sebs_parse_return_t;

/**
 * This is the function pointer to a parser function
 * @param pdata struct containing the parser data for the current parser.
 * @param fdata parser specific struct containing information for the current parser function
 * @return
 */
typedef sebs_parse_return_t (*sebs_parse_function_t)(struct sebs_parser_data_t* pdata);


typedef sebs_parse_return_t (*sebs_parse_handler_function_t)(struct sebs_parser_data_t* pdata);

/**
 * This struct stores a parser function and the data location for calling it
 */
typedef struct
{
	sebs_parse_function_t parser_function; //!< Parser-function to be called
	void* parser_data; //!< Parser data to call the parser function with
} sebs_parser_call_t;



/**
 * This struct stores the current data for the parser
 * it needs to be stored in the handler_data and
 * the link inside the handler_data must point on to the
 * handler_data storing the current parser_data_t.
 */
typedef struct sebs_parser_data_t
{
	bool handler_init;
	void* handler_init_data;
	bool function_init;

	/**
	 * This is the function with its data
	 * called by the parser normally
	 *
	 * If the function pointer is zero, the
	 * handler function will be called with
	 * EVENT_INIT
	 */
	sebs_parser_call_t current_parser;

	/**
	 * This is the pointer to the function with
	 * which current_parser will be replaced,
	 * when it returns with trsebs_parser_call_ts_parser_call_t next_parser;
	 */
	sebs_parser_call_t next_parser;

	/**
	 * Event output of advanced parser-functions
	 * which are supplied with the pointer
	 * to parser_data_t
	 */
	int32_t event;

	/**
	 * When set to true and the current parsing function returns true,
	 * the parser function will be called with the event HANDLER_CALL_FUNCTION_END
	 *
	 * If it's set to false, the current_parser will be replaced by the values of next_parser.
	 */
	bool return_to_handler;

	/**
	 * The pointer to the handler_data struct
	 * containing this struct
	 */
	void *handler_data;

	/**
	 * This stores the event handler function
	 */
	sebs_parse_handler_function_t handler_function;

	/**
	 * Message Size Limit
	 * This specifies a message size limit, when it is reached the void *dataevent
	 * SEBS_PARSE_EVENT_SIZE_REACHED is set.
	 *
	 * The length is checked at the beginning by adding up buffer length
	 * to a variable at the entry of the function.
	 *
	 * If the variable is set to zero, this feature is disabled.
	 */
	uint32_t security_len;

	/**
	 * The current length of the input buffer
	 * can be used to calculate the parsed overall length.
	 * overall_len + call_len - len
	 */
	uint32_t call_len;

	/**
	 * The current length of the processed message
	 * (updates after finishing the current buffer content)
	 */
	uint32_t overall_len;


	/**
	 * Pointer of the current input buffer
	 */
	char **buf;

	/**
	 * The current length variable
	 */
	int32_t *len;
} sebs_parser_data_t;

sebs_parser_data_t* sebs_parser_init(void *handler_data, sebs_parse_handler_function_t handler_function);

/**
 * This is the base function for the streaming parsers
 * @param buf the input buffer
 * @param len length of the current input buffer
 * @param data the Data for the parser
 */
void sebs_parser_frame(char *buf, int32_t len, sebs_parser_data_t* data);






#endif /* SEBS_PARSER_FRAME_H_ */
