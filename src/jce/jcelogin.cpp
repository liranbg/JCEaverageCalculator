#include "jcelogin.h"

jceLogin::jceLogin(user * username)
{
    this->recieverPage = new std::string();
    this->jceA = username;
    this->JceConnector = new qtsslsocket(dst_host, dst_port);
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
    jceStatus status = jceStatus::JCE_NOT_CONNECTED;

    if (checkConnection() == true) //connected to host
    {
        if (makeFirstVisit() == true) //requst and send first validation
        {
            std::cout << "visit 1\n";
            std::cout << *(this->recieverPage);
            status = jceStatus::JCE_FIRST_VALIDATION_PASSED;
            if (checkValidation() == true) //check if username and password are matching
            {
                std::cout << "visit 2\n";
                std::cout << *(this->recieverPage);
                status = jceStatus::JCE_SECOND_VALIDATION_PASSED;
                if (makeSecondVisit() == true) //siging in the website
                {
                    std::cout << "visit 3\n";
                    std::cout << *(this->recieverPage);
                    status = jceStatus::JCE_YOU_ARE_IN;
                    setLoginFlag(true);
                }
                else
                    status = jceStatus::ERROR_ON_VALIDATION;
            }
            else
                status = jceStatus::ERROR_ON_VALIDATION;

        }
        else
            status = jceStatus::ERROR_ON_VALIDATION_USER_BLOCKED;

    }
    else
        status = jceStatus::ERROR_ON_OPEN_SOCKET;

    //we throw status even if we are IN!
    throw status;

}

bool jceLogin::checkConnection()
{
    if (JceConnector->isCon())
        return true;

    return false;
}

void jceLogin::reConnect()  throw (jceStatus)
{
    closeAll();
    this->recieverPage = new std::string();
    this->JceConnector = new qtsslsocket(dst_host, dst_port);
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
    delete recieverPage;
    recieverPage = NULL;
    JceConnector = NULL;

}

int jceLogin::makeFirstVisit()
{
    std::string usr = jceA->getUsername();
    std::string psw = jceA->getPassword();
    if (JceConnector->send(jceLoginHtmlScripts::makeRequest(jceLoginHtmlScripts::getFirstValidationStep(*jceA))))
    {
        if (!JceConnector->recieve(*recieverPage))
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
    if ((JceConnector->send(jceLoginHtmlScripts::makeRequest(jceLoginHtmlScripts::getSecondValidationStep(*jceA)))))
    {
        if (!(JceConnector->recieve(*recieverPage)))
            return jceLogin::ERROR_ON_GETTING_INFO;

        return true;
    }
    else
        return jceLogin::ERROR_ON_SEND_REQUEST;

    return true;
}

int jceLogin::getGrades()
{
    if  ((JceConnector->send(jceLoginHtmlScripts::makeRequest(jceLoginHtmlScripts::getGradesPath(*jceA)))))
    {
        if (!(JceConnector->recieve(*recieverPage)))
            return jceLogin::ERROR_ON_GETTING_GRADES;

        return true;
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
    return this->loginFlag;
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
    std::cout << *recieverPage << std::endl;

    std::size_t hasspass_position1 = recieverPage->find("-A,-N");
    hasspass_position1 += 5;
    std::size_t hasspass_position2 = recieverPage->find(",-A,-A", hasspass_position1);
    if ((hasspass_position2 != std::string::npos) && (hasspass_position1 != std::string::npos))
    {
        std::string hasspass = recieverPage->substr(hasspass_position1,hasspass_position2-hasspass_position1);
        jceA->setHashedPassword(hasspass);
    }
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
