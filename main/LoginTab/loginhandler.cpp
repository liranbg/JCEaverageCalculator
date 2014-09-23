#include "loginhandler.h"

loginHandler::loginHandler(user *ptr, QStatusBar *statusBarPtr,QPushButton *loginButtonPtr): logggedInFlag(false)
{
    this->loginButtonPtr = loginButtonPtr;

    //statusBar
    statusBar = statusBarPtr;
    iconButtomStatusLabel = new QLabel();
    statusBar->addPermanentWidget(iconButtomStatusLabel,0);
    setIconConnectionStatus(jceLogin::jceStatus::JCE_NOT_CONNECTED);

    //user settings
    userPtr = ptr;
    this->jceLog = new jceLogin(userPtr);
    QObject::connect(this->jceLog,SIGNAL(connectionReadyAfterDisconnection()),this,SLOT(readyAfterConnectionLost()));
}
bool loginHandler::login(QString username,QString password)
{
    qDebug() << Q_FUNC_INFO << "logging in with username and password";
    if (isLoggedInFlag())
    {
        qDebug() << Q_FUNC_INFO << "we are already connected. Logging out and return false";
        logout();
        return false;
    }
    setIconConnectionStatus(jceLogin::jceStatus::JCE_START_VALIDATING_PROGRESS);

    userPtr->setUsername(username);
    userPtr->setPassword(password);

    if (makeConnection() == true)
    {
        setIconConnectionStatus(jceLogin::jceStatus::JCE_YOU_ARE_IN);
        loginButtonPtr->setText(QObject::tr("Logout"));
        return isLoggedInFlag();
    }
    else
    {
        logout();
        return false;
    }
}

void loginHandler::logout()
{
    loginButtonPtr->setText(QObject::tr("Login"));
    setIconConnectionStatus(jceLogin::jceStatus::JCE_NOT_CONNECTED);
    jceLog->closeAll();
    logggedInFlag = false;
}

bool loginHandler::makeConnection()
{
    if (this->jceLog == NULL)
        return false;

    int status = (int)jceLog->makeConnection();
    switch (status)
    {
    case jceLogin::JCE_YOU_ARE_IN:
    {
        logggedInFlag = true;
        return logggedInFlag;
    }
    case jceLogin::ERROR_ON_VALIDATION:
    {
        popMessage(QObject::tr("Please Check Your Username & Password"),false);
        return false;
    }
    case jceLogin::ERROR_ON_VALIDATION_USER_BLOCKED:
    {
        popMessage(QObject::tr("You have been <b>BLOCKED</b> by JCE, please try in a couple of minutes."));
        return false;
    }
    case jceLogin::ERROR_ON_OPEN_SOCKET:
    {
        popMessage(QObject::tr("Please Check Your Internet Connection."));
        return false;
    }
    case jceLogin::JCE_NOT_CONNECTED:
    {
        break;
    }
    case jceLogin::ERROR_ON_GETTING_INFO:
    {
        popMessage(QObject::tr("Receive Request Timeout."));
        return false;
    }
    case jceLogin::ERROR_ON_SEND_REQUEST:
    {
        popMessage(QObject::tr("Send Request Timeout."));
        return false;
    }
    }

    return false;
}

void loginHandler::readyAfterConnectionLost()
{
    qWarning() << Q_FUNC_INFO;
    setLoginFlag(false);
    login(userPtr->getUsername(),userPtr->getPassword());

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
        phrase.setText(jceLog->getPage());
    else
        throw jceLogin::ERROR_ON_GETTING_INFO;

    return phrase.toPlainText();
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
void loginHandler::setIconConnectionStatus(jceLogin::jceStatus statusDescription)
{
    QPixmap iconPix;
    switch (statusDescription)
    {
    case jceLogin::jceStatus::JCE_START_VALIDATING_PROGRESS:
        iconPix.load(":/icons/blueStatusIcon.png");
        statusBar->showMessage(tr("Connecting..."));
        break;
    case jceLogin::jceStatus::JCE_YOU_ARE_IN:
        iconPix.load(":/icons/greenStatusIcon.png");
        statusBar->showMessage(tr("Connected"));
        break;
    case jceLogin::jceStatus::JCE_NOT_CONNECTED:
        iconPix.load(":/icons/redStatusIcon.png");
        statusBar->showMessage(tr("Disconnected"));
        break;
    default:
        iconPix.load(":/icons/redStatusIcon.png");
        statusBar->showMessage(tr("Ready."));
        break;
    }
    iconButtomStatusLabel->setPixmap(iconPix);

    this->statusBar->repaint();
}
void loginHandler::popMessage(QString message,bool addInfo)
{
    if (addInfo)
        message.append(QObject::tr("\nIf this message appear without reason, please contact me at liranbg@gmail.com"));

    QMessageBox msgBox;
    msgBox.setWindowTitle(QObject::tr("Error"));
    msgBox.setText(message);
    msgBox.exec();
    msgBox.setFocus();

}


