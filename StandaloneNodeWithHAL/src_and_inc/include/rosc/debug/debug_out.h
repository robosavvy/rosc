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
 *  debug_out.h created by Christian Holl on 28.03.2013
 */

#ifndef DEBUG_OUT_H_
#define DEBUG_OUT_H_

#ifdef __DEBUG__PRINTS__
	#ifndef __DEBUG__WITH__XMOS__PRINTH__
		#include <stdio.h>
		#define STR "%s\n"
		#define INT "%i\n"
		#define CHR "%c\n"
	#else
		#include <print.h>
		#define STR str
		#define INT int
		#define CHR chr
	#endif


		#ifndef __DEBUG__WITH__XMOS__PRINTH__
					#define DEBUG_PRINT(STR_INT_CHR, DESCR,  VARIABLE)\
						printf(DESCR);\
						printf(": ");\
						printf(STR_INT_CHR,VARIABLE)

					#define	DEBUG_PRINT_STR(STR)\
						printf(STR);\
						printf("\n")

		#else
					#define DEBUG_PRINT(STR_INT_CHR, DESCR,  VARIABLE)\
					printstr(DESCR);\
					printstr(": ");\
					print ## STR_INT_CHR ## ln(VARIABLE)

					#define	DEBUG_PRINT_STR(STR)\
						printstrln(STR)
		#endif

#else
	#define DEBUG_PRINT(STR_INT_CHR, DESCR,  VARIABLE)
	#define	DEBUG_PRINT_STR(STR)
#endif
#endif /* DEBUG_OUT_H_ */
