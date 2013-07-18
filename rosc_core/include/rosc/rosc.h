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
 *  rosc.h created by Christian Holl
 */

#ifndef ROSC_SPIN_H_
#define ROSC_SPIN_H_

#include <rosc/debug/debug_out.h>
#include <rosc/system/spec.h>
#include <rosc/system/types.h>
#include <rosc/system/mem.h>
#include <rosc/com/ros_msg_common.h>
#include <rosc/system/iface.h>



#ifndef offsetof
	#error offsetof macro not found! Include stddef.h, or define it yourself inside spec.h
#endif


#ifndef __HOSTNAME_MAX_LEN__
	#warning __HOSTNAME_MAX_LEN__ undefined! Automatically set to 50.
	#define __HOSTNAME_MAX_LEN__ 50
#endif

#ifndef __NODENAME_MAX_LEN__
	#warning __NODENAME_MAX_LEN__ undefined! Automatically set to 50.
	#define __NODENAME_MAX_LEN__ 50
#endif

#ifndef __ROS_PARAMETER_MAX_LEN__
	#warning __ROS_PARAMETER_MAX_LEN__ undefined! Automatically set to 50.
	#define __ROS_PARAMETER_MAX_LEN__ 50
#endif


#ifdef __SYSTEM_HAS_MALLOC__
#error ROSC MALLOC IS NOT IMPLEMENTED YET THIS WILL ___NOT___ WORK ____ATM____!
#endif


#include <rosc/system/setup.h>
#include <rosc/system/rosc_spin.h>
#include <rosc/system/rosc_init.h>
#include <rosc/system/iface.h>



#endif /* ROSC_SPIN_H_ */
