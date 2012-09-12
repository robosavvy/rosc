/*
 *  Copyright by Synapticon GmbH (www.synapticon.com)  ©2012
 *
 *  debugutilities.c
 *
 *  This file is part of StandaloneNodeWithHAL.
 *
 *  StandaloneNodeWithHAL is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  StandaloneNodeWithHAL is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with StandaloneNodeWithHAL. If not, see <http://www.gnu.org/licenses/>.
 *
 *	File created by choll on 12.09.2012
 */


#include "debugutilities.h"
#include <stdio.h>

void printbuffer(char* buffer, unsigned int len)
{
	int i;
	for(i=0;i<len;++i)
	{
		printf("%c",buffer[i]);
	}
}
