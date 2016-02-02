/*
 * TCPMessengerProtocol.h
 *
 *  Created on: Feb 13, 2013
 *      Author: efi
 */

#ifndef TCPMESSENGERPROTOCOL_H_
#define TCPMESSENGERPROTOCOL_H_

/**
 * TCP Messenger protocol:
 * all messages are of the format [Command 4 byte int]
 * and optionally data follows in the format [Data length 4 byte int][ Data ]
 */
#define MSNGR_PORT 3346
#define MAX_MSG_SIZE 1024
#define TIMEOUT 10

#define CLOSE_SESSION_WITH_PEER 	1
#define OPEN_SESSION_WITH_PEER 		2
#define EXIT						3
#define SEND_MSG_TO_PEER			4
#define SESSION_REFUSED				5
#define SESSION_ESTABLISHED			6
#define REGISTER					7
#define LOGIN						8
#define LIST_USERS					9
#define LIST_CONNECTED_USERS		10
#define LIST_ROOMS					11
#define LIST_ROOM_USERS				12
#define OPEN_ROOM					13
#define CLOSE_ROOM					14
#define USER_ENTER_ROOM				15
#define USER_LEAVE_ROOM				16
#define SUCCESS "SUCCESS"
#define FAILURE "FAILURE"

#define TEST_PEER_NAME "test"
#define SESSION_REFUSED_MSG "Connection to peer refused, peer might be busy or disconnected, try again later"

#define INVALID_OPERATION "Invalid operation"
#define GUEST_NAME "<guest>"

#endif /* TCPMESSENGERPROTOCOL_H_ */
