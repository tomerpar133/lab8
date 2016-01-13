#include "TCPMsnDispatcher.h"
#include "TCPMessengerServer.h"
TCPMsnDispatcher::TCPMsnDispatcher()
{
	this->isActive = true;
}

void TCPMsnDispatcher::addClient(TCPSocket* client)
{
	//this->multiTCPListener.addSocket(client);
	string clientAsString = client->getClientAsString();
	this->clientsMap.insert(std::pair<string,TCPSocket*>(clientAsString, client));
	this->clientsMap[client->getClientAsString()] = client;
}

void TCPMsnDispatcher::run()
{
	while (this->isActive)
	{
		if (!this->clientsMap.empty())
		{
			MultipleTCPSocketsListener multiTCPListener;
			multiTCPListener.addSockets(this->getClientsSockets());
			TCPSocket* client = multiTCPListener.listenToSocket();
			if (client)
			{
				if (TCPMessengerServer::isSocketClosed(client))
				{
					this->removeClient(client->getClientAsString());
				}
				else
				{
					int code = TCPMessengerServer::readCommandFromPeer(client);
					this->execute(code, client);
				}
			}
		}
	}
}

void TCPMsnDispatcher::execute(int code, TCPSocket* source)
{
	switch (code)
	{
		case OPEN_SESSION_WITH_PEER:
			this->openSession(source);
			break;
		case EXIT:
			break;
	}
}

void TCPMsnDispatcher::openSession(TCPSocket* sourceClient)
{
	// Search for existing session and destroy if exist
	if (brokersMap[sourceClient])
	{
		brokersMap[sourceClient]->closeSession();                                                 
	}
	
	string targetStr = TCPMessengerServer::readDataFromPeer(sourceClient);
	TCPSocket* targetClient = this->clientsMap[targetStr];
	if (targetClient)
	{
		TCPMessengerServer::sendCommandToPeer(targetClient, OPEN_SESSION_WITH_PEER);
		TCPMessengerServer::sendDataToPeer(targetClient, sourceClient->getClientAsString());
		TCPMessengerServer::sendCommandToPeer(sourceClient, SESSION_ESTABLISHED);
		// Create new broker with sourceClient and targetClient
		TCPMsnBroker* broker = new TCPMsnBroker(sourceClient, targetClient, this);
		this->brokersMap[sourceClient] = broker;
		this->brokersMap[targetClient] = broker;
		this->removeClient(sourceClient->getClientAsString());
		this->removeClient(targetClient->getClientAsString());
		cout << "Executing broker with peers : {" << sourceClient->getClientAsString() << " & " << targetClient->getClientAsString() << "}" << endl;
		broker->start();
	} 
	else 
	{
		TCPMessengerServer::sendCommandToPeer(sourceClient, SESSION_REFUSED);
	}
}

vector<string> TCPMsnDispatcher::getClients()
{
	vector<string> clients;
	for(std::map<string,TCPSocket*>::iterator iter = this->clientsMap.begin(); iter != this->clientsMap.end(); ++iter)
	{
		clients.push_back(iter->first);
	}
	
	return clients;
}

vector<TCPSocket*> TCPMsnDispatcher::getClientsSockets()
{
	vector<TCPSocket*> clients;
	for(std::map<string,TCPSocket*>::iterator iter = this->clientsMap.begin(); iter != this->clientsMap.end(); ++iter)
	{
		clients.push_back(iter->second);
	}
	
	return clients;
}

void TCPMsnDispatcher::removeClient(string client)
{
	this->clientsMap.erase(client);
}

void TCPMsnDispatcher::removeBroker(TCPSocket* clientOne, TCPSocket* clientTwo)
{
	TCPMsnBroker* broker = this->brokersMap[clientOne];
	this->brokersMap.erase(clientOne);
	this->brokersMap.erase(clientTwo);
	delete broker;
}

TCPMsnDispatcher::~TCPMsnDispatcher()
{
	while (this->clientsMap.size() > 0)
	{
		std::map<string,TCPSocket*>::iterator iter = this->clientsMap.begin();
		TCPSocket* client = (this->clientsMap[iter->first]);
		client->cclose();
		this->clientsMap.erase(iter);
		delete client;
	}
	
	cout << "dispatcher died" << endl;
}
