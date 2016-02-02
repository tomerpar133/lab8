#ifndef TCPMSNLOGINHANDLER_H_
#define TCPMSNLOGINHANDLER_H_

#include <vector>
#include <algorithm>
#include "Client.h"
#include "TCPMessengerProtocol.h"
#include "MultipleClientSocketsListener.h"
#include "TCPMsnDispatcher.h"
#include "TCPSocket.h"
#include "AuthUtils.h"
#include "MThread.h"

using namespace std;

class TCPMsnLoginHandler : public MThread
{
	bool isActive;
	vector<Client*> guestsPool;
	TCPMsnDispatcher* tcpMsnDispatcher;
	AuthUtils authUtils;
public:
	TCPMsnLoginHandler(TCPMsnDispatcher* tcpMsnDispatcher);
	void addGuest(TCPSocket* socket);
	void registerClient(Client* client);
	void clientLogin(Client* client);
	void invalideOpcode(Client* client);
	void run();
	void execute(int code, Client* client);
	virtual ~TCPMsnLoginHandler();
};

#endif /*TCPMSNLOGINHANDLER_H_*/
