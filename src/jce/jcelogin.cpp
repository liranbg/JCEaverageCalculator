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

void jceLogin::makeConnection() throw (jceStatus)
{
    jceStatus status;
    cout << "Connecting with username: " << jceA->getUsername() << std::endl;

    if (checkConnection() == true)
    {
        cout << "first visiting..." << endl;
        if (makeFirstVisit() == true)
        {
            status = jceStatus::JCE_FIRST_VALIDATION_PASSED;
            cout << "validating..." << endl;
            if (checkValidation() == true)
            {
                cout << "singing in" << endl;
                status = jceStatus::JCE_SECOND_VALIDATION_PASSED;
                if (makeSecondVisit() == true)
                {
                    status = jceStatus::JCE_YOU_ARE_IN;
                    setLoginFlag(true);
                }
                else
                {
                    status = jceStatus::ERROR_ON_VALIDATION;
                    cout << "not in evantually. error!" << std::endl;
                }

            }
            else
            {
                status = jceStatus::ERROR_ON_VALIDATION;
                cout<< "error on validation" << std::endl;
            }

        }
        else
        {
            cout << "error on first visit" << std::endl;
            status = jceStatus::ERROR_ON_VALIDATION_USER_BLOCKED;
        }

    }
    else
    {
        cout << "error on creating socket" << std::endl;
        status = jceStatus::ERROR_ON_OPEN_SOCKET;
    }
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

void jceLogin::setProgressBar(QProgressBar *ptr)
{
    this->JceConnector->setProgressBar(ptr);
}
bool jceLogin::isLoginFlag() const
{
    return this->loginFlag;
}

std::string jceLogin::getPage()
{
    return *recieverPage;
}

bool jceLogin::checkValidation()
{
    //finds the hashed password

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
