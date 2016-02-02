#include "TCPMsnConferenceBroker.h"
#include "TCPMessengerServer.h"

TCPMsnConferenceBroker::TCPMsnConferenceBroker(string roomName, Client* roomOwner, TCPMsnDispatcher* tcpMsnDispatcher)
{
	this->isActive = true;
	this->roomName = roomName;
	this->roomOwner = roomOwner;
	this->tcpMsnDispatcher = tcpMsnDispatcher;
}

void TCPMsnConferenceBroker::clientEnterRoom(Client* client)
{
	// TODO send the new user existing users in the chat room
	this->clients.push_back(client);
	// TODO notify the room owner of the new client
}

void TCPMsnConferenceBroker::clientLeaveRoom(Client* client)
{
	vector<Client*>::iterator clientToRemove = std::find(this->clients.begin(), this->clients.end(), client);
	if (clientToRemove != this->clients.end())
		this->clients.erase(clientToRemove);
	this->tcpMsnDispatcher->addClient(client);
	// TODO notify the room owner of the leaving client
}

void TCPMsnConferenceBroker::run() 
{
	while (this->isActive)
	{
		MultipleClientSocketsListener multiClientListener;
		multiClientListener.addClients(this->clients);
		Client* source = multiClientListener.listenToClients();
		
		if (TCPMessengerServer::isSocketClosed(source->getSocket()))
		{
			if (source == roomOwner)
				this->closeRoom();
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

void TCPMsnConferenceBroker::closeRoom()
{
	for (unsigned int i = 0; i < this->clients.size(); i++)
	{
		this->tcpMsnDispatcher->addClient(this->clients[i]);
	}
	
	this->tcpMsnDispatcher->removeConferenceBroker(this->roomName);
}

void TCPMsnConferenceBroker::execute(int code, Client* source)
{
	switch (code)
	{
	case CLOSE_ROOM:
		if (source == this->roomOwner)
		{
			this->closeRoom();
			TCPMessengerServer::sendDataToPeer(source->getSocket(), SUCCESS);
		}
		else 
		{
			TCPMessengerServer::sendDataToPeer(source->getSocket(), FAILURE);
		}
		break;
	default:
		this->tcpMsnDispatcher->execute(code, source);
	}
}

vector<string> TCPMsnConferenceBroker::getUsers()
{
	vector<string> users;
	
	for (unsigned int i = 0; i < this->clients.size(); i++)
	{
		users.push_back(this->clients[i]->getUsername());
	}
	
	return users;
}

TCPMsnConferenceBroker::~TCPMsnConferenceBroker()
{
}
