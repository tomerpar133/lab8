#include "Client.h"

Client::Client(TCPSocket* socket, string username)
{
	this->socket = socket;
	this->username = username;
}

string Client::getUsername()
{
	return this->username;
}

void Client::setUsername(string username)
{
	this->username = username;
}

TCPSocket* Client::getSocket()
{
	return this->socket;
}

Client::~Client()
{
}
