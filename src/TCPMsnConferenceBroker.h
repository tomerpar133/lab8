#ifndef TCPMSNCONFERENCEBROKER_H_
#define TCPMSNCONFERENCEBROKER_H_

#include <map>
#include <vector>
#include "MThread.h"
#include "Client.h"
#include "MultipleClientSocketsListener.h"
#include "TCPMsnDispatcher.h"

using namespace std;

class TCPMsnDispatcher;
class TCPMsnConferenceBroker : public MThread
{
	bool isActive;
	TCPMsnDispatcher* tcpMsnDispatcher;
	string roomName;
	Client* roomOwner;
	vector<Client*> clients;
	
public:
	TCPMsnConferenceBroker(string, Client*, TCPMsnDispatcher*);
	void clientEnterRoom(Client*);
	void clientLeaveRoom(Client*);
	vector<string> getUsers();
	void run();
	void closeRoom();
	void execute(int code, Client* source);
	virtual ~TCPMsnConferenceBroker();
};

#endif /*TCPMSNCONFERENCEBROKER_H_*/
