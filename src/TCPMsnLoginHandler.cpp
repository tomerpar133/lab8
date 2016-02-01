#include "TCPMsnLoginHandler.h"
#include "TCPMessengerServer.h"

TCPMsnLoginHandler::TCPMsnLoginHandler(TCPMsnDispatcher* tcpMsnDispatcher)
{
	this->tcpMsnDispatcher = tcpMsnDispatcher;
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

void TCPMsnLoginHandler::registerClient(Client* client)
{
	string username = TCPMessengerServer::readDataFromPeer(client->getSocket());
	string password = TCPMessengerServer::readDataFromPeer(client->getSocket());
	// Add them to the file of usernames and passwords? or to temp area to add to file in the end?
	client->setUsername(username);
	this->tcpMsnDispatcher->addClient(client);
}

void TCPMsnLoginHandler::clientLogin(Client* client)
{
	string username = TCPMessengerServer::readDataFromPeer(client->getSocket());
	string password = TCPMessengerServer::readDataFromPeer(client->getSocket());
	// if (password is correct)
	client->setUsername(username);
	this->tcpMsnDispatcher->addClient(client);
}

void TCPMsnLoginHandler::execute(int code, Client* client)
{
	switch (code)
	{
	case LOGIN:
		this->clientLogin(client);
		break;
	case REGISTER:
		this->registerClient(client);
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

vector<string> TCPMsnLoginHandler::getRegisteredList()
{
	vector<string> users;
	users.push_back("Tomer");
	users.push_back("Avihay");
	return users;
}

TCPMsnLoginHandler::~TCPMsnLoginHandler()
{
	
}
