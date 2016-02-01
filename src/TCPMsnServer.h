#ifndef TCPMSNSERVER_H_
#define TCPMSNSERVER_H_

#include <string>
#include <vector>
#include "TCPSocket.h"
#include "MThread.h"
#include "TCPMessengerProtocol.h"
#include "TCPMsnDispatcher.h"
#include "TCPMsnLoginHandler.h"

using namespace std;
//class TCPMsnLoginHandler;
class TCPMsnServer : public MThread
{
public:
	bool isActive;
	TCPMsnDispatcher tcpMsnDispatcher;
	TCPMsnLoginHandler* tcpMsnLoginHandler;
	TCPMsnServer();
	void run();
	void stop();
	// Accept new connection and  
	void listenForever();
	vector<string> getClients();
	vector<string> getRegisteredUsers();
	vector<string> getAllSessions();
	vector<string> getAllRooms();
	vector<string> getUsersInRoom(string);
	//vector<string> getClients();
	virtual ~TCPMsnServer();
};

#endif /*TCPMSNSERVER_H_*/
