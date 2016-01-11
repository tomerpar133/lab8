
#include "TCPMessengerServer.h"
#include "TCPMessengerProtocol.h"

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
	while (peer->recv(((char*)&command), sizeof(int)) < 1);
	return htons(command);
}

string TCPMessengerServer::readDataFromPeer(TCPSocket* peer){
	//TODO: read a string from socket
	char msg[MAX_MSG_SIZE];
	int msgLen = ntohl(MAX_MSG_SIZE);
	peer->recv(msg, msgLen);
	return msg;
}

void TCPMessengerServer::sendCommandToPeer(TCPSocket* peer, int command){
	//TODO: send command to socket
	peer->send((char*)&command, sizeof(int));
}

void TCPMessengerServer::sendDataToPeer(TCPSocket* peer, string msg){
	//TODO: send string to socket
	peer->send(msg.c_str(), msg.length());
}

void TCPMessengerServer::listPeers(){
	//TODO: print the connected peers
	vector<string> peers = this->tcpMsnServer.getPeers();

	for (unsigned int i = 0; i < peers.size(); i++)
	{
		cout << "peer " << (i+1) << ") " << peers[i] << endl;
	}
}
