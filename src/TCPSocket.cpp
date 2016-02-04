/*
 * TCPSocket.cpp
 *
 *  Created on: Feb 6, 2013
 *      Author: Eliav Menachi
 */

#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include "TCPSocket.h"
using namespace std;

TCPSocket::TCPSocket(int connected_sock,struct sockaddr_in serverAddr,struct sockaddr_in peerAddr)
{
	socketFd = connected_sock;
	myAddress = serverAddr;
	peerAddress = peerAddr;
}


TCPSocket::TCPSocket(int port)
{
	socketFd = socket(AF_INET, SOCK_STREAM, 0);

	if (socketFd < 0)
	{
		perror("Error creating socket myAddress");
	}

	// set the server address for binding
	bzero(&myAddress, sizeof(myAddress));
	myAddress.sin_family = AF_INET;
	myAddress.sin_port = htons(port);

	if (bind(socketFd, (struct sockaddr*)&myAddress, sizeof(myAddress)) < 0)
	{
		perror("Error binding myAddress");
	}
}


TCPSocket::TCPSocket(string peerIp, int port)
{
	socketFd = socket(AF_INET, SOCK_STREAM, 0);

	if (socketFd < 0)
	{
		perror("Error creating socket peer");
		close(socketFd);
	}

	// set the peer address to connect to
	bzero(&peerAddress, sizeof(peerAddress));
	peerAddress.sin_family = AF_INET;
	peerAddress.sin_port = htons(port);
	peerAddress.sin_addr.s_addr = inet_addr(peerIp.data());

	// connect the socket to the peer server
	if (connect(socketFd, (struct sockaddr*)&peerAddress, sizeof(peerAddress)) < 0)
	{
		perror("Error binding peer");
		close(socketFd);
	}
}


TCPSocket* TCPSocket::listenAndAccept()
{
	// listen on the socket
	listen(socketFd, 1);

	// call accept on the socket
	socklen_t size = sizeof(peerAddress);
	int acceptFd = accept(socketFd, (struct sockaddr*)&peerAddress,&size);

	// return new TCPSocket object holding the new secondary server socket returned from the accept call

	return new TCPSocket(acceptFd, myAddress, peerAddress);
}


int TCPSocket::recv(char* buffer, int length)
{
	return read(socketFd, buffer, length);
}


int TCPSocket::send(const char* msg, int len)
{
	return write(socketFd, msg, len);
}


void TCPSocket::cclose()
{
	// socket cleanup
	shutdown(socketFd, SHUT_RDWR);
	close(socketFd);
}

string TCPSocket::getDestIP()
{
	return this->fromAddr();
}

string TCPSocket::fromAddr()
{
	return inet_ntoa(peerAddress.sin_addr);
}
