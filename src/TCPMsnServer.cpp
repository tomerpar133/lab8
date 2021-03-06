#include "TCPMsnServer.h"


TCPMsnServer::TCPMsnServer()
{
	this->isActive = true;
	this->tcpMsnLoginHandler = new TCPMsnLoginHandler(&this->tcpMsnDispatcher); 
}

void TCPMsnServer::run()
{
	this->tcpMsnDispatcher.start();
	this->tcpMsnLoginHandler->start();
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
		TCPSocket* newGuestSocket = tcpSocket.listenAndAccept();
		cout << "client connected: " << newGuestSocket->getDestIP() << endl; 
		this->tcpMsnLoginHandler->addGuest(newGuestSocket);
	}
}

vector<string> TCPMsnServer::getClients()
{
	return this->tcpMsnDispatcher.getClients();
}

vector<string> TCPMsnServer::getAllSessions()
{
	return this->tcpMsnDispatcher.getSessions();
}

vector<string> TCPMsnServer::getAllRooms()
{
	return this->tcpMsnDispatcher.getRooms();
}

vector<string> TCPMsnServer::getUsersInRoom(string roomName)
{
	return this->tcpMsnDispatcher.getUsersInRoom(roomName);
}

TCPMsnServer::~TCPMsnServer()
{
	this->tcpMsnDispatcher.stop();
	this->tcpMsnLoginHandler->stop();
	this->tcpMsnDispatcher.waitForThread();
	cout << "Dispatcher was closed" << endl;
	this->tcpMsnLoginHandler->waitForThread();
	delete this->tcpMsnLoginHandler;
}
