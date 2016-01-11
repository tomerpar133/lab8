#ifndef TCPMSNSERVER_H_
#define TCPMSNSERVER_H_

#include <string>
#include "TCPSocket.h"
#include "MThread.h"
#include "TCPMessengerProtocol.h"
#include "TCPMsnDispatcher.h"

using namespace std;

class TCPMsnServer : public MThread
{
public:
	bool isActive;
	TCPMsnDispatcher tcpMsnDispatcher;
	TCPMsnServer();
	void run();
	void stop();
	// Accept new connection and  
	void listenForever();
	string* getPeers();
	virtual ~TCPMsnServer();
};

#endif /*TCPMSNSERVER_H_*/
