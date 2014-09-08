#include "loginhandler.h"

loginHandler::loginHandler(user *ptr): logggedInFlag(false)
{
    this->jceLog = new jceLogin(ptr);
}
void loginHandler::setPointers(QLabel *statusLabelPtr,QLineEdit *pswdEditPtr,QLineEdit *usrnmEditPtr)
{

    this->statusLabelPtr = statusLabelPtr;
    this->pswdEditPtr = pswdEditPtr;
    this->usrnmEditPtr = usrnmEditPtr;
}

bool loginHandler::makeConnection()
{
    if (this->jceLog == NULL)
        return false;

    try
    {
        jceLog->makeConnection();
    }
    catch (jceLogin::jceStatus &a)
    {
        int status = (int)a;
        switch (status)
        {
        case jceLogin::JCE_YOU_ARE_IN:
        {
            logggedInFlag = true;
            return logggedInFlag;
            break;
        }
        case jceLogin::ERROR_ON_VALIDATION:
        {
            popMessage("Please Check Your Username & Password",false);

            usrnmEditPtr->setDisabled(false);
            pswdEditPtr->setDisabled(false);

            pswdEditPtr->selectAll();
            pswdEditPtr->setFocus();
            return false;
        }
        case jceLogin::ERROR_ON_VALIDATION_USER_BLOCKED:
        {
            popMessage("You have been blocked by JCE, please try in a couple of minutes.");
            jceLog->closeAll();
            return false;
        }
        case jceLogin::ERROR_ON_OPEN_SOCKET:
        {
            popMessage("Please Check Your Internet Connection.");
            jceLog->closeAll();
            return false;
        }
        case jceLogin::JCE_NOT_CONNECTED:
        {
            jceLog->reConnect();
            /*
             *  Fix: need to add a prompte window to ask user whether he wants to reconnect or not
             */
            break;
        }
        case jceLogin::ERROR_ON_GETTING_INFO:
        {
            popMessage("Recieve Request Time Out.");
            jceLog->closeAll();
            return false;
            break;
        }
        case jceLogin::ERROR_ON_SEND_REQUEST:
        {
            popMessage("Send Request Time Out.");
            jceLog->closeAll();
            return false;
            break;
        }
        }
    }
    return false;
}


bool loginHandler::isLoggedInFlag()
{
    if (jceLog->isLoginFlag()) //checking connection and then if logged in
        return this->logggedInFlag;
    else
        this->setLoginFlag(false);
    return false;
}

void loginHandler::setLoginFlag(bool flag)
{
    this->logggedInFlag = flag;
}

QString loginHandler::getCurrentPageContect()
{
    QTextEdit phrase;
    if (isLoggedInFlag())
        phrase.setText(QString::fromStdString(jceLog->getPage()));
    else
        throw jceLogin::ERROR_ON_GETTING_INFO;

    return phrase.toPlainText();
}

void loginHandler::makeDisconnectionRequest()
{
    jceLog->closeAll();
    this->logggedInFlag = false;
}

int loginHandler::makeGradeRequest(int fromYear, int toYear, int fromSemester, int toSemester)
{
    if (isLoggedInFlag())
        return jceLog->getGrades(fromYear, toYear, fromSemester, toSemester);
    else
        return jceLogin::JCE_NOT_CONNECTED;
}

int loginHandler::makeCalendarRequest(int year, int semester)
{
    if (isLoggedInFlag())
        return jceLog->getCalendar(year,semester);
    else
        return jceLogin::JCE_NOT_CONNECTED;
}
void loginHandler::popMessage(QString message,bool addInfo)
{
    if (addInfo)
        message.append("\nIf this message appear without reason, please contact me at liranbg@gmail.com");

    QMessageBox msgBox;
    msgBox.setWindowTitle("Error");
    msgBox.setText(message);
    msgBox.exec();
    msgBox.setFocus();

}


