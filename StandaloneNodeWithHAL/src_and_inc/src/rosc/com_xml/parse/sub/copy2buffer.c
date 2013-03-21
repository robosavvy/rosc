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
 *  copy2buffer.c created by Christian Holl on 20.03.2013
 */


#ifndef FORCE_INLINE
	#ifndef ENABLE_C
		#define ENABLE_C
	#endif

	#include <rosc/com_xml/parse/sub/copy2buffer.h>
#endif


#ifndef FORCE_INLINE
	void copy2buffer(char *buf, uint32_t *len_ptr, parse_act_t *pact)
	 //work around for inlining the function
#endif
#ifdef ENABLE_C
{
	#ifndef FORCE_INLINE
			uint32_t len=*len_ptr;
	#endif

	const char *sep=pact->submode_data.copy2Buffer.endChrs;
	while(len > 0)
	{
		bool isEndChar=false;
		while(*sep!='\0')
		{
			if(*buf==*sep)
			{
				isEndChar=true;
				break;
			}
			++sep;
		}
		if((pact->submode_data.copy2Buffer.cur_pos+1<=pact->submode_data.copy2Buffer.max_len) && !isEndChar )
		{
			pact->submode_data.copy2Buffer.buffer[pact->submode_data.copy2Buffer.cur_pos]=*buf;
			pact->submode_data.copy2Buffer.cur_pos++;
			len--;
			buf++;
		}
		else if(isEndChar)
		{
			pact->submode_result=COPY2BUFFER_ENDCHR;
		}
		else
		{
			pact->submode_result=COPY2BUFFER_MAXLEN;
		}
		buf++;
		len--;
	}
}
#endif

