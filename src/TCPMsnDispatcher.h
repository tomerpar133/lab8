#ifndef TCPMSNDISPATCHER_H_
#define TCPMSNDISPATCHER_H_

#include <string>
#include <map>
#include <vector>
#include "TCPSocket.h"
#include "MThread.h"
#include "MultipleTCPSocketsListener.h"

class TCPMsnDispatcher : public MThread
{
public:
	MultipleTCPSocketsListener multiTCPListener;
	map<string, TCPSocket*> clientsMap;
	bool isActive;
	TCPMsnDispatcher();
	void run();
	void addClient(TCPSocket* client);
	vector<string> getClients();
	virtual ~TCPMsnDispatcher();
};

#endif /*TCPMSNDISPATCHER_H_*/
