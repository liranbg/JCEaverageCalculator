#ifndef user_H
#define user_H

#include <QString>

class user
{
public:

    user(QString username,QString password);
    ~user();
    void setUsername(QString& username);
    void setPassword(QString& password);

    QString getPassword() const;
    QString getUsername() const;

    void setUserID(QString& ID);
    void setHashedPassword(QString& hashpass);

    QString getUserID() const;
    QString getHashedPassword() const;

    void setInfluenceCourseOnly(bool status);
    bool getInfluenceCourseOnly() const;

private:

    QString username;
    QString password;

    QString hashedPassword;
    QString userID;

    bool influenceCourseOnly;

};
#endif


