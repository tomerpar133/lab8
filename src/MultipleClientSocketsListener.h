/*
 * MultipleClientSocketsListener.h
 *
 *  Created on: Feb 14, 2013
 *      Author: Tomer hagever
 */

#ifndef MULTIPLECLIENTSOCKETSLISTENER_H_
#define MULTIPLECLIENTSOCKETSLISTENER_H_

#include <iostream>
#include <stdio.h>
#include <vector>
#include <sys/time.h>
#include "TCPMessengerProtocol.h"
#include "TCPSocket.h"
#include "Client.h"

using namespace std;


class MultipleClientSocketsListener{
private:
	vector<Client*> clients;

public:
	/*
	 * Add the given socket to the socket list to be listen on
	 */
	void addClient(Client*);

	/*
	 * Add the given sockets to the socket list to be listen on
	 */
	void addClients(vector<Client*>);

	/*
	 * This method checks to see if any of the previous given sockets are ready for reading
	 * It returns one of the Sockets that are ready.
	 */
	Client* listenToClients();

private:
	/**
	 * This method returns the biggest fd in the vector or 0 if not found
	 */
	int findBiggestFd(fd_set* readFd);
};


#endif /* MULTIPLECLIENTSOCKETSLISTENER_H_ */
