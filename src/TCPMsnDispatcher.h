#ifndef TCPMSNDISPATCHER_H_
#define TCPMSNDISPATCHER_H_

#include "TCPSocket.h"
#include "MThread.h"
#include "MultipleTCPSocketsListener.h"

class TCPMsnDispatcher : public MThread
{
public:
	MultipleTCPSocketsListener multiTCPListener;
	bool isActive;
	TCPMsnDispatcher();
	void run();
	void addClient(TCPSocket* client);
	virtual ~TCPMsnDispatcher();
};

#endif /*TCPMSNDISPATCHER_H_*/
