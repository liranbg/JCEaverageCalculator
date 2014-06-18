#include "loginhandler.h"

loginHandler::loginHandler(jceLogin *ptr,QLabel *statusLabelPtr,QLineEdit *pswdEditPtr,QLineEdit *usrnmEditPtr)
{
    this->jceLog = ptr;
    this->statusLabelPtr = statusLabelPtr;
    this->pswdEditPtr = pswdEditPtr;
    this->usrnmEditPtr = usrnmEditPtr;
}

bool loginHandler::makeConnection()
{
    if (this->jceLog == NULL)
    {
        return false;
    }
    try
    {
        jceLog->makeConnection();
    }
    catch (jceLogin::jceStatus &a)
    {

        if (a == jceLogin::JCE_YOU_ARE_IN)
        {
            return true;
        }
        else if (a == jceLogin::ERROR_ON_VALIDATION)
        {
            popMessage("check your password");

            usrnmEditPtr->setDisabled(false);
            pswdEditPtr->setDisabled(false);

            pswdEditPtr->selectAll();
            pswdEditPtr->setFocus();
            return false;
        }
        else if (a == jceLogin::ERROR_ON_OPEN_SOCKET)
        {
            popMessage("Please check your internet status");

            jceLog->closeAll();

            return false;
        }
        else if (a == jceLogin::ERROR_ON_VALIDATION_USER_BLOCKED)
        {
            std::cout << "error!!!!!!" <<  (jceLog->getPage()) << std::endl;
            popMessage("You were blocked, please wait couple of minutes or contact JCE");
            jceLog->closeAll();

            return false;
        }
    }
    return false;
}
void loginHandler::popMessage(QString message)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle("Error");
    msgBox.setText(message);
    msgBox.exec();
    msgBox.setFocus();
}


