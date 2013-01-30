/*
 *  Copyright by Synapticon GmbH (www.synapticon.com)  ©2013
 *
 *  exmsg.h
 *
 *  This file is part of rosc_example_messages.
 *
 *  rosc_example_messages is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  rosc_example_messages is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with rosc_example_messages. If not, see <http://www.gnu.org/licenses/>.
 *
 *	File created by choll on 30.01.2013
 */



/*
 * Message File
 * bool   r_or_w
 * uint8  Bytes[]
 * uint16 Words[]
 */

#include <rosc/system/types.h>
#include <rosc/com_ifaces/iface_setup.h>

#ifndef EXMSG_H_
#define EXMSG_H_


//This is used to send out a message for every configuration
#pragma pack(push,1)

typedef struct
{
	bool r_or_w;

	struct
	{
	uint32_t size;
	uint8_t *data;
	}Bytes;

	struct
	{
	uint32_t size;
	uint16_t *data;
	}Words;

}examplepack_exmsg_t;

#pragma pack(pop)

#define FIXED_SIZE_EXAMPLEPACK_EXMSG_STRUCT(__STRUCTNAME,__BYTES_SIZE, __WORDS_SIZE)\
		struct\
		{\
			bool r_or_w;\
			\
			struct\
			{\
			uint32_t size;\
			}Bytes;\
			\
			struct\
			{\
			uint32_t size;\
			uint16_t data[__WORDS_SIZE];\
			}Words;\
		}__STRUCTNAME;

#define FIXED_SIZE_EXAMPLEPACK_EXMSG_MARSHALLING_CMDS(__BYTES_SIZE, __WORDS_SIZE)\


#endif /* EXMSG_H_ */
