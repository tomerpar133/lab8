#include "TCPMsnLoginHandler.h"
#include "TCPMessengerServer.h"

TCPMsnLoginHandler::TCPMsnLoginHandler(TCPMsnDispatcher* tcpMsnDispatcher)
{
	this->tcpMsnDispatcher = tcpMsnDispatcher;
	this->isActive = true;
}

void TCPMsnLoginHandler::addGuest(TCPSocket* socket)
{
	Client* newClient = new Client(socket, GUEST_NAME);
	this->guestsPool.push_back(newClient);
}

void TCPMsnLoginHandler::run()
{
	while(isActive)
	{
		MultipleClientSocketsListener multiClientListener;
		if (!this->guestsPool.empty())
		{
			multiClientListener.addClients(this->guestsPool);
			Client* client = multiClientListener.listenToClients();
			if (client)
			{
				if (TCPMessengerServer::isSocketClosed(client->getSocket()))
				{
					vector<Client*>::iterator clientToRemove = std::find(this->guestsPool.begin(), this->guestsPool.end(), client);
					if (clientToRemove != this->guestsPool.end())
						this->guestsPool.erase(clientToRemove);
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

void TCPMsnLoginHandler::registerClient(Client* client)
{
	string username = TCPMessengerServer::readDataFromPeer(client->getSocket());
	string password = TCPMessengerServer::readDataFromPeer(client->getSocket());
	
	if (!authUtils.addUser(username, password))
		TCPMessengerServer::sendDataToPeer(client->getSocket(), FAILURE);
	else
	{
		client->setUsername(username);
		this->tcpMsnDispatcher->addClient(client);
		TCPMessengerServer::sendDataToPeer(client->getSocket(), SUCCESS);
	}
}

void TCPMsnLoginHandler::clientLogin(Client* client)
{
	string username = TCPMessengerServer::readDataFromPeer(client->getSocket());
	string password = TCPMessengerServer::readDataFromPeer(client->getSocket());
	
	if (!this->authUtils.authenticate(username, password))
		TCPMessengerServer::sendDataToPeer(client->getSocket(), FAILURE);
	else
	{
		client->setUsername(username);
		this->tcpMsnDispatcher->addClient(client);
		TCPMessengerServer::sendDataToPeer(client->getSocket(), SUCCESS);
	}
}

void TCPMsnLoginHandler::execute(int code, Client* client)
{
	string roomName;
	
	switch (code)
	{
	case LOGIN:
		this->clientLogin(client);
		break;
	case REGISTER:
		this->registerClient(client);
		break;
	case LIST_USERS:
		TCPMessengerServer::sendDataToPeer(client->getSocket(), 
				TCPMessengerServer::vectorToString(TCPMessengerServer::getRegisteredUsers()));
		break;
	case LIST_CONNECTED_USERS:
		TCPMessengerServer::sendDataToPeer(client->getSocket(), 
				TCPMessengerServer::vectorToString(this->tcpMsnDispatcher->getClients()));
		break;
	case LIST_ROOMS:
		TCPMessengerServer::sendDataToPeer(client->getSocket(), 
				TCPMessengerServer::vectorToString(this->tcpMsnDispatcher->getRooms()));
		break;
	case LIST_ROOM_USERS:
		roomName = TCPMessengerServer::readDataFromPeer(client->getSocket());
		TCPMessengerServer::sendDataToPeer(client->getSocket(), 
				TCPMessengerServer::vectorToString(this->tcpMsnDispatcher->getUsersInRoom(roomName)));
		break;
	default:
		this->invalideOpcode(client);
		break;
	}
}

void TCPMsnLoginHandler::invalideOpcode(Client* client)
{
	TCPMessengerServer::sendCommandToPeer(client->getSocket(), SEND_MSG_TO_PEER);
	TCPMessengerServer::sendDataToPeer(client->getSocket(), INVALID_OPERATION);
}

TCPMsnLoginHandler::~TCPMsnLoginHandler()
{
	
}
