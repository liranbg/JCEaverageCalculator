#include "jcelogin.h"

jceLogin::jceLogin(user * username)
{
    this->recieverPage = new std::string();
    this->jceA = username;
    this->JceConnector = new jceSSLClient();
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
    if (this->recieverPage == NULL)
        this->recieverPage = new std::string();

    if (JceConnector->makeConnect(dst_host,dst_port) == false)
        throw jceStatus::ERROR_ON_OPEN_SOCKET;

    int returnMode;
    jceStatus status = jceStatus::JCE_NOT_CONNECTED;

    returnMode = checkConnection();
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
    throw status;

}

bool jceLogin::checkConnection() const
{
    if (JceConnector->isConnected())
        return true;

    return false;
}

void jceLogin::reConnect()  throw (jceStatus)
{
    closeAll();
    if (this->JceConnector != NULL)
        delete JceConnector;
    this->recieverPage = new std::string();
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

void jceLogin::closeAll()
{
    JceConnector->makeDiconnect();
    delete recieverPage;
    recieverPage = NULL;
    loginFlag = false;

}

int jceLogin::makeFirstVisit()
{
    std::string usr = jceA->getUsername();
    std::string psw = jceA->getPassword();
    if (JceConnector->sendData(jceLoginHtmlScripts::makeRequest(jceLoginHtmlScripts::getFirstValidationStep(*jceA))))
    {
        if (!JceConnector->recieveData(*recieverPage,true))
            return jceLogin::ERROR_ON_GETTING_INFO;
    }
    else
        return jceLogin::ERROR_ON_SEND_REQUEST;

    return true;
}

int jceLogin::makeSecondVisit()
{
    std::string usrid=jceA->getUserID();
    std::string pswid=jceA->getHashedPassword();
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
int jceLogin::getGrades(int fromYear, int toYear, int fromSemester, int toSemester)
{
    std::cout << fromYear << " " << toYear << " " << fromSemester << " " << toSemester << std::endl;
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

void jceLogin::setLoginFlag(bool x)
{
    this->loginFlag = x;
}
bool jceLogin::isLoginFlag() const
{
    if (checkConnection())
        return this->loginFlag;
    return false;
}

std::string jceLogin::getPage()
{
    return *recieverPage;
}

/**
 * @brief jceLogin::checkValidation Made by Nadav Luzzato
 * @return  true if second validation step is right
 */
bool jceLogin::checkValidation()
{
    //finds the hashed password
    std::size_t hasspass_position1,hasspass_position2;

    if ((hasspass_position1 = recieverPage->find("-A,-N")) == string::npos)
        return false;
    hasspass_position1 += 5;
    if ((hasspass_position2 = recieverPage->find(",-A,-A", hasspass_position1)) == string::npos)
        return false;

    std::string hasspass = recieverPage->substr(hasspass_position1,hasspass_position2-hasspass_position1);
    jceA->setHashedPassword(hasspass);

    //finds the user id
    std::size_t id_position1 = recieverPage->find("value=\"-N", 0);
    id_position1 += 9;
    std::size_t id_position2 = recieverPage->find(",-A", id_position1);
    if ((id_position2 != std::string::npos) && (id_position1 != std::string::npos))
    {
        std::string hassid = recieverPage->substr(id_position1,id_position2-id_position1);
        jceA->setUserID(hassid);
    }
    if (((jceA->getUserID()).empty()) || ((jceA->getHashedPassword()).empty()))
        return false;

    return true;
}
