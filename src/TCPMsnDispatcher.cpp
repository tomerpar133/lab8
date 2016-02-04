#include "TCPMsnDispatcher.h"
#include "TCPMessengerServer.h"

TCPMsnDispatcher::TCPMsnDispatcher()
{
	this->isActive = true;
}

void TCPMsnDispatcher::addClient(Client* client)
{
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
					delete client;
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
		case OPEN_ROOM:
			this->openRoom(source);
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
					TCPMessengerServer::vectorToString(this->getRooms()));
			break;
		case LIST_ROOM_USERS:
			roomName = TCPMessengerServer::readDataFromPeer(source->getSocket());
			TCPMessengerServer::sendDataToPeer(source->getSocket(), 
					TCPMessengerServer::vectorToString(this->getUsersInRoom(roomName)));
			break;
		case EXIT:
			this->removeClient(source->getUsername());
			delete source;
			break;
	}
}

void TCPMsnDispatcher::openSession(Client* sourceClient)
{
	// Search for existing session and destroy if exist
	if (this->brokersMap.find(sourceClient) != this->brokersMap.end())
	{
		brokersMap[sourceClient]->closeSession(sourceClient);                                                 
	}
	
	string targetStr = TCPMessengerServer::readDataFromPeer(sourceClient->getSocket());
	Client* targetClient = this->clientsMap[targetStr];
	if (targetClient)
	{
		int targetReply;
		TCPMessengerServer::sendCommandToPeer(targetClient->getSocket(), OPEN_SESSION_WITH_PEER);
		TCPMessengerServer::sendDataToPeer(targetClient->getSocket(), sourceClient->getUsername());
		targetReply = TCPMessengerServer::readCommandFromPeer(targetClient->getSocket());
		if (targetReply == SESSION_ESTABLISHED)
		{
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
	else 
	{
		TCPMessengerServer::sendCommandToPeer(sourceClient->getSocket(), SESSION_REFUSED);
	}
}

void TCPMsnDispatcher::openRoom(Client* source)
{
	string roomName = TCPMessengerServer::readDataFromPeer(source->getSocket());
	TCPMessengerServer::sendDataToPeer(source->getSocket(), SUCCESS);
	this->removeClient(source->getUsername());
	if (this->conferencesMap.find(roomName) == this->conferencesMap.end())
	{
		cout << "Create new room - " << roomName << endl;
		this->conferencesMap[roomName] = new TCPMsnConferenceBroker(roomName, source, this);
		this->conferencesMap[roomName]->start();
	}
	else 
	{
		cout << "Add user to existing room - " << roomName << endl;
		this->conferencesMap[roomName]->clientEnterRoom(source);
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
	
	for(std::map<string, TCPMsnConferenceBroker*>::iterator iter = this->conferencesMap.begin(); iter != this->conferencesMap.end(); ++iter)
	{
		vector<string> roomUsers = iter->second->getUsers();
		clients.insert(clients.begin(), roomUsers.begin(), roomUsers.end());
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
			sessionsList.push_back(iter->second->getSessionName());
		}
	}
	
	return sessionsList;
}

vector<string> TCPMsnDispatcher::getRooms()
{
	vector<string> roomsList;

	if (this->conferencesMap.size() == 0) return vector<string>();
	
	for(std::map<string,TCPMsnConferenceBroker*>::iterator iter = this->conferencesMap.begin(); iter != this->conferencesMap.end(); ++iter)
	{
		roomsList.push_back(iter->first);
	}
	
	return roomsList;
}

vector<string> TCPMsnDispatcher::getUsersInRoom(string roomName)
{
	if (this->conferencesMap.find(roomName) != this->conferencesMap.end())
		return this->conferencesMap[roomName]->getUsers();
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
	this->clientsMap.erase(this->clientsMap.find(client));
}

void TCPMsnDispatcher::removeBroker(Client* clientOne, Client* clientTwo)
{
	TCPMsnBroker* broker = this->brokersMap.find(clientOne)->second;
	this->brokersMap.erase(this->brokersMap.find(clientOne));
	this->brokersMap.erase(this->brokersMap.find(clientTwo));
	delete broker;
}

void TCPMsnDispatcher::removeConferenceBroker(string roomName)
{
	std::map<string,TCPMsnConferenceBroker*>::iterator iter = this->conferencesMap.find(roomName);
	TCPMsnConferenceBroker* conferenceBroker = iter->second;
	this->conferencesMap.erase(iter);
	conferenceBroker->stop();
	cout << "Room was deleted <" << roomName << endl;
	cout << "Rooms count " << this->conferencesMap.size() << endl;
}

void TCPMsnDispatcher::stop()
{
	this->isActive = false;
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
			this->brokersMap[client]->closeSession(client);                                                 
		}
		
		client->getSocket()->cclose();
		this->clientsMap.erase(iter);
		delete client;
	}
	
	cout << "dispatcher died" << endl;
}
