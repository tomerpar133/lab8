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
		this->closeRoom();
		break;
	default:
		this->tcpMsnDispatcher->execute(code, source);
	}
}

TCPMsnConferenceBroker::~TCPMsnConferenceBroker()
{
}
