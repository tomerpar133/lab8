#ifndef AUTHUTILS_H_
#define AUTHUTILS_H_

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

static const char* USERS_FILE_PATH = "shadow";

class AuthUtils
{
public:
	AuthUtils();
	bool addUser(string, string);
	bool isUserExist(string);
	bool authenticate(string, string);
	virtual ~AuthUtils();
};

#endif /*AUTHUTILS_H_*/
