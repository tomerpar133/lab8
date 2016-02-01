#ifndef TCPMSNDISPATCHER_H_
#define TCPMSNDISPATCHER_H_


#include <map>
#include <vector>
#include <string>
#include "TCPSocket.h"
#include "MThread.h"
#include "MultipleClientSocketsListener.h"
#include "TCPMessengerProtocol.h"
#include "TCPMsnBroker.h"
#include "Client.h"

using namespace std;
class TCPMsnBroker;
class TCPMsnDispatcher : public MThread
{
public:
	//MultipleTCPSocketsListener multiTCPListener;
	map<string, Client*> clientsMap;
	map<Client*, TCPMsnBroker*> brokersMap;
	bool isActive;
	TCPMsnDispatcher();
	void run();
	void addClient(Client* client);
	void removeClient(string client);
	void removeBroker(Client* clientOne, Client* clientTwo);
	vector<string> getClients();
	vector<string> getSessions();
	vector<string> getRooms();
	vector<string> getUsersInRoom(string);
	// Command Handlers
	void execute(int code, Client* source);
	void openSession(Client* source);
	vector<Client*> getClientsSockets();
	virtual ~TCPMsnDispatcher();
};

#endif /*TCPMSNDISPATCHER_H_*/
