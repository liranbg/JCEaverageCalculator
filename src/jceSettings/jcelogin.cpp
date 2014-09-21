#include "jcelogin.h"

/**
 * @brief jceLogin::jceLogin
 * @param username pointer to allocated user settings
 */
jceLogin::jceLogin(user* username)
{
    this->recieverPage = new QString();
    this->jceA = username;
    this->JceConnector = new jceSSLClient();
    QObject::connect(JceConnector,SIGNAL(serverDisconnectedbyRemote()),this,SLOT(reValidation()));
}

jceLogin::~jceLogin()
{
    this->jceA = NULL;
    delete recieverPage;
    delete JceConnector;
    JceConnector = NULL;
    recieverPage = NULL;
}
/**
 * @brief jceLogin::makeConnection  Connecting to JCE student web site with JceA (username object) and validate it.
 *                  throws error upon the given error from JCE website or Socket error
 */
void jceLogin::makeConnection() throw (jceStatus)
{
    qDebug() << "jceLogin::makeConnection(); connection to be make";

    if (this->recieverPage == NULL)
        this->recieverPage = new QString();

    if (JceConnector->makeConnect(dst_host,dst_port) == false) //couldnt make a connection
        throw jceStatus::ERROR_ON_OPEN_SOCKET;

    int returnMode; //gets status according to called function of validation step
    jceStatus status = jceStatus::JCE_NOT_CONNECTED;

    returnMode = checkConnection(); //checking socket status. is connected?

    if (returnMode == true) //connected to host
    {
        returnMode = makeFirstVisit();
        if (returnMode == true) //requst and send first validation
        {
            status = jceStatus::JCE_START_VALIDATING_PROGRESS;
            returnMode = checkValidation();
            if (returnMode == true) //check if username and password are matching
            {
                status = jceStatus::JCE_VALIDATION_PASSED;
                returnMode = makeSecondVisit();
                if (returnMode == true) //siging in the website
                {
                    qDebug() << "jceLogin::makeConnection(); Signed in succeesfully";
                    status = jceStatus::JCE_YOU_ARE_IN;
                    setLoginFlag(true);
                }
                else if (returnMode == jceLogin::ERROR_ON_GETTING_INFO)
                {
                    status = jceLogin::ERROR_ON_GETTING_INFO;
                }
                else if (returnMode == jceLogin::ERROR_ON_SEND_REQUEST)
                {
                    status = jceLogin::ERROR_ON_SEND_REQUEST;
                }
                else
                    status = jceStatus::ERROR_ON_VALIDATION;
            }
            else
                status = jceStatus::ERROR_ON_VALIDATION;

        }
        else if (returnMode == jceLogin::ERROR_ON_GETTING_INFO)
        {
            status = jceLogin::ERROR_ON_GETTING_INFO;
        }
        else if (returnMode == jceLogin::ERROR_ON_SEND_REQUEST)
        {
            status = jceLogin::ERROR_ON_SEND_REQUEST;
        }
        else
            status = jceStatus::ERROR_ON_VALIDATION_USER_BLOCKED;

    }
    else
        status = jceStatus::JCE_NOT_CONNECTED;

    //we throw status even if we are IN!
    qDebug() << "jceLogin::makeConnection(); throw status: " << status;
    throw status;

}
/**
 * @brief jceLogin::checkConnection
 * @return  if connected true, otherwise false
 */
bool jceLogin::checkConnection() const
{
    if (JceConnector->isConnected())
        return true;

    return false;
}
/**
 * @brief jceLogin::reConnect
 * closing connection and deleting pointers.
 * calling class's makeConnection function and throw the exception of it.
 */
void jceLogin::reConnect()  throw (jceStatus)
{
    closeAll();
    if (this->JceConnector != NULL)
        delete JceConnector;
    this->recieverPage = new QString();
    this->JceConnector = new jceSSLClient();

    try
    {
        makeConnection();
    }
    catch (jceLogin::jceStatus &a)
    {
        throw a;
    }
}
/**
 * @brief jceLogin::closeAll
 */
void jceLogin::closeAll()
{
    JceConnector->makeDiconnect();
    delete recieverPage;
    recieverPage = NULL;

}
/**
 * @brief jceLogin::makeFirstVisit making the first validation step of jce student portal login
 * @return jceLogin enum list on error, true if valid
 */
int jceLogin::makeFirstVisit()
{
    QString usr = jceA->getUsername();
    QString psw = jceA->getPassword();
    if (JceConnector->sendData(jceLoginHtmlScripts::makeRequest(jceLoginHtmlScripts::getFirstValidationStep(*jceA))))
    {
        if (!JceConnector->recieveData(*recieverPage,true))
            return jceLogin::ERROR_ON_GETTING_INFO;
    }
    else
        return jceLogin::ERROR_ON_SEND_REQUEST;

    return true;
}
/**
 * @brief jceLogin::makeSecondVisit making the second validation step of jce student portal login
 * @return jceLogin enum list on error, true if valid
 */
