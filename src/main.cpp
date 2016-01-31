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
	cout<<"lp - list peers"<<endl;
	cout<<"x - shutdown server"<<endl;
	cout<<"-----------------------"<<endl;
}

void listPeers()
{
	msngrServer.listPeers();
}

void exitHandler()
{
	msngrServer.close();
	cout<<"messenger was closed"<<endl;
	exit(0);
}

int main(){
	cout<<"Welcome to TCP messenger Server"<<endl;
	printInstructions();
	
	commandsMap["lp"] = listPeers;
	commandsMap["x"] = exitHandler;
	
	while(true){
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
