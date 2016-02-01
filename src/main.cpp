//============================================================================
// Name        : TCPMessengerServer
// Author      : Eliav Menachi
// Version     :
// Copyright   :
// Description :
//============================================================================

#include <iostream>
#include <map>
#include <stdlib.h>
#include "TCPMessengerServer.h"
using namespace std;
typedef void (*commandHandler)(); 
map<string, commandHandler> commandsMap;
TCPMessengerServer msngrServer;

void printInstructions(){
	cout<<"-----------------------"<<endl;
	cout<<"lu 	- list all users"<<endl;
	cout<<"lcu 	- list connected users"<<endl;
	cout<<"ls	- list sessions"<<endl;
	cout<<"lr 	- list all rooms"<<endl;
	cout<<"lru	- list all users in this room (e.g. lru <room name>)"<<endl;
	cout<<"x 	- shutdown server"<<endl;
	cout<<"-----------------------"<<endl;
}

void listConnectedUsers()
{
	msngrServer.listPeers();
}

void listUsers()
{
	msngrServer.listRegisteredUsers();
	// Print saved users in wadia
}

void listRooms()
{
	msngrServer.listRooms();
}

void listSessions()
{
	msngrServer.listSessions();
}

void listRoomUsers()
{
	string roomName;
	cin >> roomName;
	msngrServer.listUsersInRoom(roomName);
}

void exitHandler()
{
	msngrServer.close();
	cout<<"messenger was closed"<<endl;
	exit(0);
}

int main()
{
	cout<<"Welcome to TCP messenger Server"<<endl;
	printInstructions();
	
	commandsMap["lu"] = listUsers;
	commandsMap["lcu"] = listConnectedUsers;
	commandsMap["ls"] = listSessions;
	commandsMap["lr"] = listRooms;
	commandsMap["lru"] = listRoomUsers;
	commandsMap["x"] = exitHandler;
	
	while(true)
	{
		string msg;
		string command;
		cin >> command;
		
		if (commandsMap.find(command) != commandsMap.end())
		{
			commandsMap[command]();
		}
		else
		{
			cout<<"wrong input"<<endl;
			printInstructions();
		}
	}
	
	return 0;
}
