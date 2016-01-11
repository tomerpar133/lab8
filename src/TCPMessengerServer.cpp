
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
	return -1;
}

string TCPMessengerServer::readDataFromPeer(TCPSocket* peer){
	//TODO: read a string from socket
	return "";
}

void TCPMessengerServer::sendCommandToPeer(TCPSocket* peer, int command){
	//TODO: send command to socket
}

void TCPMessengerServer::sendDataToPeer(TCPSocket* peer, string msg){
	//TODO: send string to socket
}

void TCPMessengerServer::listPeers(){
	//TODO: print the connected peers
	vector<string> peers = this->tcpMsnServer.getPeers();

	for (unsigned int i = 0; i < peers.size(); i++)
	{
		cout << "peer " << (i+1) << ") " << peers[i] << endl;
	}
}
