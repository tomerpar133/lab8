#include "TCPMsnBroker.h"
#include "TCPMessengerServer.h"


TCPMsnBroker::TCPMsnBroker(Client* clientOne, Client* clientTwo, TCPMsnDispatcher* dispatcher)
{
	this->isActive = true;
	this->clientOne = clientOne;
	this->clientTwo = clientTwo;
	this->multiTCPListener.addClient(clientOne);
	this->multiTCPListener.addClient(clientTwo);
	this->dispatcher = dispatcher;
}

void TCPMsnBroker::run()
{
	while (this->isActive) 
	{
		Client* source = this->multiTCPListener.listenToClients();
		if (source)
		{
			Client* target = source == this->clientOne ? this->clientTwo : this->clientOne;
			if (TCPMessengerServer::isSocketClosed(source->getSocket()))
			{
				exit(source, target);
				break;
			}
			
			int command = TCPMessengerServer::readCommandFromPeer(source->getSocket());
			this->execute(command, source, target);	
		}
	}
}

void TCPMsnBroker::execute(int command, Client* source, Client* target)
{
	switch (command)
	{
		case OPEN_SESSION_WITH_PEER:
			this->dispatcher->openSession(source);
			break;
			
		case CLOSE_SESSION_WITH_PEER:
			this->closeSession(source);
			break;
			
		case SEND_MSG_TO_PEER:
			this->sendMessage(source, target);
			break;
			
		case EXIT:
			this->exit(source, target);
			break;
	}
}

void TCPMsnBroker::sendMessage(Client* source, Client* target)
{
	string message = TCPMessengerServer::readDataFromPeer(source->getSocket());
	TCPMessengerServer::sendCommandToPeer(target->getSocket(), SEND_MSG_TO_PEER);
	TCPMessengerServer::sendDataToPeer(target->getSocket(), message);
}

void TCPMsnBroker::closeSession(Client* source)
{
	Client* target = source == this->clientOne ? this->clientTwo : this->clientOne;
	this->isActive = false;
	TCPMessengerServer::sendCommandToPeer(target->getSocket(), CLOSE_SESSION_WITH_PEER);
	TCPMessengerServer::readCommandFromPeer(target->getSocket());
	this->dispatcher->addClient(this->clientOne);
	this->dispatcher->addClient(this->clientTwo);
	this->dispatcher->removeBroker(this->clientOne, this->clientTwo);
}

void TCPMsnBroker::exit(Client* source, Client* target)
{
	this->isActive = false;
	TCPMessengerServer::sendCommandToPeer(this->clientOne->getSocket(), CLOSE_SESSION_WITH_PEER);
	TCPMessengerServer::sendCommandToPeer(this->clientTwo->getSocket(), CLOSE_SESSION_WITH_PEER);
	this->dispatcher->addClient(target);
	delete source;
	this->dispatcher->removeBroker(this->clientOne, this->clientTwo);
}

string TCPMsnBroker::getSessionName()
{
	return "{ " + this->clientOne->getUsername() + " & " + this->clientTwo->getUsername() + " }";
}

TCPMsnBroker::~TCPMsnBroker()
{
	cout << "broker died @-^--" << endl;
}
