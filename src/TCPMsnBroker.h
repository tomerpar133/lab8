#ifndef TCPMSNBROKER_H_
#define TCPMSNBROKER_H_

#include "MThread.h"
#include "TCPSocket.h"
#include "MultipleTCPSocketsListener.h"
#include "TCPMsnDispatcher.h"
#include "TCPMessengerProtocol.h"
class TCPMsnDispatcher;
class TCPMsnBroker : public MThread
{
	TCPSocket* clientOne;
	TCPSocket* clientTwo;
	MultipleTCPSocketsListener multiTCPListener;
	TCPMsnDispatcher* dispatcher;
	bool isActive;
public:
	TCPMsnBroker(TCPSocket* clientOne, TCPSocket* clientTwo, TCPMsnDispatcher* dispatcher);
	void run();
	void execute(int command, TCPSocket* source, TCPSocket* target);
	void sendMessage(TCPSocket* source, TCPSocket* target);
	void closeSession();
	void exit(TCPSocket* source, TCPSocket* target);
	virtual ~TCPMsnBroker();
};

#endif /*TCPMSNBROKER_H_*/
