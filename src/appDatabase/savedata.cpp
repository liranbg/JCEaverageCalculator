#include "savedata.h"

/**
 * @brief This is A functions that will init the tags file.
 * if it exist - do nothing
 * if there is no file, it will create it and run the deleteData function
 * so tags will be set.
 */
SaveData::SaveData()
{
    QFile file(FILE_NAME);
    if(!file.exists())
    {
         file.open(QIODevice::ReadWrite | QIODevice::Text);
         file.close();
         createDB();
         save();
    }
    else
        load();
}

bool SaveData::isSaved()
{
    if(DB.value("username") == "")
        return false;
    return true;
}

void SaveData::reset()
{
    DB.clear();
    createDB();
    save();
}

void SaveData::setUsername(QString username)
{
    DB.insert("username", username);
    save();
}

void SaveData::setPassword(QString password)
{
    DB.insert("password", password);
    save();
}

void SaveData::setCal(QString cal)
{
    DB.insert("calendar", cal);
    save();
}

void SaveData::setLocal(QString local)
{
    DB.insert("local", local);
    save();
}

QString SaveData::getUsername()
{
    return DB.value("username");
}

QString SaveData::getPassword()
{
    return DB.value("password");
}

QString SaveData::getLocal()
{
    return DB.value("local");
}

QString SaveData::getCal()
{
    return DB.value("calendar");
}



void SaveData::load()
{
    QFile file(FILE_NAME);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in >> DB;
    file.close();
}

void SaveData::save()
{
    QFile file(FILE_NAME);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << DB;
    file.flush();
    file.close();
}

void SaveData::createDB()
{
    DB.insert("username", "");
    DB.insert("password", "");
    DB.insert("local", "default");
    DB.insert("calendar", "");
}
