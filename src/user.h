#ifndef user_H
#define user_H

#include <string>
#include <iostream>
#include "jce/jcedate.h"

using namespace std;
class user
{
public:
    user(string username,string password);
    ~user();
    void setUsername(string& username);
    void setPassword(string& password);

    string getPassword() const;
    string getUsername() const;

    void setUserID(string& ID);
    void setHashedPassword(string& hashpass);

    string getUserID() const;
    string getHashedPassword() const;

    void setDate(std::string fromYear, std::string fromSemester, std::string toYear, std::string toSemester);

    void setInfluenceCourseOnly(bool status);
    bool getInfluenceCourseOnly() const;

    jceDate * const date = new jceDate("","","",""); //we do not change the pointer

private:

    string username;
    string password;

    string hashedPassword;
    string userID;

    bool influenceCourseOnly;



};
#endif


