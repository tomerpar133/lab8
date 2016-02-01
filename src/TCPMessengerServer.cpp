
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
	cout << "Got command " << ntohl(command) << endl; 
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
	int msgLength = htonl(msg.length() + 1);
	peer->send((char*)&msgLength, sizeof(int));
	peer->send(msg.c_str(), msg.length() + 1);
	cout << "Send to client (" << ntohl(msgLength) << ") msg: " << msg.c_str() << endl;
}

bool TCPMessengerServer::isSocketClosed(TCPSocket* peer)
{
	int n = 0;
	ioctl(peer->getSocketFd(), FIONREAD, &n);
	
	return n == 0;
}

vector<string> TCPMessengerServer::getRegisteredUsers()
{
	vector<string> users;
	users.push_back("Tomer");
	users.push_back("Avihay");

	return users;
}

void TCPMessengerServer::listRegisteredUsers()
{
	printStringVector(TCPMessengerServer::getRegisteredUsers(), "All registered users");
}

void TCPMessengerServer::listPeers()
{
	printStringVector(this->tcpMsnServer.getClients(), "Connected users");
}

void TCPMessengerServer::listSessions()
{
	printStringVector(this->tcpMsnServer.getAllSessions(), "Sessions");
}

void TCPMessengerServer::listRooms()
{
	printStringVector(this->tcpMsnServer.getAllRooms(), "Rooms");
}

void TCPMessengerServer::listUsersInRoom(string roomName)
{
	printStringVector(this->tcpMsnServer.getUsersInRoom(roomName), "Users in room : " + roomName);
}

// Template method which prints a vector :)
void TCPMessengerServer::printStringVector(vector<string> vect, string description)
{
	cout << "------------------------------" << endl;
	cout << description << endl;
	cout << "------------------------------" << endl;
	for (unsigned int i = 0; i < vect.size(); i++)
	{
		cout << (i+1) << ") " << vect[i] << endl;
	}
	cout << "------------------------------" << endl;
}

string TCPMessengerServer::vectorToString(vector<string> vect)
{
	string packedVector = "";
	
	for (unsigned int i = 0; i < vect.size(); i++)
	{
		packedVector += vect[i] + ';';
	}
	
	return packedVector;
}
