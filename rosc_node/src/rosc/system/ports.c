/*
 * ports.c
 *
 *  Created on: 03.05.2013
 *      Author: choll
 */
#include <rosc/system/ports.h>
#include <rosc/system/spec.h>


#ifndef __SYSTEM_HAS_MALLOC__
	#ifndef PORTS_STATIC_MAX_NUMBER
		#error No port setting macro defined, define PORTS_DYNAMIC or PORTS_STATIC_MAX_NUMBER <maximal ports>
		#pragma message("nagnagnag" ".")
	#endif
#endif

#ifndef __SYSTEM_HAS_MALLOC__
	#ifndef	__BIGGEST_MESSAGE_TYPE_SIZE__
		#define __BIGGEST_MESSAGE_TYPE_SIZE__ sizeof(parse_act_t)
	#endif
#endif

