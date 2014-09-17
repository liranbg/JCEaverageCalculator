#include "jcesslclient.h"
/**
 * @brief jceSSLClient::jceSSLClient  Constructer, setting the signals
 */
jceSSLClient::jceSSLClient() : flag(false), packet("")
{
    //setting signals
    connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(checkErrors(QAbstractSocket::SocketError)));
    connect(this,SIGNAL(connected()),this,SLOT(setConnected()));
    connect(this,SIGNAL(encrypted()),this,SLOT(setEncrypted()));
    connect(this,SIGNAL(disconnected()),this,SLOT(setDisconnected()));

    //loop event will connect the server, and when it is connected, it will quit - but connection will be open
   connect(this, SIGNAL(encrypted()), &loop, SLOT(quit()));
   connect(this,SIGNAL(error(QAbstractSocket::SocketError)),&loop,SLOT(quit()));

}

/**
 * @brief jceSSLClient::makeConnect connecting to server with given port. using eventloop to assure it wont stuck the application.
 * @param server  - server to connect to
 * @param port    - the using port
 * @return        - true if connected, false otherwise
 */
bool jceSSLClient::makeConnect(QString server, int port)
{
    qDebug() << "jceSSLClient::makeConnect; Making connection";
    if (isConnected())
    {
        qWarning() << "jceSSLClient::makeConnect; Was already connected. Aborting.";
        makeDiconnect();
    }
    if (isOpen())
    {
        qWarning() << "jceSSLClient::makeConnect; IsO pen. Aborting.";
        makeDiconnect();
    }

    qDebug() << "jceSSLClient::makeConnect; Connection to: " << server << "On Port: " << port;
    connectToHostEncrypted(server.toStdString().c_str(), port);

    loop.exec(); //starting connection, waiting to encryption and then it ends

    qDebug() << "jceSSLClient::makeConnect; returning the connection status: " << isConnected();
    return isConnected();

}
/**
 * @brief jceSSLClient::makeDiconnect from current server
 * @return the flag of connection status
 */
bool jceSSLClient::makeDiconnect()
{
    qDebug() << "jceSSLClient::makeDiconnect;";
    if (loop.isRunning())
  {
        qWarning() << "jceSSLClient::makeDiconnect; Killing connection thread";
        loop.exit();
   }
        if (state() == QAbstractSocket::SocketState::UnconnectedState)
    {
        qDebug() << "jceSSLClient::makeDiconnect; Disconnected with [UnconnectedState]";
        flag = false;
    }
    else if (state() == QAbstractSocket::SocketState::ConnectedState)
    {
        qWarning() << "jceSSLClient::makeDiconnect; Disconnecting with [ConnectedState] ";
        abort();
        if (state() != QAbstractSocket::SocketState::UnconnectedState)
        {
            qWarning() << "jceSSLClient::makeDiconnect; still open! recursion call to disconnect";
            abort();//still connected? ensure the disconnection
            flag = makeDiconnect(); //recursion call!
        }
        flag = false;
    }
    else
    {
        qWarning() << "jceSSLClient::makeDiconnect; Disconnecting [else] ";
        abort(); //ensure the disconnection
        flag = false;
    }
    qDebug() << "jceSSLClient::makeDiconnect; disconnect return with " << flag;
    return flag;
}

/**
 * @brief jceSSLClient::isConnected connection checking
 * @return regardless to state, it checks if there's a connection
 */
bool jceSSLClient::isConnected()
{
    //checking state before returning flag!
    if (state() == QAbstractSocket::SocketState::UnconnectedState)
    {
        flag = false;
    }
    else if (state() == QAbstractSocket::SocketState::ClosingState)
    {
        flag = false;
    }
    else if (state() == QAbstractSocket::SocketState::ConnectedState)
    {
        flag = true;
    }
    return flag;
}
/**
 * @brief jceSSLClient::sendData  - given string, send it to server
 * @param str - string to send
 * @return true if succeed and byte were written
 */
bool jceSSLClient::sendData(QString str)
{
    bool sendDataFlag = false;
    if (isConnected()) //if connected
    {
        write(str.toStdString().c_str(),str.length());
        if (waitForBytesWritten())
            sendDataFlag = true;
    }
    qDebug() << "jceSSLClient::sendData; Sending Data status is: " << sendDataFlag;
    return sendDataFlag;
}
/**
 * @brief jceSSLClient::recieveData
 * @param str   this variable will store the recieved data
 * @param fast  true for LOGIN ONLY, false to retrieve all data
 * @return  true if recieved data bigger than zero
 */
bool jceSSLClient::recieveData(QString &str, bool fast)
{
    qDebug() << "jceSSLClient::recieveData Data receiving!";
    packet = "";
    bool sflag = false;

    if (fast) //fast mode connection, good only for login step!!!!
    {
        qDebug() << "jceSSLClient::recieveData login step receiving";
        QEventLoop loop;
        connect(this, SIGNAL(readyRead()), &loop, SLOT(quit()));
        connect(this, SIGNAL(readyRead()), this, SLOT(readIt()));
        loop.exec();
        disconnect(this, SIGNAL(readyRead()), &loop, SLOT(quit()));
        disconnect(this, SIGNAL(readyRead()), this, SLOT(readIt()));
    }
    else
    {
        qDebug() << "jceSSLClient::recieveData normal receiving";
        QString p;
        while (waitForReadyRead(milisTimeOut))
        {
            do
            {
                p = readAll();
                packet.append(p);
            }while (p.size() > 0);
        }
    }
    str = packet;
    qDebug() << "jceSSLClient::recieveData received bytes: " << str.length() ;
    if (str.length() > 0)
        sflag = true;
    qDebug() << "jceSSLClient::recieveData return with flag: " << sflag;
    return sflag;

}
/**
 * @brief jceSSLClient::readIt function to read for fast mode in recieved data
 */
