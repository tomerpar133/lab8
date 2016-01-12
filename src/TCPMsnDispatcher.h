#ifndef TCPMSNDISPATCHER_H_
#define TCPMSNDISPATCHER_H_


#include <map>
#include <vector>
#include "TCPSocket.h"
#include "MThread.h"
#include "MultipleTCPSocketsListener.h"
#include "TCPMessengerProtocol.h"
#include "TCPMsnBroker.h"

using namespace std;
class TCPMsnBroker;
class TCPMsnDispatcher : public MThread
{
public:
	MultipleTCPSocketsListener multiTCPListener;
	map<string, TCPSocket*> clientsMap;
	map<TCPSocket*, TCPMsnBroker*> brokersMap;
	bool isActive;
	TCPMsnDispatcher();
	void run();
	void addClient(TCPSocket* client);
	void removeClient(string client);
	void removeBroker(TCPSocket* clientOne, TCPSocket* clientTwo);
	vector<string> getClients();
	// Command Handlers
	void execute(int code, TCPSocket* source);
	void openSession(TCPSocket* source);
	virtual ~TCPMsnDispatcher();
};

#endif /*TCPMSNDISPATCHER_H_*/
