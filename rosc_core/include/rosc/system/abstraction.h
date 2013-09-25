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
 *  abstraction.h created by Christian Holl
 */

#ifndef ABSTRACTION_H_
#define ABSTRACTION_H_

#include <rosc/system/types.h>

#ifdef HW_IFACE_REQUIRED_I2C
	extern void abstract_init_i2c(void *interface);
	extern void abstract_i2c(void *interface, bool rw, uint16_t address, uint8_t reg,  uint8_t *data, size_t size);
#endif

#ifdef HW_IFACE_REQUIRED_SPI
	extern void abstract_init_spi(void *interface);
	extern void abstract_rw_spi(void* interface, uint8_t* in_buffer, uint8_t out_buffer, size_t size);
#endif

#ifdef HW_IFACE_REQUIRED_PARALLEL
	extern void abstract_init_parallel(void *interface);
	extern void abstract_parallel(void* interface, bool rw, void* data_io);
#endif

#ifdef HW_IFACE_REQUIRED_SERIAL
	extern void abstract_init_serial(void *interface);
	extern void abstract_write_serial(void* interface, void* data_io);
	extern void abstract_read_serial(void* interface, void* data_io);
#endif

#ifdef HW_IFACE_SUPPORT_TIME
	extern void abstract_get_time(uint32_t* sec, uint32_t* nsec);
#endif

#ifdef HW_IFACE_SUPPORT_SET_TIME
	extern void abstract_set_time(uint32_t* sec, uint32_t* nsec);
#endif





#endif /* ABSTRACTION_H_ */
