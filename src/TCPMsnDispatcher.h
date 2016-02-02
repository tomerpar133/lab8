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
#include "TCPMsnConferenceBroker.h"

using namespace std;

class TCPMsnConferenceBroker;
class TCPMsnBroker;
class TCPMsnDispatcher : public MThread
{
	map<string, Client*> clientsMap;
	map<Client*, TCPMsnBroker*> brokersMap;
	bool isActive;
	map<string, TCPMsnConferenceBroker*> conferencesMap;
	
public:
	
	TCPMsnDispatcher();
	void run();
	void addClient(Client* client);
	void removeClient(string client);
	void removeBroker(Client* clientOne, Client* clientTwo);
	void removeConferenceBroker(string roomName);
	vector<string> getClients();
	vector<string> getSessions();
	vector<string> getRooms();
	vector<string> getUsersInRoom(string);
	// Command Handlers
	void execute(int code, Client* source);
	void openSession(Client* source);
	void openRoom(Client*);
	vector<Client*> getClientsSockets();
	virtual ~TCPMsnDispatcher();
};

#endif /*TCPMSNDISPATCHER_H_*/
