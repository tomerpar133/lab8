#include "TCPMsnServer.h"


TCPMsnServer::TCPMsnServer()
{
	this->isActive = true;
	this->tcpMsnLoginHandler = new TCPMsnLoginHandler(&this->tcpMsnDispatcher);
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
		TCPSocket* newGuestSocket = tcpSocket.listenAndAccept();
		cout << "client connected: " << newGuestSocket->getClientAsString() << endl; 
		this->tcpMsnLoginHandler->addGuest(newGuestSocket);
	}
}

vector<string> TCPMsnServer::getClients()
{
	return this->tcpMsnDispatcher.getClients();
}

vector<string> TCPMsnServer::getRegisteredUsers()
{
	return this->tcpMsnLoginHandler->getRegisteredList();
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
}
