#include "TCPMsnServer.h"


TCPMsnServer::TCPMsnServer()
{
	this->isActive = true;
}

void TCPMsnServer::run()
{
	this->tcpMsnDispatcher.start();
	listenForever();
}

void TCPMsnServer::listenForever()
{
	while (this->isActive) 
	{
		TCPSocket* tcpSocket = new TCPSocket(MSNGR_PORT);
		tcpSocket->listenAndAccept();
		//Send the socket to the Dispatcher
		this->tcpMsnDispatcher.addClient(tcpSocket);
	}
}

TCPMsnServer::~TCPMsnServer()
{
}
