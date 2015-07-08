/* 
 * File:   serial.h
 * Author: steph
 *
 * Created on 8 July 2015, 10:23 AM
 */

#ifndef SERIAL_H
#define	SERIAL_H

#include "message.h"

struct toControlMsg *recieveMessage();
void sendMessage(struct toNUCMsg *msg);

#endif	/* SERIAL_H */

