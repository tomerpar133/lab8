#include "AuthUtils.h"

AuthUtils::AuthUtils()
{
	fstream usersFile;
	usersFile.open(USERS_FILE_PATH, fstream::app);
	if (usersFile.is_open())
		usersFile.close();
}

bool AuthUtils::addUser(string username, string password)
{
	if (!AuthUtils::isUserExist(username))
	{
		ofstream usersFile;
		usersFile.open(USERS_FILE_PATH, fstream::app);
		
		if (usersFile.is_open())
		{
			usersFile << username << endl;
			usersFile << password << endl;
		}
		
		usersFile.close();
		return true;
	}
	
	return false;
}

bool AuthUtils::isUserExist(string username)
{
	ifstream usersFile;
	string currUser;
	usersFile.open(USERS_FILE_PATH);
	
	if (usersFile.is_open())
	{
		while(getline(usersFile, currUser))
		{
			if (currUser == username)
				return true;
			
			// skip the password line
			if (!getline(usersFile, currUser))
				break;
		}
	}
	
	usersFile.close();
	
	return false;
}

vector<string> AuthUtils::getUsers()
{
	vector<string> users;
	ifstream usersFile;
	string currUser;
	usersFile.open(USERS_FILE_PATH);
	
	if (usersFile.is_open())
	{
		while(getline(usersFile, currUser))
		{
			users.push_back(currUser);			
			if (!getline(usersFile, currUser))
				break;
		}
	}
	
	usersFile.close();
	
	return users;
}

bool AuthUtils::authenticate(string username, string password)
{
	ifstream usersFile;
	string currUser;
	string currPassword;
	usersFile.open(USERS_FILE_PATH);
	
	if (usersFile.is_open())
	{
		while(getline(usersFile, currUser))
		{
			if (currUser == username)
			{
				if (getline(usersFile, currPassword) && currPassword == password)
					return true;
			}
			else 
				getline(usersFile, currPassword);
		}
	}
	
	usersFile.close();
	
	return false;
}

AuthUtils::~AuthUtils()
{
}
