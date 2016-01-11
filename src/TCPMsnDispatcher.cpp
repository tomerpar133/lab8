#include "TCPMsnDispatcher.h"
#include "TCPMessengerServer.h"

TCPMsnDispatcher::TCPMsnDispatcher()
{
	this->isActive = true;
}

void TCPMsnDispatcher::addClient(TCPSocket* client)
{
	cout << "Called add client" << endl;
	this->multiTCPListener.addSocket(client);
	cout << "Added to multi listen" << endl;
	string clientAsString = client->getClientAsString();
	cout << "Client became string : " << clientAsString << endl;
	this->clientsMap.insert(std::pair<string,TCPSocket*>(clientAsString, client));
	cout << "Added to map" << endl;
	this->clientsMap[client->getClientAsString()] = client;
	cout << "END client" << endl;
}

void TCPMsnDispatcher::run()
{
	while (this->isActive)
	{
		if (!this->clientsMap.empty())
		{
			TCPSocket* client = this->multiTCPListener.listenToSocket();
			if (client)
			{
				int code = TCPMessengerServer::readCommandFromPeer(client);
				cout << "Got command " << code << " from " << client->getClientAsString() << endl;
				this->execute(code, client);
			}
		}
	}
}

void TCPMsnDispatcher::execute(int code, TCPSocket* source)
{
	switch (code)
	{
		case OPEN_SESSION_WITH_PEER:
			this->openSession(source);
			break;
		case EXIT:
			break;

	}
}

void TCPMsnDispatcher::openSession(TCPSocket* sourceClient)
{
	// Search for existing session and destroy if exist
	string targetStr = TCPMessengerServer::readDataFromPeer(sourceClient);
	TCPSocket* targetClient = this->clientsMap[targetStr];
	if (targetClient)
	{
		// Create new broker with sourceClient and targetClient
		cout << "Executing broker with peers : {" << sourceClient->getClientAsString() << " & " << targetClient->getClientAsString() << "}" << endl;
	} else {
		TCPMessengerServer::sendCommandToPeer(sourceClient, SESSION_REFUSED);
	}
}

vector<string> TCPMsnDispatcher::getClients()
{
	vector<string> clients;
	for(std::map<string,TCPSocket*>::iterator iter = this->clientsMap.begin(); iter != this->clientsMap.end(); ++iter)
	{
		clients.push_back(iter->first);
	}
	
	return clients;
}

TCPMsnDispatcher::~TCPMsnDispatcher()
{
	while (this->clientsMap.size() > 0)
	{
		std::map<string,TCPSocket*>::iterator iter = this->clientsMap.begin();
		TCPSocket* client = (this->clientsMap[iter->first]);
		client->cclose();
		this->clientsMap.erase(iter);
		delete client;
	}
}
