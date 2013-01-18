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


unsigned int seekString( const char** stringptr,
						 const char** stringlist,
						 unsigned int stringlist_len,
						 const char* separator_list,
						 bool start)
{
	static unsigned int first; //first string of the string list possibilities
	static unsigned int last;	 //last string of the string list possibilities
	static unsigned int pos;   //current position inside the string

	if(start)
	{
		last=stringlist_len;
		first=0;
		pos=0;
	}

	while((**stringptr)!='\0')
	{

		//Check if current one is a separator
		bool isSeparator=false;
		const char *sep=separator_list;
		char current=**stringptr;
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
			const char *ptr=*(stringlist+w);
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
		if(!found)return SEEKSTRING_NOT_IN_LIST;
		(*stringptr)++;
		pos++;

	}
	return SEEKSTRING_STRINGEND;
}

int parseStringUInt(const char **buffer, bool goAhead)
{
	static int cnt;
	static int value;
	if(goAhead==false)
	{
		cnt=0;
		value=0;
	}
	char currentChar=(**buffer);
	//Go ahead while the current char is a number
	while(currentChar>=48 && currentChar<=48+9)
	{
		//If this is not the first time multiply by ten
		if(cnt) value*=10;
		//Convert the char figure into integer and add it to the value
		value+=currentChar-48;

		//Going ahead with the next char
		(*buffer)++;
		currentChar=(**buffer);
		cnt++;
	}
	if(!cnt) return PARSE_INT_NO_NUMBER;
	else
		if(currentChar!='\0')
			return value;
		else
			return PARSE_INT_STR_END;
}

inline void skipSpace(const char** buffer)
{
	while((**buffer)==' ')
	{
		(*buffer)++;
	}
}

void skipAllTillSeperators(const char** buffer, char *separator_list)
{
	char currentChar=**buffer;
	bool isSeparator=false;
	while(currentChar!='\0' && !isSeparator)
	{
		const char *sep=separator_list;
		while(*sep!='\0')
		{
			if(currentChar==*sep)
			{
				currentChar='\0';
				isSeparator=true;
				break;
			}
			++sep;
		}
		++(*buffer);
		currentChar=**buffer;
	}
}

inline bool checkforSpecialChr(const char **buffer, char chr)
{
	bool res=(**buffer==chr);
	++(*buffer);
	return res;
}

inline unsigned int parseHttpMethod(const char **buffer, const char **methods)
{
	unsigned int state=0;
	int param;
	int ires;
	int ret;
	const char **strList;
	while(state<=13)
	{

		//  POST / HTTP/1.1\n
		switch(state)
		{
		case 3: case 7:
			param='/'; //We expect to find that char here
			break;
		case 10:
			param='.'; //We expect to find that char here (Version Dot)
			break;
		case 13:
			param='\n'; //We expect to find that a new line
			break;
		case 1:
			strList=methods; //Set string array for methods
			param=HTTP_METHODS_LEN;
			break;
		case 5:
			*strList="HTTP"; //Use the seek function for only one word
			param=1;
			break;
		}

		switch(state)
		{
		//Skipping spaces inside the Method string
		case 0: //at beginning
		case 2: //after method
		case 4: //after first /
		case 6: //after HTTP
		case 8:	//after second /
		case 12: //after version
			skipSpace(buffer);
			break;
		case 3: case 7: case 10: case 13:
			if(!checkforSpecialChr(buffer,param))
				return false;
			break;
			case 1: /*METHOD*/
			case 5: /*HTTP*/
				ires=seekString(buffer,(const char**)strList,HTTP_METHODS_LEN,parse_separators[PARSE_METHOD_SEP_METHOD],true);
				if(state == 1)
					ret = ires+HTTP_METHOD_PARSE_INVALID+1;
				if( ires<0 )
					if(state == 5)
						return HTTP_METHOD_PARSE_INVALID;
					else
						return HTTP_METHOD_PARSE_UNKNOWN;
				break;
			case 9: case 11://Parse Version String
				ires=parseStringUInt(buffer,0);
				if(ires>=HTTP_METHOD_PARSE_INVALID)
				{
					if(state==9 && ires!=1)
							return HTTP_METHOD_PARSE_INVALID;
				}
				else
				{
					return HTTP_METHOD_PARSE_INVALID;
				}
				break;
			default:
					return HTTP_METHOD_PARSE_INVALID;
				break;
		}
		state++;
	}
	return ret;
}

