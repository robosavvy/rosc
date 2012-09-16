/*
 *  Copyright by Synapticon GmbH (www.synapticon.com)  ©2012
 *
 *  msg_parse.c
 *
 *  This file is part of ROScNode Library.
 *
 *  ROScNode Library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  ROScNode Library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with ROScNode Library. If not, see <http://www.gnu.org/licenses/>.
 *
 *	File created by Christian Holl on 11.09.2012
 */

#include "msg_parse.h"

const char *methods[]=
{
		""
		"HTTP",
};




typedef enum
{
	method,
	header,
	xml,
}parse_state_message_part;


typedef enum
{
	SEEKWORD_NOT_IN_LIST=-1,
	SEEKWORD_STRINGEND=-2,

}seekWordStatus;


unsigned int seekWord( const char** wordptr,
						 const char** wordlist,
						 unsigned int wordlist_len,
						 const char* separator_list,
						 bool start)
{
	static unsigned int first; //first word of the word list possibilities
	static unsigned int last;	 //last word of the word list possibilities
	static unsigned int pos;   //current position inside the word

	if(start)
	{
		last=wordlist_len;
		first=0;
		pos=0;
	}

	while((**wordptr)!='\0')
	{

		//Check if current one is a separator
		bool isSeparator=false;
		const char *sep=separator_list;
		char current=**wordptr;
		while(*sep!='\0')
		{
			if(current==*sep)
			{
				current='\0';
				isSeparator=true;
				break;
			}
			++sep;
		}

		//Seek for the current char inside of all strings at pos
		bool found=false;//found at least one match
		unsigned int w;
		for(w=first;w<last;w++)
		{
			const char *ptr=*(wordlist+w);
			char c=*(ptr+pos);
			if(c==current)
			{

				if(isSeparator) return w;
				if(!found)first=w; //If this is the first match increase the first value to it
				found=true;
			}
			else
			{
				if(found) //found another suitable string before?
				{
					last=w;
					break; //if it does not match, it will not do that any more -> alphabetic
				}
			}
		}
		if(!found)return SEEKWORD_NOT_IN_LIST;
		(*wordptr)++;
		pos++;

	}
	return SEEKWORD_STRINGEND;
}







unsigned int stringParse(const char* str, unsigned int len,  int (*handler)(int event), bool start)
{
//	static const skip_chars_method=" "
//	static parse_state_message_part part;
//	static char* current_word[100];
//	static unsigned int current_word_len;






	return 0;
}


