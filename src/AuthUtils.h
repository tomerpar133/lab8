#ifndef AUTHUTILS_H_
#define AUTHUTILS_H_

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

static const char* USERS_FILE_PATH = "shadow";

class AuthUtils
{
public:
	AuthUtils();
	static bool addUser(string, string);
	static bool isUserExist(string);
	static bool authenticate(string, string);
	static vector<string> getUsers();
	virtual ~AuthUtils();
};

#endif /*AUTHUTILS_H_*/