void jceSSLClient::readIt()
{
    QString p;
    do
    {
        p = readAll();
        packet.append(p);
    }while (p.size() > 0);

}
/**
 * @brief jceSSLClient::setConnected called when signaled with connected, calling the encryption function
 */
void jceSSLClient::setConnected()
{
    waitForEncrypted();
}
/**
 * @brief jceSSLClient::setDisconnected called when signaled with disconnected, setting flag to false
 */
void jceSSLClient::setDisconnected()
{
    flag = false;
}
/**
 * @brief jceSSLClient::setEncrypted called when signaled with encrypted. setting the buffer size and keeping alive.
 */
void jceSSLClient::setEncrypted()
{
    setReadBufferSize(10000);
    setSocketOption(QAbstractSocket::KeepAliveOption,1);
    if (state() == QAbstractSocket::SocketState::ConnectedState)
        flag = true;
    else
    {
        qWarning() << "jceSSLClient::setEncrypted(); Connection status didnt change!";
        flag = false;
    }
}
/**
 * @brief jceSSLClient::showIfErrorMsg message box to show the error occured according to socket
 *  if relevant, we will prompt a message box to informate the user
 *  otherwise, we will handle the error or ignore it.
 */
void jceSSLClient::showIfErrorMsg()
{
    QMessageBox msgBox;
    SocketError enumError = error();
    QString errorString;
    bool relevantError = false;
    switch (enumError)
    {
    case QAbstractSocket::SocketError::ConnectionRefusedError:
        errorString = QObject::tr("ConnectionRefusedError");
        relevantError = true;
        break;
    case QAbstractSocket::SocketError::RemoteHostClosedError:
        errorString = QObject::tr("RemoteHostClosedError");
        relevantError = true;
        break;
    case QAbstractSocket::SocketError::HostNotFoundError:
        errorString = QObject::tr("HostNotFoundError");
        relevantError = true;
        break;
    case QAbstractSocket::SocketError::SocketAccessError:
        errorString = QObject::tr("SocketAccessError");
        break;
    case QAbstractSocket::SocketError::SocketResourceError:
        errorString = QObject::tr("SocketResourceError");
        break;
    case QAbstractSocket::SocketError::SocketTimeoutError:
        errorString = QObject::tr("SocketTimeoutError");
        if (!isConnected())
            relevantError = true;
        break;
    case QAbstractSocket::SocketError::DatagramTooLargeError:
        errorString = QObject::tr("DatagramTooLargeError");
        break;
    case QAbstractSocket::SocketError::NetworkError:
        errorString = QObject::tr("NetworkError");
        relevantError = true;
        break;
    case QAbstractSocket::SocketError::AddressInUseError:
        errorString = QObject::tr("AddressInUseError");
        break;
    case QAbstractSocket::SocketError::SocketAddressNotAvailableError:
        errorString = QObject::tr("SocketAddressNotAvailableError");
        break;
    case QAbstractSocket::SocketError::UnsupportedSocketOperationError:
        errorString = QObject::tr("UnsupportedSocketOperationError");
        break;
    case QAbstractSocket::SocketError::ProxyAuthenticationRequiredError:
        errorString = QObject::tr("ProxyAuthenticationRequiredError");
        break;
    case QAbstractSocket::SocketError::SslHandshakeFailedError:
        errorString = QObject::tr("SslHandshakeFailedError");
        break;
    case QAbstractSocket::SocketError::ProxyConnectionRefusedError:
        errorString = QObject::tr("ProxyConnectionRefusedError");
        break;
    case QAbstractSocket::SocketError::UnfinishedSocketOperationError:
        errorString = QObject::tr("UnfinishedSocketOperationError");
        break;
    case QAbstractSocket::SocketError::ProxyConnectionClosedError:
        errorString = QObject::tr("ProxyConnectionClosedError");
        break;
    case QAbstractSocket::SocketError::ProxyConnectionTimeoutError:
        errorString = QObject::tr("ProxyConnectionTimeoutError");
        break;
    case QAbstractSocket::SocketError::ProxyNotFoundError:
        errorString = QObject::tr("ProxyNotFoundError");
        break;
    case QAbstractSocket::SocketError::ProxyProtocolError:
        errorString = QObject::tr("ProxyProtocolError");
        break;
    case QAbstractSocket::SocketError::OperationError:
        errorString = QObject::tr("OperationError");
        break;
    case QAbstractSocket::SocketError::SslInternalError:
        errorString = QObject::tr("SslInternalError");
        break;
    case QAbstractSocket::SocketError::SslInvalidUserDataError:
        errorString = QObject::tr("SslInvalidUserDataError");
        break;
    case QAbstractSocket::SocketError::TemporaryError:
        errorString = QObject::tr("TemporaryError");
        break;
    case QAbstractSocket::SocketError::UnknownSocketError:
        errorString = QObject::tr("UnknownSocketError");
        relevantError = true;
        break;
    }
    if (relevantError) //informative string to be shown
    {
        qDebug() << "jceSSLClient::showIfErrorMsg(); relevant error. msgbox popped";
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.setText(errorString);
        msgBox.exec();
    }

}
/**
 * @brief jceSSLClient::checkErrors this function exctuing when socket error has occured
 * @param a includes the error enum from QAbstractSocket::SocketError enum list
 */
void jceSSLClient::checkErrors(QAbstractSocket::SocketError a)
{
    qWarning() << "jceSSLClient::checkErrors; Var Error: " << a;
    qWarning() << "jceSSLClient::checkErrors; Error: " << this->errorString();
    showIfErrorMsg();
}

