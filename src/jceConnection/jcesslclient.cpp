#include "jcesslclient.h"

/**
 * @brief jceSSLClient::jceSSLClient  Constructer, setting the signals
 */
jceSSLClient::jceSSLClient(QProgressBar *progressbarPtr) : flag(false), packet(""),networkConf(), reConnection(false)
{
    this->progressBar = progressbarPtr;
    //setting signals
    connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(checkErrors(QAbstractSocket::SocketError)));
    connect(this,SIGNAL(connected()),this,SLOT(setConnected()));
    connect(this,SIGNAL(encrypted()),this,SLOT(setEncrypted()));
    connect(this,SIGNAL(disconnected()),this,SLOT(setDisconnected()));
    connect(&networkConf,SIGNAL(onlineStateChanged(bool)),this,SLOT(setOnlineState(bool)));

    //loop event will connect the server, and when it is connected, it will quit - but connection will be open
    connect(this, SIGNAL(encrypted()), &loop, SLOT(quit()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),&loop,SLOT(quit()));

}
/**
 * @brief jceSSLClient::makeConnect connecting to server with given port. using eventloop to assure it wont stuck the application.
 * @param server  - server to connect to
 * @param port    - the using port
 * @return        - true if connected, false otherwise
 */
bool jceSSLClient::makeConnect(QString server, int port)
{
    if (this->supportsSsl() == false)
    {
        qCritical() << Q_FUNC_INFO << "Couldnt load ssl package. ERROR";
        return false;
    }
    else
        qDebug() << Q_FUNC_INFO << "ssl loaded.";

    if (isConnectedToNetwork() == false)
    {
        qDebug() << Q_FUNC_INFO << "return false. not online";
        return false;
    }
    else
        qDebug() << Q_FUNC_INFO << "we're online";


    if (reConnection) //reset reconnectiong flag
    {
        qDebug() << Q_FUNC_INFO <<  "Making Reconnection";
    }
    else
        qDebug() << Q_FUNC_INFO <<  "Making Connection";

    if (isConnected())
    {
        qDebug() << Q_FUNC_INFO <<  "flag=true, calling makeDisconnect()";
        makeDiconnect();
    }

    qDebug() << Q_FUNC_INFO <<  "Connection to: " << server << "On Port: " << port;
    connectToHostEncrypted(server.toStdString().c_str(), port);

    loop.exec(); //starting connection, waiting to encryption and then it ends

    qDebug() << Q_FUNC_INFO <<  "returning the connection status: " << isConnected();
    if (reConnection)
    {
        reConnection = false;
        emit serverDisconnectedbyRemote();
    }
    return isConnected();

}
/**
 * @brief jceSSLClient::makeDiconnect from current server
 * @return the flag of connection status
 */
bool jceSSLClient::makeDiconnect()
{
    if (loop.isRunning())
    {
        qWarning() << Q_FUNC_INFO << "Killing connection thread";
        loop.exit();
    }
    qDebug() << Q_FUNC_INFO << "disconnecting from host and emitting disconnected()";
    this->disconnectFromHost(); //emits disconnected > setDisconnected
    setSocketState(QAbstractSocket::SocketState::UnconnectedState);
    return (!isConnected());

}

/**
 * @brief jceSSLClient::isConnected connection checking
 * @return regardless to state, it checks if there's a connection
 */
