/*
 *  Copyright by Synapticon GmbH (www.synapticon.com)  ?2013
 *
 *  rosinit.h
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
 *	File created by choll on Jan 28, 2013
 */


#include <rosc/system/rosc_init.h>
#include <rosc/system/setup.h>
#include <rosc/system/ports.h>

#include <stdio.h> //TODO Remove
void rosc_init()
{
	DEBUG_PRINT(STR,"","ROSC_INIT");
	ports_info_init();
}
