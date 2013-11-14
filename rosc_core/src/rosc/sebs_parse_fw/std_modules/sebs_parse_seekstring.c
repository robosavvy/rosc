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
 *  sebs_parse_seekstring.c created by Christian Holl
 */

#include <rosc/sebs_parse_fw/std_modules/sebs_parse_seekstring.h>

//decrease stringlist len instead of fit_max
sebs_parse_return_t sebs_parse_seekstring(sebs_parser_data_t* pdata)
{
	sebs_parse_seekstring_data_t *fdata =
			(sebs_parse_seekstring_data_t *) pdata->current_parser.parser_data;
	if (pdata->function_init)
	{
		pdata->function_init = false;
		fdata->curChrPos = 0;
		fdata->result = 0;
	}

	while (*pdata->len > 0)
	{



		//Check if current char in buffer one is a separator
		bool isSeparator = false;
		const char *sep = fdata->endchrs;
		char curChrBuf = **pdata->buf;
		while (*sep != '\0')
		{
			if (curChrBuf == *sep)
			{
				curChrBuf = '\0';
				break;
			}
			++sep;
		}

		int8_t casechange = 0; //will contain upper-lower case conversion when not case-sensitive and alphabetic char
		if (!fdata->case_sensitive) //if it is not case sensitive
		{
			if (curChrBuf >= 65 && curChrBuf <= 90) //a-z
			{
				casechange = 32;
			}
			else if (curChrBuf >= 97 && curChrBuf <= 122) //A-Z
			{
				casechange = -32;
			}
		}

		bool found = false; //found at least one match
		uint16_t newlen = 0; //the new length of the string list, when finding a non-suitable after one or more suitable strings
		//Find the first possibility in remaining strings on current place
		while (fdata->stringlist_len > 0 && fdata->stringlist_len > newlen)
		{
			if ((fdata->stringlist[newlen])[fdata->curChrPos] == curChrBuf
					|| (fdata->stringlist[newlen])[fdata->curChrPos]
							== (curChrBuf + casechange))
			{
				found = true;
				newlen++;
			}
			else
				if (!found)	//If no suitable string was found before
				{
					++fdata->result; //Increase the current array number
					++fdata->stringlist; //Increase the stringlist pointer
					--fdata->stringlist_len; //deacrease stringlist length
				}
				else
				{
					//If something was found before at that place means that all strings
					//after this one here are not suitable
					fdata->stringlist_len = newlen;
					break;
				}
		}

		//Check if maxlength is set and reached...
		if(fdata->max_length!=0 && fdata->curChrPos==fdata->max_length-1)
		{
			if(fdata->endchrs[0]=='\0') //If there is no endchr, max legth is the specified ending...
			{
				curChrBuf = '\0';
			}
			else
			{
				fdata->result = SEBS_PARSE_SEEKSTRING_NOT_FOUND_MAX_LENGTH;
				return (SEBS_PARSE_RETURN_FINISHED);
			}
		}

		//If we have a separator the string must end here so
		if (curChrBuf == '\0')
		{
			if (found) //If found is not true here, the string is not inside the list
			{
				DEBUG_PRINT(STR,"SEEKSTRING RESULT", fdata->stringlist[0]);
			}
			else
			{
				fdata->result = SEBS_PARSE_SEEKSTRING_NOT_FOUND;
				DEBUG_PRINT_STR("SEEKSTRING: !STRING NOT FOUND!");
			}
			//End
			return (SEBS_PARSE_RETURN_FINISHED);
		}
		++*pdata->buf;
		fdata->curChrPos++;
		--*pdata->len;



	}
	return (SEBS_PARSE_RETURN_GO_AHEAD);
}
