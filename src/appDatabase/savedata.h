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
#include <QDebug>
#include <QMap>

#define FILE_NAME   "JAC_DB.dat"

class SaveData
{
public:
    void static init();
    bool static isSaved();
    bool static reset();
    QString static setUsername(QString username);
    QString static setPassword(QString password);
    QString static setCal(QString cal);
    QString static setLocal(QString local);
    QString static getUsername();
    QString static getPassword();
    QString static getCal();
    QString static getLocal();

private:
    QMap<QString, QString> DB;
    void static load();
    void static save();
    void static createDB();
};

#endif // SAVEDATA_H

