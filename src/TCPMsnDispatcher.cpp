#include "TCPMsnDispatcher.h"

TCPMsnDispatcher::TCPMsnDispatcher()
{
}

void TCPMsnDispatcher::addClient(TCPSocket* client)
{
	multiTCPListener.addSocket(client);
}

void TCPMsnDispatcher::run()
{
	
}

TCPMsnDispatcher::~TCPMsnDispatcher()
{
}
