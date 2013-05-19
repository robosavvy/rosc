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
 *  seekstring.c created by Christian Holl
 */

#include <rosc/msg_parsers/sub/seekstring.h>


///@todo Enhancement notice -- increase saved pointer instead of fit_min variable (decrease stringlist len by current number!)
//decrease stringlist len instead of fit_max
bool seekstring(char **buf, int32_t *len, seekstring_data_t *data)
{
		while(*len > 0)
		{
			//Check if current char in buffer one is a separator
			bool isSeparator=false;
			const char *sep=data->endchrs;
			char curChrBuf=**buf;
			while(*sep!='\0')
			{
				if(curChrBuf==*sep)
				{
					curChrBuf='\0';
					isSeparator=true;
					break;
				}
				++sep;
			}

			//Seek for the current char inside of all strings at given position
			bool found=false;//found at least one match
			unsigned int StrLstE;//Contains the current string list entry to be checked
			for(StrLstE=data->fit_min;StrLstE<data->fit_max;StrLstE++)
			{
				const char *ptr=*(data->stringlist+StrLstE);
				char curChrStrLstE=*(ptr+data->curChrPos);

				//If the current char inside the current entry of the string array is finished
				if(curChrStrLstE == '\0')
				{
					if(isSeparator) //and the current char in the buffer is a separator as well
					{
						//write positive result
						data->result=StrLstE;
						found=true;
					}
					else //and no separator inside the buffer, the current value does not match
					{
						//so we skip this entry.
						data->fit_min=StrLstE+1;
					}
				}
				else //if the current char is no separator
				{
					bool match=false;
					if(curChrStrLstE==curChrBuf)
					{
						match=true;
					}
					else if(!data->case_sensitive) //if it is not case sensitive
					{
						if(curChrBuf>=65 && curChrBuf<=90) //a-z
						{
							if(curChrStrLstE==curChrBuf+32)
								match=true;
						}
						else if(curChrBuf>=97 && curChrBuf<=122) //A-Z
						{
							if(curChrStrLstE==curChrBuf-32)
								match=true;
						}
					}

					//and does match the current buffer
					if(match)
					{
						//If this is the first match increase the first value to it
						if(!found)
							data->fit_min=StrLstE;
						found=true;
					}
					else //and does not match the current buffer
					{
						if(found) //and we found another suitable string before this one?
						{
							data->fit_max=StrLstE;
							//if it does not match, it will not do any more -> string sorting is considered to be alphabetic!
							break;
						}
						else
						{
							data->fit_min=StrLstE+1;
						}
					}
				}
			}
			//If a separator is found inside the current iteration and the string was not found
			//it is not inside the string array.
			if(isSeparator)
			{
				if(!found)
				{
					DEBUG_PRINT_STR("UNKNOWN");
					data->result=SEEKSTRING_STRING_NOT_FOUND;
				}
				else
				{
					DEBUG_PRINT(STR,"SEEKSTRING",data->stringlist[data->result]);
				}
				return true;
			}

			++*buf;
			data->curChrPos++;
			--*len;
		}
		return false;
}
