#ifndef FORCE_INLINE
	#ifndef ENABLE_C
		#define ENABLE_C
	#endif

	#include <rosc/com_xml/parse/sub/seekstring.h>
#endif


#ifndef FORCE_INLINE
	/**
	 * This is a sub function for the parser in parser.c
	 * which is meant to be inlined by force into the parser function
	 * (if FORCE_INLINE is enabled) for saving space
	 * especially on platforms, where a function call is very "expensive"
	 * in memory reasons.
	 *
	 * The sub function seeks for a string inside a given string array.
	 * All the parameters are stored in the parsers struct, to be able
	 * to stream the parsing content.
	 *
	 * @param buf	  the input buffer
	 * @param len_ptr the lenght of the current input buffer
	 * @param pact    the parsers struct
	 */
	void seekstring(char *buf, uint32_t *len_ptr, parse_act_t *pact)
	 //work around for inlining the function
#endif
#ifdef ENABLE_C
{
	#ifndef FORCE_INLINE
			uint32_t len=*len_ptr;
	#endif

		if(pact->submode_state==PARSE_SUBMODE_INIT)
		{
			pact->submode_data.seekString.curChrPos=0;
			pact->submode_data.seekString.fit_min=0;
			pact->submode_data.seekString.fit_max=pact->submode_data.seekString.stringlist_len-1;
			pact->submode_state=PARSE_SUBMODE_RUNNING;
		}
		while(len > 0)
		{
			//Check if current char in buffer one is a separator
			bool isSeparator=false;
			const char *sep=pact->submode_data.seekString.endchrs;
			char curChrBuf=*buf;
			while(*sep!='\0')
			{
				if(curChrBuf==*sep)
				{
					curChrBuf='\0';
					isSeparator=true;
					pact->submode_data.seekString.separator=*sep;
					break;
				}
				++sep;
			}

			//Seek for the current char inside of all strings at given position
			bool found=false;//found at least one match
			unsigned int StrLstE;//Contains the current string list entry to be checked
			for(StrLstE=pact->submode_data.seekString.fit_min;StrLstE<=pact->submode_data.seekString.fit_max;StrLstE++)
			{
				const char *ptr=*(pact->submode_data.seekString.stringlist+StrLstE);
				char curChrStrLstE=*(ptr+pact->submode_data.seekString.curChrPos);

				//If the current char inside the current entry of the string array is finished
				if(curChrStrLstE == '\0')
				{
					if(isSeparator) //and the current char in the buffer is a separator as well
					{
						//write positive result
						pact->submode_result=StrLstE;
						found=true;
					}
					else //and no separator inside the buffer, the current value does not match
					{
						//so we skip this entry.
						pact->submode_data.seekString.fit_min=StrLstE+1;
					}
				}
				else //if the current char is no separator
				{
					//and does match the current buffer
					if(curChrStrLstE==curChrBuf)
					{
						//If this is the first match increase the first value to it
						if(!found)pact->submode_data.seekString.fit_min=StrLstE;
						found=true;
					}
					else //and does not match the current buffer
					{
						if(found) //and we found another suitable string before this one?
						{
							pact->submode_data.seekString.fit_max=StrLstE-1;
							//if it does not match, it will not do any more -> string sorting is considered to be alphabetic!
							break;
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
					DEBUG_PRINT(STR, "SEEKSTRING",  "NOT FOUND!");
					pact->submode_result=STRING_NOT_FOUND;
				}
				else
				{
					DEBUG_PRINT(STR, "SEEKSTRING",  pact->submode_data.seekString.stringlist[pact->submode_result]);
				}
				pact->submode_state=PARSE_SUBMODE_FINISHED;
				break;
			}



			buf++;
			pact->submode_data.seekString.curChrPos++;
			len--;
		}


}
#endif
