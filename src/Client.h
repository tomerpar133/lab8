#ifndef CLIENT_H_
#define CLIENT_H_
#include "TCPSocket.h"
#include <string>

class Client
{
	TCPSocket* socket;
	string username;
public:
	string getUsername();
	void setUsername(string username);
	TCPSocket* getSocket();
	Client(TCPSocket* socket, string username);
	virtual ~Client();
};

#endif /*CLIENT_H_*/
