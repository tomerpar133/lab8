//============================================================================
// Name        : TCP Messenger Server
// Author      : Eliav Menachi
// Version     :
// Copyright   :
// Description : TCP Messenger application
//============================================================================

#ifndef TCPMESSENGERSERVER_H__
#define TCPMESSENGERSERVER_H__

#include <strings.h>
#include <map>
#include <vector>
#include <set>
#include <sys/ioctl.h>
#include "MThread.h"
#include "TCPSocket.h"
#include "MultipleTCPSocketsListener.h"
#include "TCPMessengerProtocol.h"
#include "TCPMsnServer.h"
//#include "TCPMsnDispatcher.h"

using namespace std;


class TCPMessengerServer{
	//TODO: add class properties
	TCPMsnServer tcpMsnServer;

public:
	/**
	 * Build the Messenger server
	 */
	TCPMessengerServer();
	~TCPMessengerServer();

	/**
	 * print out the list of connected clients (for debug)
	 */
	void listPeers();

	/**
	 * close the server
	 */
	void close();

	/**
	 * read command from peer
	 */
	static int readCommandFromPeer(TCPSocket* peer);

	/**
	 * read data from peer
	 */
	static string readDataFromPeer(TCPSocket* peer);

	/**
	 * send command to peer
	 */
	static void sendCommandToPeer(TCPSocket* peer, int command);

	/**
	 * send data to peer
	 */
	static void sendDataToPeer(TCPSocket* peer, string msg);

	/**
	 * check if socket is closed
	 */
	static bool isSocketClosed(TCPSocket* peer);

};
#endif
