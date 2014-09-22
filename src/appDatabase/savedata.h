#ifndef SAVEDATA_H
#define SAVEDATA_H

/**
 *	SaveData Class 
 * --------------------------------------
 * 
 * Uses QMap
 *
 * key :  vale
 *
 * Keys:
 * "username"
 * "password"
 * "local"
 * "calendar"
 *
 * Note that the password will be encrypted using the SimpleCrypt class!
 *
 * SimpleCrypt class was found at the qt projects forms - and been fixed by Sagi Dayan.
 *
 */

/* QT libs */
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QMap>
#include "simplecrypt.h"

#define FILE_NAME   "JM.dat"

class SaveData
{
public:
    SaveData();
    bool isSaved();
    void reset();
    void setUsername(QString username);
    void setPassword(QString password);
    void setCal(QString cal);
    void setLocal(QString local);
    QString getUsername();
    QString getPassword();
    QString getCal();
    QString getLocal();

private:
    QMap<QString, QString> DB;
    void load();
    void save();
    void createDB();
    QString encrypt(QString pass);
    QString decrypte(QString pass);

};

#endif // SAVEDATA_H

