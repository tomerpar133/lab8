
#include "TCPMessengerServer.h"
#include "TCPMessengerProtocol.h"
#include "AuthUtils.h"

/***********************   TCPMessengerServer implementation ******************************/

TCPMessengerServer::TCPMessengerServer(){
	this->tcpMsnServer.start();
}

void TCPMessengerServer::close(){
	this->tcpMsnServer.stop();
}

TCPMessengerServer::~TCPMessengerServer(){
}

int TCPMessengerServer::readCommandFromPeer(TCPSocket* peer){
	// read a command from socket
	int command = 0;
	peer->recv((char*)&command, sizeof(int));
	cout << "Got command " << ntohl(command) << endl; 
	return ntohl(command);
}

string TCPMessengerServer::readDataFromPeer(TCPSocket* peer){
	// read a string from socket
	char msg[MAX_MSG_SIZE];
	int msgLength;
	peer->recv((char*)&msgLength, sizeof(int));
	peer->recv(msg, ntohl(msgLength));
	return msg;
}

void TCPMessengerServer::sendCommandToPeer(TCPSocket* peer, int command){
	// send command to socket
	command = htonl(command);
	peer->send((char*)&command, sizeof(int));
}

void TCPMessengerServer::sendDataToPeer(TCPSocket* peer, string msg){
	// send string to socket
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
	return AuthUtils::getUsers();
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
	ostringstream packedVector;
	unsigned int i = 0;
	
	if (vect.size() == 0) return " ";
	
	for (; i < vect.size() - 1; i++)
	{
		packedVector << (i+1) << ") " << vect[i] << endl;
	}
	
	if (i < vect.size())
		packedVector << (i+1) << ") " << vect[i];
	
	return packedVector.str();
}
