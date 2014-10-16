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

/**
 * @brief This will check if the is data saved ie. username and password.
 * the only way in JCE manager is to store a username and password together. therefore if there is
 * a user name in the file, so there is a password ass well.
 * @return true if the is a username in the file. (if not the username will be "")
 */
bool SaveData::isSaved()
{
    if(DB.value("username") == "")
        return false;
    return true;
}

/**
 * @brief This void function will erase all data. QMap will be cleard and saved to the file.
 * data thet will be deleted:
 *   # username
 *   # password
 *   # calendar
 *   # locale
 */
void SaveData::reset()
{
    DB.clear();
    createDB();
    save();
}

/**
 * @brief gest a username and saves it into the QMap.
 * QMap then is saved to the file.
 * @param username - users username from the mainwindow ui pointer.
 */
void SaveData::setUsername(QString username)
{
    DB.insert("username", username);
    save();
}

/**
 * @brief gets a password, sends it to encyption, and saves the encrypter password to QMap.
 * saves the QMap into the file.
 * @param password - raw password (not encrypted)
 */
void SaveData::setPassword(QString password)
{
    DB.insert("password", encrypt(password));
    save();
}

/**
 * @brief gest a calendar and saves it into the QMap.
 * QMap then is saved to the file.
 * @param cal - last calendar that user seen
 */
void SaveData::setCal(QString cal)
{
    DB.insert("calendar", cal);
    save();
}

/**
 * @brief gest a locale and saves it into the QMap.
 * QMap then is saved to the file.
 * @param locale - QString (he, en)
 */
void SaveData::setLocale(QString local)
{
    DB.insert("locale", local);
    save();
}

/**
 * @brief read from file
 * @return - username
 */
QString SaveData::getUsername()
{
    return DB.value("username");
}

/**
 * @brief read from file
 * @return - unencrypted password
 */
QString SaveData::getPassword()
{
    return decrypte(DB.value("password"));
}

/**
 * @brief read from file
 * @return locale
 */
QString SaveData::getLocale()
{
    return DB.value("locale");
}

/**
 * @brief read from file
 * @return calendar
 */
QString SaveData::getCal()
{
    return DB.value("calendar");
}

/**
 * @brief read from file the QMap object into the DB inside "this" object.
 */
void SaveData::load()
{
    QFile file(FILE_NAME);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);
    in >> DB;
    file.close();
}

/**
 * @brief Saves the QMap to the file. overriding the content of the file.
 */
void SaveData::save()
{
    QFile file(FILE_NAME);
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);
    out << DB;
    file.flush();
    file.close();
}

/**
 * @brief Creates en empty tamplate for QMap (use only on init! if there is no file.)
 */
void SaveData::createDB()
{
    DB.insert("username", "");
    DB.insert("password", "");
    DB.insert("locale", "en");
    DB.insert("calendar", "");
}

/**
 * @brief this function will connect this class to the simplecrypt class.
 * sends the raw password to encryption and returns the encrypted one.
 * @param  pass - raw password.
 * @return      encrypted password.
 */
QString SaveData::encrypt(QString pass)
{
    SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f027));
    return crypto.encryptToString(pass);
}

/**
 * @brief this function will connect this class to the simplecrypt class.
 * sends the encrypted password to decryption and returns the raw one.
 * @param  pass - encrypted password.
 * @return      raw password.
 */
QString SaveData::decrypte(QString pass)
{
    SimpleCrypt crypto(Q_UINT64_C(0x0c2ad4a4acb9f027));
    return crypto.decryptToString(pass);
}
