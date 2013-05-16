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
 *  numberparse.c created by Christian Holl
 */

#ifndef FORCE_INLINE
	#ifndef ENABLE_C
		#define ENABLE_C
	#endif

	#include <rosc/msg_parsers/sub/numberparse.h>
#endif


#ifndef FORCE_INLINE
	void numberparse(char **buf_ptr, uint32_t *len_ptr, parse_act_t *pact)
	 //work around for inlining the function
#endif
#ifdef ENABLE_C
{
	#ifndef FORCE_INLINE
			uint32_t len=*len_ptr;
			char *buf=*buf_ptr;
	#endif

	if(pact->submode_state==PARSE_SUBMODE_INIT)
	{
		pact->submode_state=PARSE_SUBMODE_RUNNING;
		pact->submode_data.numberParse.cur_place=0;
		pact->submode_data.numberParse.number=0;
	}
	while(len > 0)
	{
		if(*buf>=48 && *buf<=57)
		{
			if(pact->submode_data.numberParse.cur_place >= pact->submode_data.numberParse.figure_max)
			{
				pact->submode_result=NUMBERPARSE_MAX_FIGURES;
				break;
			}
			else
			{
				pact->submode_data.numberParse.number*=10;
				pact->submode_data.numberParse.number+=*buf-48;
				++pact->submode_data.numberParse.cur_place;
				++buf;
				--len;
			}
		}
		else
		{
			pact->submode_result=NUMBERPARSE_ANOTHER_CHAR;
			pact->submode_state=PARSE_SUBMODE_FINISHED;
			pact->submode=PARSE_SUBMODE_NONE;
			break;
		}
	}
	if(pact->submode_result==NUMBERPARSE_ANOTHER_CHAR)
	{
		if(pact->submode_data.numberParse.cur_place == 0)
		pact->submode_result=NUMBERPARSE_ERROR_NONUMBER;
	}

	#ifndef FORCE_INLINE
		*len_ptr=len;
		*buf_ptr=buf;
	#endif


}
#endif

