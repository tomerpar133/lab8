#include "TCPMsnServer.h"


TCPMsnServer::TCPMsnServer()
{
	this->isActive = true;
}

void TCPMsnServer::run()
{
	//this->tcpMsnDispatcher.start();
	listenForever();
}

void TCPMsnServer::stop()
{
	this->isActive = false;
}

void TCPMsnServer::listenForever()
{
	cout << "Server is up and listening" << endl;
	while (this->isActive) 
	{
		TCPSocket* tcpSocket = new TCPSocket(MSNGR_PORT);
		tcpSocket->listenAndAccept();
		//Send the socket to the Dispatcher
		this->tcpMsnDispatcher.addClient(tcpSocket);
	}
}

vector<string> TCPMsnServer::getPeers()
{
	return vector<string>(); //this->tcpMsnDispatcher.getClients();
}

TCPMsnServer::~TCPMsnServer()
{
}
