#include "TCPMsnBroker.h"
#include "TCPMessengerServer.h"


TCPMsnBroker::TCPMsnBroker(TCPSocket* clientOne, TCPSocket* clientTwo, TCPMsnDispatcher* dispatcher)
{
	this->isActive = true;
	this->clientOne = clientOne;
	this->clientTwo = clientTwo;
	this->multiTCPListener.addSocket(clientOne);
	this->multiTCPListener.addSocket(clientTwo);
	this->dispatcher = dispatcher;
}

void TCPMsnBroker::run()
{
	while (this->isActive) 
	{
		TCPSocket* source = this->multiTCPListener.listenToSocket();
		if (source)
		{
			TCPSocket* target = source == this->clientOne ? this->clientTwo : this->clientOne;
			if (TCPMessengerServer::isSocketClosed(source))
			{
				exit(source, target);
				break;
			}
			
			int command = TCPMessengerServer::readCommandFromPeer(source);
			this->execute(command, source, target);	
		}
	}
}

void TCPMsnBroker::execute(int command, TCPSocket* source, TCPSocket* target)
{
	switch (command)
	{
		case OPEN_SESSION_WITH_PEER:
			this->dispatcher->openSession(source);
			break;
			
		case CLOSE_SESSION_WITH_PEER:
			this->closeSession();
			break;
			
		case SEND_MSG_TO_PEER:
			this->sendMessage(source, target);
			break;
			
		case EXIT:
			this->exit(source, target);
			break;
	}
}

void TCPMsnBroker::sendMessage(TCPSocket* source, TCPSocket* target)
{
	string message = TCPMessengerServer::readDataFromPeer(source);
	TCPMessengerServer::sendCommandToPeer(target, SEND_MSG_TO_PEER);
	TCPMessengerServer::sendDataToPeer(target, message);
}

void TCPMsnBroker::closeSession()
{
	this->isActive = false;
	TCPMessengerServer::sendCommandToPeer(this->clientOne, CLOSE_SESSION_WITH_PEER);
	TCPMessengerServer::sendCommandToPeer(this->clientTwo, CLOSE_SESSION_WITH_PEER);
	cout << "returning clients to dispatcher" << endl;
	this->dispatcher->addClient(this->clientOne);
	this->dispatcher->addClient(this->clientTwo);
	this->dispatcher->removeBroker(this->clientOne, this->clientTwo);
}

void TCPMsnBroker::exit(TCPSocket* source, TCPSocket* target)
{
	this->isActive = false;
	TCPMessengerServer::sendCommandToPeer(this->clientOne, CLOSE_SESSION_WITH_PEER);
	TCPMessengerServer::sendCommandToPeer(this->clientTwo, CLOSE_SESSION_WITH_PEER);
	this->dispatcher->removeBroker(this->clientOne, this->clientTwo);
	this->dispatcher->addClient(target);
	delete source;
}

TCPMsnBroker::~TCPMsnBroker()
{
	cout << "broker died" << endl;
}
