#include "loginhandler.h"

loginHandler::loginHandler()
{

}
void loginHandler::setPointers(jceLogin *ptr,QLabel *statusLabelPtr,QLineEdit *pswdEditPtr,QLineEdit *usrnmEditPtr)
{
    this->jceLog = ptr;
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
            return true;
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
             *  Fix: need to add promte window to ask user whenever he wants to reconnect or not
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


