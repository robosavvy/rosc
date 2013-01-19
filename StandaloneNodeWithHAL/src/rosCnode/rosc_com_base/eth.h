/*
 * BSD 3-Clause License
 *
 * Copyright (c) 2013, Synapticon GmbH
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the distribution.
 *
 * Neither the name of the Synapticon GmbH nor the names of its contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * eth.h
 *
 * Created on: 12.01.2013
 *     Author: Christian Holl
 */

#ifndef ETH_H_
#define ETH_H_

#include <inttypes.h>


typedef unsigned char ip_address_t[4];
typedef char* host_name_t;
typedef uint16_t port_id;


extern ip_address_t system_ip;
extern ip_address_t master_ip;


/**
 * This function opens a port for listening.
 * If port is 0 the next possible port is tried to be opened.
 * If port contains a positive number, it tries to open exactly this port.
 * @return Port identifier, if 0 opening a port was not possible.
 *
 */
extern port_id listenPort(uint16_t port);

/**
 * This function connects to a server
 * @param target_ip The IP address of the remote system
 * @param remote_port The port on the remote system
 * @param local_port The port on the local system, if 0 is supplied it will take the next free port.
 * @return Port identifier, if 0 connection was not possible.
 */
extern port_id connectServer(ip_address_t target_ip, uint16_t remote_port, uint16_t local_port);


/**
 * Closes a port with the ID
 * @param portID
 */
extern void closeConnection(port_id portID);

/**
 * This function receives data from a port when available and returns the data length.
 * If not it returns 0 and in a case of a error a negative value.
 * @param portID
 * @param buffer
 * @param buffersize
 * @return number of received bytes
 */
extern uint32_t receiveFromPort(port_id portID, char* buffer, uint32_t buffersize);

/**
 * This function transmittes something over a open port
 * @param portID
 * @param buffer this points to the output buffer
 * @param len length of the output buffer
 * @return number of transmitted bytes
 */
extern uint32_t sendToPort(port_id portID, char* buffer, uint32_t len);

/**
 * This function automatically determines the IP address of the system if the adress in the global variable is set to zero.
 */
extern void auto_aquire_system_ip();


#endif /* ETH_H_ */
