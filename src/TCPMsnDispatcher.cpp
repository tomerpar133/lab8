#include "TCPMsnDispatcher.h"

TCPMsnDispatcher::TCPMsnDispatcher()
{
	this->isActive = true;
}

void TCPMsnDispatcher::addClient(TCPSocket* client)
{
	this->multiTCPListener.addSocket(client);
	this->clientsMap.insert(std::pair<string,TCPSocket*>(client->getClientAsString(), client));
	//this->clientsMap[client->getClientAsString()] = client;
}

void TCPMsnDispatcher::run()
{
	while (this->isActive)
	{
		TCPSocket* client = multiTCPListener.listenToSocket();
		
		if (client)
		{
			
		}
	}
}

vector<string> TCPMsnDispatcher::getClients()
{
	vector<string> clients;
	for(std::map<string,TCPSocket*>::iterator iter = clientsMap.begin(); iter != clientsMap.end(); ++iter)
	{
		clients.push_back(iter->first);
	}
	
	return clients;
}

TCPMsnDispatcher::~TCPMsnDispatcher()
{
}
