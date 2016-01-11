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

void TCPMsnServer::stop()
{
	this->isActive = false;
}

void TCPMsnServer::listenForever()
{
	cout << "Server is up and listening" << endl;
	TCPSocket tcpSocket(MSNGR_PORT);
	while (this->isActive) 
	{
		//Send the socket to the Dispatcher
		this->tcpMsnDispatcher.addClient(tcpSocket.listenAndAccept());
	}
}

vector<string> TCPMsnServer::getPeers()
{
	return this->tcpMsnDispatcher.getClients();
}

TCPMsnServer::~TCPMsnServer()
{
}
