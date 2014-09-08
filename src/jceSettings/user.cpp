#include "user.h"


user::user(string username,string password) : hashedPassword(""),userID(""), influenceCourseOnly(false)
{

    this->username = username;
    this->password = password;
}
user::~user()
{
}


void user::setInfluenceCourseOnly(bool status)
{
    this->influenceCourseOnly = status;
}

bool user::getInfluenceCourseOnly() const
{
    return this->influenceCourseOnly;
}

void user::setUsername(string& username) {
    this->username=username;
}

void user::setPassword(string& password) {
    this->password=password;
}

void user::setUserID(string& ID)
{
    this->userID = ID;
}
void user::setHashedPassword(string& hashpass)
{
    this->hashedPassword = hashpass;
}

string user::getPassword() const { return password; }
string user::getUsername() const { return username; }
string user::getUserID() const { return userID; }
string user::getHashedPassword() const { return hashedPassword; }
