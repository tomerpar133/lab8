
#include "TCPMessengerServer.h"
#include "TCPMessengerProtocol.h"
#include <stdlib.h>

/***********************   TCPMessengerServer implementation ******************************/

TCPMessengerServer::TCPMessengerServer(){
	this->tcpMsnServer.start();
	//TODO: class initialization
}

void TCPMessengerServer::close(){
	//TODO: close the server
	this->tcpMsnServer.stop();
}

TCPMessengerServer::~TCPMessengerServer(){
}

int TCPMessengerServer::readCommandFromPeer(TCPSocket* peer){
	//TODO: read a command from socket
	int command = 0;
	peer->recv((char*)&command, sizeof(int));
	return ntohl(command);
}

string TCPMessengerServer::readDataFromPeer(TCPSocket* peer){
	//TODO: read a string from socket
	char msg[MAX_MSG_SIZE];
	int msgLength;
	peer->recv((char*)&msgLength, sizeof(int));
	peer->recv(msg, ntohl(msgLength));
	return msg;
}

void TCPMessengerServer::sendCommandToPeer(TCPSocket* peer, int command){
	//TODO: send command to socket
	command = htonl(command);
	peer->send((char*)&command, sizeof(int));
}

void TCPMessengerServer::sendDataToPeer(TCPSocket* peer, string msg){
	//TODO: send string to socket
	int msgLength = htonl(msg.length());
	peer->send((char*)&msgLength, sizeof(int));
	peer->send(msg.c_str(), msg.length());
}

bool TCPMessengerServer::isSocketClosed(TCPSocket* peer)
{
	int n = 0;
	ioctl(peer->getSocketFd(), FIONREAD, &n);
	
	return n == 0;

}

void TCPMessengerServer::listPeers(){
	//TODO: print the connected peers
	vector<string> peers = this->tcpMsnServer.getPeers();

	for (unsigned int i = 0; i < peers.size(); i++)
	{
		cout << "peer " << (i+1) << ") " << peers[i] << endl;
	}
}
