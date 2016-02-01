#include "MultipleClientSocketsListener.h"
#include <sys/select.h>
//#include <libexplain/select.h>
/*
 * Add the given socket to the socket list to be listen on
 */
void MultipleClientSocketsListener::addClient(Client* client)
{
	//TODO: add he given socket to the socket list
	if (client == NULL)
	{
		return;
	}
	this->clients.push_back(client);
}

/*
 * Set the given sockets to the socket list to be listen on
 */
void MultipleClientSocketsListener::addClients(vector<Client*> clientVec)
{
	//TODO: set the given sockets as the socket list to selct from
	for (unsigned int i = 0; i < clientVec.size(); i++)
	{
		addClient(clientVec[i]);
	}
}
Client* MultipleClientSocketsListener::listenToClients()
{
	if (clients.empty())
	{
		return NULL;
	}
	
	//TODO: create local set for the select function (fd_set)
	fd_set readFd;
	FD_ZERO(&readFd);


	//TODO: fill the set with file descriptors from the socket list using (FD_SET macro)
	
	int biggestSocket = 0;
	
	for (unsigned int i = 0; i < this->clients.size(); i++)
	{
		int currentFd = this->clients[i]->getSocket()->getSocketFd();
		FD_SET(currentFd, &readFd);

		if (currentFd > biggestSocket)
		{
			biggestSocket = currentFd;
		}
	}
	//TODO: perform the select
	struct timeval timeout;
	timeout.tv_sec = TIMEOUT;
	timeout.tv_usec = 0;
	int numOfActiveFds = select(biggestSocket + 1, &readFd, NULL, NULL, &timeout);
	//TODO: check the returned value from the select to find the socket that is ready
	Client* activeClient = NULL;
	if (numOfActiveFds > 0)
	{
		for (unsigned int i = 0; i < this->clients.size(); i++)
		{
			Client* currentClient = this->clients[i];
			if (FD_ISSET(currentClient->getSocket()->getSocketFd(), &readFd))
			{
				cout << "Incoming from : " << currentClient->getUsername() << endl;
				activeClient = currentClient;
				break;
			}
		}
	} 
	
	//TODO: if select return a valid socket return the matching TCPSocket object otherwise return NULL
	return activeClient;
}

int MultipleClientSocketsListener::findBiggestFd(fd_set* readFd)
{
	int biggest = 0;
	
	for (unsigned int i = 0; i < this->clients.size(); i++)
	{
		int currentFd = this->clients[i]->getSocket()->getSocketFd();
		FD_SET(currentFd, readFd);

		if (currentFd > biggest)
		{
			biggest = currentFd;
		}
	}

	return biggest;
}

