#include "TCPMsnConferenceBroker.h"
#include "TCPMessengerServer.h"

TCPMsnConferenceBroker::TCPMsnConferenceBroker(string roomName, Client* roomOwner, TCPMsnDispatcher* tcpMsnDispatcher)
{
	this->isActive = true;
	this->roomName = roomName;
	this->roomOwner = roomOwner;
	this->tcpMsnDispatcher = tcpMsnDispatcher;
	this->clientEnterRoom(roomOwner);
}

void TCPMsnConferenceBroker::clientEnterRoom(Client* client)
{
	cout << "New client in room " << this->roomName << " - " << client->getUsername() << endl;
	
	// Update the present users
	for (unsigned int i = 0; i < this->clients.size(); i++)
	{
		TCPMessengerServer::sendCommandToPeer(client->getSocket(), USER_ENTER_ROOM);
		TCPMessengerServer::sendDataToPeer(client->getSocket(), this->clients[i]->getUsername());
		TCPMessengerServer::sendDataToPeer(client->getSocket(), this->clients[i]->getSocket()->getDestIP());
		
		TCPMessengerServer::sendCommandToPeer(this->clients[i]->getSocket(), USER_ENTER_ROOM);
		TCPMessengerServer::sendDataToPeer(this->clients[i]->getSocket(), client->getUsername());
		TCPMessengerServer::sendDataToPeer(this->clients[i]->getSocket(), client->getSocket()->getDestIP());
	}
	
	this->clients.push_back(client);
}

void TCPMsnConferenceBroker::clientLeaveRoom(Client* client)
{
	cout << "Client left room " << this->roomName << " - " << client->getUsername() << endl;
	
	vector<Client*>::iterator clientToRemove = std::find(this->clients.begin(), this->clients.end(), client);
	if (clientToRemove != this->clients.end())
		this->clients.erase(clientToRemove);
	this->tcpMsnDispatcher->addClient(client);
	
	// Notify the room owner of the leaving client
	for (unsigned int i = 0; i < this->clients.size(); i++)
	{
		TCPMessengerServer::sendCommandToPeer(this->clients[i]->getSocket(), USER_LEAVE_ROOM);
		TCPMessengerServer::sendDataToPeer(this->clients[i]->getSocket(), client->getSocket()->getDestIP());
	}
}

void TCPMsnConferenceBroker::run() 
{
	while (this->isActive)
	{
		usleep(300000);
		MultipleClientSocketsListener multiClientListener;
		multiClientListener.addClients(this->clients);
		Client* source = multiClientListener.listenToClients();
		cout << "Conference listening for " << this->clients.size() << " client(s)." << endl;
		if (source)
		{
			if (TCPMessengerServer::isSocketClosed(source->getSocket()))
			{
				if (source->getUsername() == roomOwner->getUsername())
					this->closeRoom(roomOwner);
				else
				{
					vector<Client*>::iterator clientToRemove = std::find(this->clients.begin(), this->clients.end(), source);
					if (clientToRemove != this->clients.end())
						this->clients.erase(clientToRemove);
					delete(source);
				}
			}
			else
			{
				int command = TCPMessengerServer::readCommandFromPeer(source->getSocket());
				this->execute(command, source);
			}
		}
	}
}

void TCPMsnConferenceBroker::closeRoom(Client* source)
{
	for (unsigned int i = 0; i < this->clients.size(); i++)
	{
		if (this->clients[i] != source)
			TCPMessengerServer::sendCommandToPeer(this->clients[i]->getSocket(), CLOSE_ROOM);
		this->tcpMsnDispatcher->addClient(this->clients[i]);
	}
	
	this->tcpMsnDispatcher->removeConferenceBroker(this->roomName);
}

void TCPMsnConferenceBroker::stop()
{
	this->isActive = false;
}

void TCPMsnConferenceBroker::execute(int code, Client* source)
{
	switch (code)
	{
	case CLOSE_ROOM:
		cout << "[Conference broker] Close room received from " << source->getUsername() << endl;
		if (source->getUsername() == this->roomOwner->getUsername())
		{
			this->closeRoom(this->roomOwner);
		}
		else 
		{
			this->clientLeaveRoom(source);
		}
		break;
	default:
		this->tcpMsnDispatcher->execute(code, source);
	}
}

vector<string> TCPMsnConferenceBroker::getUsers()
{
	vector<string> users;
	
	if (this->clients.size() == 0)
		return vector<string>();
	
	for (unsigned int i = 0; i < this->clients.size(); i++)
	{
		cout << "User found in room " << this->roomName << " : ";
		cout << clients[i]->getUsername() << endl; 
		users.push_back(this->clients[i]->getUsername());
	}
	
	return users;
}

TCPMsnConferenceBroker::~TCPMsnConferenceBroker()
{
}
