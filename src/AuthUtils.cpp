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
	if (!this->isUserExist(username))
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
