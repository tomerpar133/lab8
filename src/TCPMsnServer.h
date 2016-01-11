#ifndef TCPMSNSERVER_H_
#define TCPMSNSERVER_H_

#include "TCPSocket.h"
#include "MThread.h"
#include "TCPMessengerProtocol.h"
#include "TCPMsnDispatcher.h"

class TCPMsnServer : public MThread
{
public:
	bool isActive;
	TCPMsnDispatcher tcpMsnDispatcher;
	TCPMsnServer();
	void run();
	// Accept new connection and  
	void listenForever();
	virtual ~TCPMsnServer();
};

#endif /*TCPMSNSERVER_H_*/
