#include "MultipleTCPSocketsListener.h"

/*
 * Add the given socket to the socket list to be listen on
 */
void MultipleTCPSocketsListener::addSocket(TCPSocket* socket )
{
	//TODO: add he given socket to the socket list
	if (socket == NULL)
	{
		return;
	}

	this->sockets.push_back(socket);
}

/*
 * Set the given sockets to the socket list to be listen on
 */
void MultipleTCPSocketsListener::addSockets(vector<TCPSocket*> socketVec)
{
	//TODO: set the given sockets as the socket list to selct from
	for (unsigned int i = 0; i < socketVec.size(); i++)
	{
		addSocket(socketVec[i]);
	}
}
TCPSocket* MultipleTCPSocketsListener::listenToSocket()
{
	//TODO: create local set for the select function (fd_set)
	fd_set readFd;
	FD_ZERO(&readFd);


	//TODO: fill the set with file descriptors from the socket list using (FD_SET macro)
	int biggestSocket = findBiggestFd(readFd);

	//TODO: perform the select
	int numOfActiveFds = select(biggestSocket + 1, &readFd, NULL, NULL, NULL);

	//TODO: check the returned value from the select to find the socket that is ready
	if (numOfActiveFds > 0)
	{
		return NULL;
	}

	//TODO: if select return a valid socket return the matching TCPSocket object otherwise return NULL
	return NULL;
}

int MultipleTCPSocketsListener::findBiggestFd(fd_set readFd)
{
	int biggest = 0;

	for (unsigned int i = 0; i < this->sockets.size(); i++)
	{
		int currentFd = this->sockets[i]->getSocketFd();
		FD_SET(currentFd, &readFd);

		if (currentFd > biggest)
		{
			biggest = currentFd;
		}
	}

	return biggest;
}

