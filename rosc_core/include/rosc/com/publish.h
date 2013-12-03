/*
 * publish.h
 *
 *  Created on: 03.12.2013
 *      Author: choll
 */

#include <rosc/system/eth.h>

#ifndef PUBLISH_H_
#define PUBLISH_H_



void publish(iface_t *interface, void *msg);
void publisherfill(iface_t *interface, void *msg, socket_t* cur);

#endif /* PUBLISH_H_ */
