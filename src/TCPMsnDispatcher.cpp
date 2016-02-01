#include "TCPMsnDispatcher.h"
#include "TCPMessengerServer.h"
TCPMsnDispatcher::TCPMsnDispatcher()
{
	this->isActive = true;
}

void TCPMsnDispatcher::addClient(Client* client)
{
	//this->multiTCPListener.addSocket(client);
	this->clientsMap[client->getUsername()] = client;
}

void TCPMsnDispatcher::run()
{
	while (this->isActive)
	{
		if (!this->clientsMap.empty())
		{
			MultipleClientSocketsListener multiClientListener;
			multiClientListener.addClients(this->getClientsSockets());
			Client* client = multiClientListener.listenToClients();
			if (client)
			{
				if (TCPMessengerServer::isSocketClosed(client->getSocket()))
				{
					this->removeClient(client->getUsername());
				}
				else
				{
					int code = TCPMessengerServer::readCommandFromPeer(client->getSocket());
					this->execute(code, client);
				}
			}
		}
	}
}

void TCPMsnDispatcher::execute(int code, Client* source)
{
	string roomName;
	
	switch (code)
	{
		case OPEN_SESSION_WITH_PEER:
			this->openSession(source);
			break;
		case LIST_USERS:
			TCPMessengerServer::sendDataToPeer(source->getSocket(), 
					TCPMessengerServer::vectorToString(TCPMessengerServer::getRegisteredUsers()));
			break;
		case LIST_CONNECTED_USERS:
			TCPMessengerServer::sendDataToPeer(source->getSocket(), 
					TCPMessengerServer::vectorToString(this->getClients()));
			break;
		case LIST_ROOMS:
			TCPMessengerServer::sendDataToPeer(source->getSocket(), 
					TCPMessengerServer::vectorToString(this->getClients()));
			break;
		case LIST_ROOM_USERS:
			roomName = TCPMessengerServer::readDataFromPeer(source->getSocket());
			TCPMessengerServer::sendDataToPeer(source->getSocket(), 
					TCPMessengerServer::vectorToString(this->getUsersInRoom(roomName)));
			break;
		case EXIT:
			break;
	}
}

void TCPMsnDispatcher::openSession(Client* sourceClient)
{
	// Search for existing session and destroy if exist
	if (brokersMap[sourceClient])
	{
		brokersMap[sourceClient]->closeSession();                                                 
	}
	
	string targetStr = TCPMessengerServer::readDataFromPeer(sourceClient->getSocket());
	Client* targetClient = this->clientsMap[targetStr];
	if (targetClient)
	{
		TCPMessengerServer::sendCommandToPeer(targetClient->getSocket(), OPEN_SESSION_WITH_PEER);
		TCPMessengerServer::sendDataToPeer(targetClient->getSocket(), sourceClient->getUsername());
		TCPMessengerServer::sendCommandToPeer(sourceClient->getSocket(), SESSION_ESTABLISHED);
		// Create new broker with sourceClient and targetClient
		TCPMsnBroker* broker = new TCPMsnBroker(sourceClient, targetClient, this);
		this->brokersMap[sourceClient] = broker;
		this->brokersMap[targetClient] = broker;
		this->removeClient(sourceClient->getUsername());
		this->removeClient(targetClient->getUsername());
		cout << "Executing broker with peers : {" << sourceClient->getUsername() << " & " << targetClient->getUsername() << "}" << endl;
		broker->start();
	} 
	else 
	{
		TCPMessengerServer::sendCommandToPeer(sourceClient->getSocket(), SESSION_REFUSED);
	}
}

vector<string> TCPMsnDispatcher::getClients()
{
	vector<string> clients;
	for(std::map<string,Client*>::iterator iter = this->clientsMap.begin(); iter != this->clientsMap.end(); ++iter)
	{
		clients.push_back(iter->first);
	}
	
	for(std::map<Client*, TCPMsnBroker*>::iterator iter = this->brokersMap.begin(); iter != this->brokersMap.end(); ++iter)
	{
		clients.push_back(iter->first->getUsername());
	}
	
	return clients;
}

vector<string> TCPMsnDispatcher::getSessions()
{
	vector<string> sessionsList;
	vector<TCPMsnBroker*> distinctBrokersList;
	
	for(std::map<Client*,TCPMsnBroker*>::iterator iter = this->brokersMap.begin(); iter != this->brokersMap.end(); ++iter)
	{
		if (std::find(distinctBrokersList.begin(), distinctBrokersList.end(), iter->second) == distinctBrokersList.end())
		{
			distinctBrokersList.push_back(iter->second);
		}
	}
	
	for (unsigned int i = 0; i < distinctBrokersList.size(); i++)
	{
		// TODO: Session to string
		//distinctBrokersList[i]
	}
	
	return vector<string>();
}

vector<string> TCPMsnDispatcher::getRooms()
{
	return vector<string>();
}

vector<string> TCPMsnDispatcher::getUsersInRoom(string roomName)
{
	return vector<string>();
}

vector<Client*> TCPMsnDispatcher::getClientsSockets()
{
	vector<Client*> clients;
	for(std::map<string,Client*>::iterator iter = this->clientsMap.begin(); iter != this->clientsMap.end(); ++iter)
	{
		clients.push_back(iter->second);
	}
	
	return clients;
}

void TCPMsnDispatcher::removeClient(string client)
{
	this->clientsMap.erase(client);
}

void TCPMsnDispatcher::removeBroker(Client* clientOne, Client* clientTwo)
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
		std::map<string,Client*>::iterator iter = this->clientsMap.begin();
		Client* client = (this->clientsMap[iter->first]);
		
		// Search for existing session and destroy if exist
		if (this->brokersMap[client])
		{
			this->brokersMap[client]->closeSession();                                                 
		}
		
		client->getSocket()->cclose();
		this->clientsMap.erase(iter);
		delete client;
	}
	
	cout << "dispatcher died" << endl;
}