bool jceSSLClient::isConnected()
{
    bool tempFlag = false;
    //checking state before returning flag!
    if (state() == QAbstractSocket::SocketState::UnconnectedState)
    {
        tempFlag = false;
    }
    else if (state() == QAbstractSocket::SocketState::ClosingState)
    {
        tempFlag = false;
    }
    else if (state() == QAbstractSocket::SocketState::ConnectedState)
    {
        if (isConnectedToNetwork())
            tempFlag = true;
        else
        {
            this->setSocketState(QAbstractSocket::SocketState::UnconnectedState);
            tempFlag = false;
        }

    }
    if (!isConnectedToNetwork()) //no link, ethernet\wifi
        tempFlag = false;
    return ((flag) && (tempFlag));
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
    qDebug() << Q_FUNC_INFO <<  "Sending Data status is: " << sendDataFlag;
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
    qDebug() << Q_FUNC_INFO <<  "Data receiving!";
    packet = "";
    bool sflag = false;

    if (fast) //fast mode connection, good only for login step!!!!
    {
        qDebug() << Q_FUNC_INFO << "login step receiving";
        //loop will exit after first read packet.
        //meanwhile packet will gain data. good for small amount of data - fast connection!
        connect(this, SIGNAL(readyRead()), &readerLoop, SLOT(quit()));
        connect(this, SIGNAL(readyRead()), this, SLOT(readIt()));
        readerLoop.exec();
        disconnect(this, SIGNAL(readyRead()), &readerLoop, SLOT(quit()));
        disconnect(this, SIGNAL(readyRead()), this, SLOT(readIt()));
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "normal receiving";
        //loop will exit after timeout \ full data
        connect(this, SIGNAL(packetHasData()), &readerLoop, SLOT(quit()));

        timer.setSingleShot(true);
        connect(&timer, SIGNAL(timeout()), &readerLoop, SLOT(quit()));
        connect(this, SIGNAL(readyRead()), this, SLOT(readItAll()));
        timer.start(5000);
        readerLoop.exec();

    }
    str = packet;
    qDebug() << Q_FUNC_INFO <<  "received bytes: " << str.length() ;
    if (str.length() > 0)
        sflag = true;
    qDebug() << Q_FUNC_INFO <<  "return with flag: " << sflag;
    disconnect(this, SIGNAL(readyRead()), this, SLOT(readItAll()));
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
        this->progressBar->setValue(this->progressBar->value() + 6);
    }while (p.size() > 0);

}
void jceSSLClient::readItAll()
{
    QString p;
    do
    {
        p = "";
        p = read(bytesAvailable());
        if (p.contains("</tbody>") == true)
        {
            //we recieved the end of table. we can stop recieving
            timer.setInterval(1000);
        }
        this->progressBar->setValue(this->progressBar->value() + 6);
        packet.append(p);
        packet.append("\0");
    }while (p.size() > 0);
}

void jceSSLClient::setOnlineState(bool isOnline)
{
    qWarning() << Q_FUNC_INFO << "isOnline status change: " << isOnline;
    if (isOnline) //to be added later
    {
        qDebug() << Q_FUNC_INFO << "Online Statue has been changed. we are online";
        //we can add here auto reconnect if wifi\ethernet link has appear
        //will be added next version
    }
    else
    {
        qWarning() << Q_FUNC_INFO << "Online State has been changed. emitting NoInternetLink";
        this->makeDiconnect();
        emit noInternetLink();
    }

}
/**
 * @brief jceSSLClient::setConnected called when signaled with connected, calling the encryption function
 */
void jceSSLClient::setConnected()
{
    waitForEncrypted();
}
/**
 * @brief jceSSLClient::setDisconnected closing socket, updating state and setting flag to false
 */
void jceSSLClient::setDisconnected()
{
    qDebug() << Q_FUNC_INFO << "connection has been DISCONNECTED";
    this->setSocketState(QAbstractSocket::SocketState::UnconnectedState);
    packet.clear();
    flag = false;
    if (reConnection)
        makeConnect();


}
/**
 * @brief jceSSLClient::setEncrypted called when signaled with encrypted. setting the buffer size and keeping alive.
 */