int jceLogin::makeSecondVisit()
{
    QString usrid=jceA->getUserID();
    QString pswid=jceA->getHashedPassword();
    if ((JceConnector->sendData(jceLoginHtmlScripts::makeRequest(jceLoginHtmlScripts::getSecondValidationStep(*jceA)))))
    {
        if (!(JceConnector->recieveData(*recieverPage,true)))
            return jceLogin::ERROR_ON_GETTING_INFO;

        return true;
    }
    else
        return jceLogin::ERROR_ON_SEND_REQUEST;

    return true;
}
/**
 * @brief jceLogin::getCalendar according to parameters, we make an HTML request and send it over socket to server
 * @param year      -   selected year
 * @param semester  -   selected semester
 * @return true if ok, jceLogin enum on error (recieverPage has html to parse if true)
 */
int jceLogin::getCalendar(int year, int semester)
{
    if  ((JceConnector->sendData(jceLoginHtmlScripts::makeRequest(jceLoginHtmlScripts::getCalendar(*jceA,year,semester)))))
    {
        if (!(JceConnector->recieveData(*recieverPage,false)))
            return jceLogin::ERROR_ON_GETTING_GRADES;
        else
            return jceLogin::JCE_GRADE_PAGE_PASSED;
    }
    else
        return jceLogin::ERROR_ON_SEND_REQUEST;

    return true;

}
/**
 * @brief jceLogin::getGrades according to parameters, we make an HTML request and send it over socket to server
 * @param fromYear  -   from year
 * @param toYear    -   to year
 * @param fromSemester  -   from semester
 * @param toSemester    -   to semester
 * @return true if ok, jceLogin enum on error (recieverPage has html to parse if true)
 */
int jceLogin::getGrades(int fromYear, int toYear, int fromSemester, int toSemester)
{
    if  ((JceConnector->sendData(jceLoginHtmlScripts::makeRequest(jceLoginHtmlScripts::getGradesPath(*jceA,fromYear, toYear, fromSemester, toSemester)))))
    {
        if (!(JceConnector->recieveData(*recieverPage,false)))
            return jceLogin::ERROR_ON_GETTING_GRADES;
        else
            return jceLogin::JCE_GRADE_PAGE_PASSED;
    }
    else
        return jceLogin::ERROR_ON_SEND_REQUEST;

    return true;

}
/**
 * @brief jceLogin::checkValidation Made by Nadav Luzzato
 * @return  true if second validation step is right
 */
bool jceLogin::checkValidation()
{

    //finds the hashed password
    QString constUserID_TAG = "value=\"-N";
    QString constHassID_TAG = "-A,-N";
    QString hasspass,hassid;
    std::size_t hasspass_position1,hasspass_position2;
    std::size_t id_position1,id_position2;

    hasspass_position1 = this->recieverPage->toStdString().find(constHassID_TAG.toStdString()); //looking for hasspass index
    if (hasspass_position1 == std::string::npos) //didnt find the tag
        return false;
    else
        hasspass_position1 += constHassID_TAG.length(); //skip the index of tag
    hasspass_position2 = this->recieverPage->toStdString().find(",-A,-A", hasspass_position1);
    //finds the hass pass
    if (hasspass_position2 != std::string::npos) //found the hasspass! storing it
        hasspass = recieverPage->mid(hasspass_position1,hasspass_position2-hasspass_position1);
    else
        return false;
    //finds the user id
    id_position1 = this->recieverPage->toStdString().find(constUserID_TAG.toStdString(), 0); //looking for hassid index
    if (id_position1 == std::string::npos) //didnt find the tag
        return false;
    else
        id_position1 += constUserID_TAG.length(); //skip the index of tag
    id_position2 = this->recieverPage->toStdString().find(",-A", id_position1);
    if (id_position2 != std::string::npos) //found the hassid! storing it
        hassid = recieverPage->mid(id_position1,id_position2-id_position1);
    else
        return false;

    //setting user information with given data hassid and hasspass
    jceA->setHashedPassword(hasspass);
    jceA->setUserID(hassid);

    qDebug() << "jceLogin::checkValidation(); Found Hashed: " << hasspass << "And ID: " << hassid;

    return true;
}
/**
 * @brief jceLogin::setLoginFlag
 * @param x
 */
void jceLogin::setLoginFlag(bool x)
{
    this->loginFlag = x;
}
/**
 * @brief jceLogin::isLoginFlag checking if there is a connection, if true - > return if we signed in. otherwise, return not (not connected dough)
 * @return
 */
bool jceLogin::isLoginFlag() const
{
    if (checkConnection())
        return this->loginFlag;
    return false;

}
/**
 * @brief jceLogin::getPage
 * @return html page
 */
QString jceLogin::getPage()
{
    return *recieverPage;
}

void jceLogin::reValidation()
{
    qDebug() << Q_FUNC_INFO << "Revalidating user";
    if (makeFirstVisit())
    {
        if (makeSecondVisit())
            qDebug() << Q_FUNC_INFO << "Validated";
    }

}
