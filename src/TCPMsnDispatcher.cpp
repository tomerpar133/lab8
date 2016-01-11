#include "TCPMsnDispatcher.h"

TCPMsnDispatcher::TCPMsnDispatcher()
{
	this->isActive = true;
}

void TCPMsnDispatcher::addClient(TCPSocket* client)
{
	multiTCPListener.addSocket(client);
}

void TCPMsnDispatcher::run()
{
	while (this->isActive)
	{
		TCPSocket* client = multiTCPListener.listenToSocket();
		
		if (client)
		{
			
		}
		
	}
}

TCPMsnDispatcher::~TCPMsnDispatcher()
{
}
