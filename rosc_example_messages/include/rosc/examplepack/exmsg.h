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

#ifndef EXMSG_H_
#define EXMSG_H_


//This is used to send out a message
typedef struct
{
	bool r_or_w;
	uint8_t *Bytes;
	uint16_t *Words;
}examplepack_exmsg_t;




#endif /* EXMSG_H_ */