void jceSSLClient::setEncrypted()
{
    qDebug() << Q_FUNC_INFO << "connection has been ENCRYPTED";
    setReadBufferSize(10000);
    setSocketOption(QAbstractSocket::KeepAliveOption,true);
    flag = true;
    if (!isConnected())
    {
        qWarning() << Q_FUNC_INFO <<  "Connection status didnt change! reseting flag to false";
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
    case QAbstractSocket::SocketError::ConnectionRefusedError: /**/
        errorString = QObject::tr("ConnectionRefusedError");
        //The connection was refused by the peer (or timed out).
        relevantError = true;
        break;
    case QAbstractSocket::SocketError::RemoteHostClosedError: /**/
        errorString = QObject::tr("RemoteHostClosedError");
        //The remote host closed the connection
        if (isConnectedToNetwork()) //we can reconnect
        {
            reConnection = true;
        }
        else
            relevantError = true;
        break;
    case QAbstractSocket::SocketError::HostNotFoundError: /**/
        errorString = QObject::tr("HostNotFoundError");
        //The host address was not found.
        relevantError = true;
        break;
    case QAbstractSocket::SocketError::SocketAccessError: /**/
        errorString = QObject::tr("SocketAccessError");
        //The socket operation failed because the application lacked the required privileges.
        break;
    case QAbstractSocket::SocketError::SocketTimeoutError: /**/
        errorString = QObject::tr("SocketTimeoutError");
        //The socket operation timed out.
        if (isConnected()); //ignore it if connected.
        else
            relevantError = true;
        break;
    case QAbstractSocket::SocketError::NetworkError: /**/
        errorString = QObject::tr("NetworkError");
        //An error occurred with the network (e.g., the network cable was accidentally plugged out).
        if (isConnectedToNetwork()) //we can reconnect
        {
        }
        else
            relevantError = true;
        break;
    case QAbstractSocket::SocketError::SslHandshakeFailedError: /**/
        errorString = QObject::tr("SslHandshakeFailedError");
        relevantError = true;
        break;
    case QAbstractSocket::SocketError::SslInternalError: /**/
        errorString = QObject::tr("SslInternalError");
        relevantError = true;
        break;
    case QAbstractSocket::SocketError::SslInvalidUserDataError: /**/
        errorString = QObject::tr("SslInvalidUserDataError");
        relevantError = true;
        break;
    case QAbstractSocket::SocketError::DatagramTooLargeError:  //not relevant to us
        errorString = QObject::tr("DatagramTooLargeError");
        break;
    case QAbstractSocket::SocketError::SocketResourceError: //not relevant to us
        break;
    case QAbstractSocket::SocketError::OperationError: //not relevant, except for debug
        errorString = QObject::tr("OperationError");
        break;
    case QAbstractSocket::SocketError::AddressInUseError: //not relevant to us
        errorString = QObject::tr("AddressInUseError");
        break;
    case QAbstractSocket::SocketError::SocketAddressNotAvailableError: //not relevant to us
        errorString = QObject::tr("SocketAddressNotAvailableError");
        break;
    case QAbstractSocket::SocketError::UnsupportedSocketOperationError: //for very old computers, not relevant to us
        errorString = QObject::tr("UnsupportedSocketOperationError");
        break;
    case QAbstractSocket::SocketError::ProxyAuthenticationRequiredError: //not relevant to us
        errorString = QObject::tr("ProxyAuthenticationRequiredError");
        break;
    case QAbstractSocket::SocketError::ProxyConnectionRefusedError: //not relevant to us
        errorString = QObject::tr("ProxyConnectionRefusedError");
        break;
    case QAbstractSocket::SocketError::UnfinishedSocketOperationError: //not relevant to us
        errorString = QObject::tr("UnfinishedSocketOperationError");
        break;
    case QAbstractSocket::SocketError::ProxyConnectionClosedError: //not relevant to us
        errorString = QObject::tr("ProxyConnectionClosedError");
        break;
    case QAbstractSocket::SocketError::ProxyConnectionTimeoutError: //not relevant to us
        errorString = QObject::tr("ProxyConnectionTimeoutError");
        break;
    case QAbstractSocket::SocketError::ProxyNotFoundError: //not relevant to us
        errorString = QObject::tr("ProxyNotFoundError");
        break;
    case QAbstractSocket::SocketError::ProxyProtocolError: //not relevant to us
        errorString = QObject::tr("ProxyProtocolError");
        break;
    case QAbstractSocket::SocketError::TemporaryError:  //not relevant to us
        errorString = QObject::tr("TemporaryError");
        break;
    case QAbstractSocket::SocketError::UnknownSocketError:  //not relevant, except for debug
        errorString = QObject::tr("UnknownSocketError");
        relevantError = true;
        break;
    }
    if (relevantError) //informative string to be shown
    {
        qDebug() << Q_FUNC_INFO << "relevant error.";
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
    //ignore this stupid error
    bool timeout = (a == QAbstractSocket::SocketError::SocketTimeoutError);
    if (!((isConnected()) && (timeout)))
    {
        qWarning() << Q_FUNC_INFO << "isOnline?: " << isConnectedToNetwork();
        qWarning() << Q_FUNC_INFO << "state is: " << state();
        qWarning() << Q_FUNC_INFO << "Var Error: " << a;
        qWarning() << Q_FUNC_INFO << "Error: " << errorString();
    }
    else
    {
        qDebug() << Q_FUNC_INFO << "isConnected?: " << isConnected() << "is timeout?" << timeout;
        qWarning() << Q_FUNC_INFO << "isOnline?: " << isConnectedToNetwork();
        qWarning() << Q_FUNC_INFO << "state is: " << state();
        qWarning() << Q_FUNC_INFO << "Var Error: " << a;
        qWarning() << Q_FUNC_INFO << "Error: " << errorString();
    }
    showIfErrorMsg();
}

/** written by KARAN BALKAR
 * @brief jceSSLClient::isConnectedToNetwork
 * @return
 */
bool jceSSLClient::isConnectedToNetwork(){

    QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    bool result = false;

    for (int i = 0; i < ifaces.count(); ++i)
    {
        QNetworkInterface iface = ifaces.at(i);

        if ( iface.flags().testFlag(QNetworkInterface::IsUp) && !iface.flags().testFlag(QNetworkInterface::IsLoopBack))
            for (int j=0; j < iface.addressEntries().count(); ++j)
                // got an interface which is up, and has an ip address
                if (result == false)
                    result = true;

    }
    return result;
}

