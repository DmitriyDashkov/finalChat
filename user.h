#pragma once
#include <string>
#include "sha1.h"
using namespace std;

class User
{
    static int userCounter;
    int _id;
    string _login;
    string _username;
    Hash _passwordHash;

    bool m_isBun;
public:
    User();
    User(string username, Hash passHash);
    int checklogin(const string& login, const Hash& passHash) const;
    string& getuserName();
    int getUserID() const;

    void setBun(bool b)     { m_isBun = b; }
    bool isBun()            { return m_isBun; }
};
