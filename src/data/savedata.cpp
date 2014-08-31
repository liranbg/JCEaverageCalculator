#include "savedata.h"



/**
 * @brief Checks the status tag in the file
 * @return boolean the status
 */
bool SaveData::isSaved()
{
    QString status;
    QFile* file = new QFile(FILE_NAME);
    /* Opening file for read */
    if(file->open(QIODevice::ReadOnly|QIODevice::Text))
    {
        status = getValViaTag("status", file);
        if(status == "true")
        {
            file->close(); //close & return
            return true;
        }
        return false; //close & return
        file->close();
    }
    /* If Faild to open */
    std::cout << "Faild To Accsess file: " << FILE_NAME << std::endl;
    return false;
}


/**
 * @brief This Function will modify the file, and save the data.
 * @param username - QString
 * @param password - QString
 * @return true if saved - false if error
 */
bool SaveData::save(QString username, QString password)
{
    std::cout << "Trying to save data..." << std::endl;
    QFile file(FILE_NAME);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
        return false; /* IO Error! */
    QTextStream output(&file);
    QString hash_pass = hashPassword(password);
    output << "[status]true[/]\n[username]"<<username<<"[/]\n"<<"[password]"<<hash_pass<<"[/]"; //Un Hashed Password in string!
    file.close();
    std::cout << "Saved!" << std::endl;
    return true;
}

bool SaveData::saveCal(QString cal)
{
    std::cout << "Trying to save data..." << std::endl;
    QFile file(FILE_NAME);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
        return false; /* IO Error! */
    QTextStream output(&file);
    QString text = output.readAll();
    QString line = text.

}

/**
 * @brief Deletes the data from file, and sets status tag to false.
 * @return true if success
 */
bool SaveData::deleteData()
{
    QFile file(FILE_NAME);
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))
        return false; /* IO Error! */
    QTextStream output(&file);
    output << DEFAULT_DATA_EMPTY;
    file.close();
    return true;
}

/**
 * @brief username getter
 * @return QString - username
 */
QString SaveData::getUsername()
{
    QString username = "";
    QFile* file = new QFile(FILE_NAME);

    /* Opening file for read */
    if(file->open(QIODevice::ReadOnly|QIODevice::Text))
        username = getValViaTag("username", file);
    file->close();
    return username;

}

/**
 * @brief getter for password
 * @return QString - password
 */
QString SaveData::getPassword()
{
    QString pass = "";
    QFile* file = new QFile(FILE_NAME);
    /* Opening file for read */
    if(file->open(QIODevice::ReadOnly|QIODevice::Text))
            pass = getValViaTag("password", file);
    file->close();
   pass = deHashPasword(pass);
    return pass;

}

/**
 * @brief Getiing the last calendar user hase been viewing.
 * @return QString - cal - in format [year]-[semester]
 */
QString SaveData::gatCal()
{
    QString cal = "";
    QFile* file = new QFile(FILE_NAME);
    /* Opening file for read */
    if(file->open(QIODevice::ReadOnly|QIODevice::Text))
            cal = getValViaTag("cal", file);
    file->close();
    return cal;
}

/**
 * @brief Getiing the users local - default is the OS local
 * @return QString - local (heb/en/default)
 */
QString SaveData::getLocal()
{
    QString local = "";
    QFile* file = new QFile(FILE_NAME);
    /* Opening file for read */
    if(file->open(QIODevice::ReadOnly|QIODevice::Text))
            local = getValViaTag("local", file);
    file->close();
    return local;
}

/**
 * @brief This is A functions that will init the tags file.
 * if it exist - do nothing
 * if there is no file, it will create it and run the deleteData function
 * so tags will be set.
 */
void SaveData::init()
{
    QFile file(FILE_NAME);
    if(!file.exists())
    {
         file.open(QIODevice::ReadWrite | QIODevice::Text);
         file.close();
         deleteData();
    }
}

/**
 * @brief This function will return the vale of a given tag name.
 * NOTE:    valid tag names are : "status", "username", "password"
 *          if tag is invalid - will return "" - an empty QString!
 * @param tag - QString, the tag name
 * @param file - a QFile pointer
 * @return  QString - the value in tag
 *          NOTE: if NULL value, or an invalid tag name -> return "" (empty QString!)
 */
QString SaveData::getValViaTag(QString tag, QFile* file)
{
    QString val, line , tmpTag;
    QTextStream textStream( file);
    while((line = textStream.readLine()) != NULL)
    {
        std::cout << "DEBUG: line => " << line.toStdString() << std::endl;
        for(int i = 0 ; i< line.length() ;++i)
        {
            if(line[i] == '[' && line[i+1] != '/') //get open tag at begining of line and not end of tag ("[/")
            {
                i++;
                tmpTag = line.mid(i, tag.length());
                if(tmpTag == tag)
                {
                    i+= tag.length()+1; // i is now right after '['
                    int j = i;
                    while(line[j] != '[')// put j at the end of the value
                        j++;
                    /* Then... the value is :*/
                    val = line.mid(i, j-i);
                    std::cout << "DEBUG:  ["<<tag.toStdString()<<"]"<<val.toStdString()<<"[/]"<<std::endl;
                    return val;
                }
            }
        }
    }
    return ""; //this will be as NULL
}

/**
 * @brief   This function will provide the user the most basic security of the stored password.
 *          it is NOT an encryption! it will store every char of the password as ascii encoded,
 *          separated by "-" for the deHsh func.
 * @param pass - a QString to "hash"
 * @return QString - The "hashed" password
 */
QString SaveData::hashPassword(QString pass)
{
    QString hash="";
    int code;
    for(QChar c : pass)
    {
        code = (int)c.toLatin1();
        hash.append(QString::number(code));
        hash.append("-");
    }
    return hash;
}


/**
 * @brief   This function will "dehash" the "hashed" password.
 *          uses standard c string.h strtok with the "-" as the choping points.
 *          builds a QString from the ascii.
 * @param pass - The hashed password
 * @return QString - password uncoded
 */
QString SaveData::deHashPasword(QString pass)
{
    char* cpyPass = strdup(pass.toStdString().c_str());
    char* tok;
    char* ch;
    int val;
    QString password;
    tok = strtok(cpyPass,"-");
    while (tok != NULL)
    {
        ch = tok;
        val = atoi(ch);
        password.append(val);
        tok = strtok(NULL, "-");
    }
    return password;
}

bool SaveData::isTagLine(QString line, QString tag)
{
    for(int i = 0 ; i< line.length() ;++i)
    {
        if(line[i] == '[' && line[i+1] != '/') //get open tag at begining of line and not end of tag ("[/")
        {
            i++;
            tmpTag = line.mid(i, tag.length());
            if(tmpTag == tag)
                return true;
        }
        return false;
    }
    return false;
}
