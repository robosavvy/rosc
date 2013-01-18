/*
 *  Copyright by Synapticon GmbH (www.synapticon.com)  ©2013
 *
 *  eth.h
 *
 *  This file is part of InterfacePorts.
 *
 *  InterfacePorts is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  InterfacePorts is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with InterfacePorts. If not, see <http://www.gnu.org/licenses/>.
 *
 *	File created by choll on 10.01.2013
 */

#ifndef ETH_H_
#define ETH_H_

#include <inttypes.h>

typedef unsigned char ip_address_t[4];
typedef uint16_t port_id;

/**
 * This function opens a port for listening.
 * If port is 0 the next possible port is tried to be opened.
 * If port contains a positive number, it tries to open exactly this port.
 * @return Port identifier, if 0 opening a port was not possible.
 *
 */
port_id listenPort(uint16_t port);

/**
 * This function connects to a server
 * @param target_ip The IP address of the remote system
 * @param remote_port The port on the remote system
 * @param local_port The port on the local system, if 0 is supplied it will take the next free port.
 * @return Port identifier, if 0 connection was not possible.
 */
port_id connectServer(ip_address_t target_ip, uint16_t remote_port, uint16_t local_port);


/**
 * Closes a port with the ID
 * @param portID
 */
void closeConnection(port_id portID);

/**
 * This function receives data from a port when available and returns the data length.
 * If not it returns 0 and in a case of a error a negative value.
 * @param portID
 * @param buffer
 * @param buffersize
 * @return number of received bytes
 */
uint32_t receiveFromPort(port_id portID, char* buffer, uint32_t buffersize);

/**
 * This function transmittes something over a open port
 * @param portID
 * @param buffer this points to the output buffer
 * @param len length of the output buffer
 * @return number of transmitted bytes
 */
uint32_t sendToPort(port_id portID, char* buffer, uint32_t len);

#endif /* ETH_H_ */
