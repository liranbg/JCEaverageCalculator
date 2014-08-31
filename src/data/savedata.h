#ifndef SAVEDATA_H
#define SAVEDATA_H

/**
 *	SaveData Class 
 * --------------------------------------
 * 
 * all functions in this class are static!
 * no need to create an object!
 * 
 * provides all the needed functions
 * for saving user data for JCE login.
 *
 * the class will enteract with a file formated by tags for storing the valus.
 * the file format is as sutch:
 * 
 * 		[status]Status Value[/]
 * 		[username]usernaem vale[/]
 * 		[password]hashed(not really) password vale[/]
 *
 * the class will create a file if not exists by calling Save Data::init() function.
 *
 * for more info about functions - see implantation
 */

/* QT libs */
#include <QString>
#include <QFile>
#include <QTextStream>

/* C/C++ libs */
#include <string.h>
#include <stdlib.h>
#include <iostream>

#define FILE_NAME   "JAC_DB.dat"
#define DEFAULT_DATA_EMPTY "[status]false[/]\n[username][/]\n[password][/]"

class SaveData
{
public:
    bool static isSaved();
    bool static save(QString username, QString password);
    bool static saveCal(QString cal);
    bool static saveLocal(QString local);
    bool static deleteData();
    QString static getUsername();
    QString static getPassword();
    QString static gatCal();
    QString static getLocal();
    void static init();
private:
    QString static getValViaTag(QString tag, QFile *file);
    QString static hashPassword(QString pass);
    QString static deHashPasword(QString pass);
    bool static isTagLine(QString line, QString tag);
};

#endif // SAVEDATA_H
