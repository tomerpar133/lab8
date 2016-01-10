#include "MultipleTCPSocketsListener.h"
#include "TCPSocket.h"

void MultipleTCPSocketsListener::addSocket(TCPSocket* socket )
{
	if (socket == NULL)
	{
		return;
	}

	sockets.push_back(socket);
}

void MultipleTCPSocketsListener::addSockets(vector<TCPSocket*> socketVec)
{
	for (int i = 0; i < socketVec.size(); i++)
	{
		addSocket(socketVec[i]);
	}
}
TCPSocket* MultipleTCPSocketsListener::listenToSocket(int timeout)
{
	TCPSocket* sock;

	// this sucks.. assume it works.. ill do it tomorrow

	return sock;
}
