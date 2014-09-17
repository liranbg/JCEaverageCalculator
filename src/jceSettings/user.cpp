#include "user.h"

/**
 * @brief user::user Constructor
 * @param username user name
 * @param password password
 */
user::user(QString username,QString password) : hashedPassword(""),userID(""), influenceCourseOnly(false)
{
    this->username = username;
    this->password = password;
}

user::~user()
{
}

/**
 * @brief user::setInfluenceCourseOnly
 * @param status show only courses with influnce?
 */
void user::setInfluenceCourseOnly(bool status)
{
    this->influenceCourseOnly = status;
}
/**
 * @brief user::getInfluenceCourseOnly
 * @return if user wants to show only influence courses
 */
bool user::getInfluenceCourseOnly() const
{
    return this->influenceCourseOnly;
}
/**
 * @brief user::setUsername
 * @param username given username to change to
 */
void user::setUsername(QString& username)
{
    this->username = username;
}
/**
 * @brief user::setPassword
 * @param password given password to change to
 */
void user::setPassword(QString& password)
{
    this->password = password;
}
/**
 * @brief user::setUserID
 * @param ID given ID from html
 */
void user::setUserID(QString& ID)
{
    this->userID = ID;
}
/**
 * @brief user::setHashedPassword
 * @param hashpass given hashed password given from html
 */
void user::setHashedPassword(QString& hashpass)
{
    this->hashedPassword = hashpass;
}
/**
 * @brief user::get*
 * @return getters
 */
QString user::getPassword() const { return password; }
QString user::getUsername() const { return username; }
QString user::getUserID() const { return userID; }
QString user::getHashedPassword() const { return hashedPassword; }
