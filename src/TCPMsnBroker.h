#ifndef TCPMSNBROKER_H_
#define TCPMSNBROKER_H_

#include "MThread.h"
#include "TCPSocket.h"
#include "MultipleClientSocketsListener.h"
#include "TCPMsnDispatcher.h"
#include "TCPMessengerProtocol.h"
class TCPMsnDispatcher;
class TCPMsnBroker : public MThread
{
	Client* clientOne;
	Client* clientTwo;
	MultipleClientSocketsListener multiTCPListener;
	TCPMsnDispatcher* dispatcher;
	bool isActive;
public:
	TCPMsnBroker(Client* clientOne, Client* clientTwo, TCPMsnDispatcher* dispatcher);
	void run();
	void execute(int command, Client* source, Client* target);
	void sendMessage(Client* source, Client* target);
	void closeSession(Client* source);
	void exit(Client* source, Client* target);
	string getSessionName();
	virtual ~TCPMsnBroker();
};

#endif /*TCPMSNBROKER_H_*/
