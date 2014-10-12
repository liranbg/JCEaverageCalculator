#include "jcesslclient.h"

/**
 * @brief jceSSLClient::jceSSLClient  Constructer, setting the signals
 */
jceSSLClient::jceSSLClient(QProgressBar *progressbarPtr) : loggedIAndConnectedFlag(false), readingFlag(false),
        reConnectionFlag(false), networkConf(), packet(""), recieveLastPacket(false), packetSizeRecieved(0)
{
    this->progressBar = progressbarPtr;
    //setting signals
    connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(checkErrors(QAbstractSocket::SocketError)));
    connect(this,SIGNAL(connected()),this,SLOT(setConnected()));
    connect(this,SIGNAL(encrypted()),this,SLOT(setEncrypted()));
    connect(this,SIGNAL(disconnected()),this,SLOT(setDisconnected()));
    connect(&networkConf,SIGNAL(onlineStateChanged(bool)),this,SLOT(setOnlineState(bool)));

    //loop event will connect the server, and when it is connected, it will quit - but connection will be open
    connect(this, SIGNAL(encrypted()), &loginThreadLoop, SLOT(quit()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)),&loginThreadLoop,SLOT(quit()));

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


    if (reConnectionFlag) //reset reconnectiong flag
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

    loginThreadLoop.exec(); //starting connection, waiting to encryption and then it ends

    qDebug() << Q_FUNC_INFO <<  "returning the connection status: " << isConnected();
    if (reConnectionFlag)
    {
        reConnectionFlag = false;
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
    if (loginThreadLoop.isRunning())
    {
        qWarning() << Q_FUNC_INFO << "Killing connection thread";
        loginThreadLoop.exit();
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
    return ((loggedIAndConnectedFlag) && (tempFlag));
}
/**
 * @brief jceSSLClient::sendData  - given string, send it to server
 * @param str - string to send
 * @return true if succeed and byte were written
 */
bool jceSSLClient::sendData(QString str)
{
    bool sendDataFlag = false;
    int amount = 0;
    if (isConnected()) //if connected
    {
        amount = write(str.toStdString().c_str(),str.length());
        qDebug() << Q_FUNC_INFO << "lenght send: " << str.length() << "lenght recieved: " << amount;
        if (amount == -1)
        {
            qCritical() << Q_FUNC_INFO << "SendData ended with -1";
            sendDataFlag = false;
        }
        else if (waitForBytesWritten())
            sendDataFlag = true;
    }
    qDebug() << Q_FUNC_INFO <<  "Sending Data status is: " << sendDataFlag;
    return sendDataFlag;
}
/**
 * @brief jceSSLClient::recieveData - recieving data through threaded reading and mutex
 * @param str - string to fill with data.
 * @return true if packet has recieced the last packet -> true, otherwise ->false
 */
bool jceSSLClient::recieveData(QString *str)
{
    qDebug() << Q_FUNC_INFO <<  "Data receiving!";
    str->clear();
    packet = "";
    recieveLastPacket = false;
    packetSizeRecieved = 0; //counting packet size
    readingFlag = true; //to ignore timeout socket error

    timer.setSingleShot(true); //counting just once.
    timer.start(milisTimeOut); //if timer is timeout -> it means the connection takes long time

    connect(this, SIGNAL(readyRead()), this, SLOT(readIt())); //we have something to read
    connect(&timer, SIGNAL(timeout()), &readerLoop, SLOT(quit())); //if timer timeout > exiting event

    readerLoop.exec();

    disconnect(&timer, SIGNAL(timeout()), &readerLoop, SLOT(quit()));
    disconnect(this, SIGNAL(readyRead()), this, SLOT(readIt()));

    str->append(packet);
//    qDebug() << *str; //if you want to see the whole packet, unmark me

    qDebug() << Q_FUNC_INFO << "packet size: " << packetSizeRecieved << "received data lenght: " << str->length();
    qDebug() << Q_FUNC_INFO <<  "return with flag: " << recieveLastPacket;

    readingFlag = false; //finished reading session
    return recieveLastPacket; //we have the last packet

}
/**
 * @brief jceSSLClient::readIt
 * this method, called by a thread to read the bytes avilable by the remote server
 * each packet we append into the class private var 'packet' (mutexed)
 * if we recieve the last packet (see tags below) we set the timer of the calling function to 100msc
 *
 */
void jceSSLClient::readIt()
{
    int packSize = bytesAvailable();
    int doTimes=0;
    QByteArray tempPacket;

    do
    {
//        qDebug() << Q_FUNC_INFO << "packet size" << packSize;

        if (doTimes++ > 0) //for debbuging, checking thread looping times
            qDebug() << Q_FUNC_INFO << "do loop" << doTimes;

        waitForReadyRead(100);
        tempPacket = read(packSize);

        readerAppendingLocker.lock();
        packetSizeRecieved += packSize;
        packet.append(tempPacket);
        packet.append("\0");
        readerAppendingLocker.unlock();

        progressBar->setValue(this->progressBar->value() + 6);

        if (tempPacket.contains("Go_To_system_After_Login.htm") || tempPacket.contains("</html>"))
        {
            //we have the last packet. (uses only in login first step
            recieveLastPacket = true;
            timer.setInterval(200);
        }
        else
        {
            //just a packet with data
        }

    }while ((packSize = bytesAvailable()) > 0);
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
    loggedIAndConnectedFlag = false;
    if (reConnectionFlag)
        makeConnect();


}
/**
 * @brief jceSSLClient::setEncrypted called when signaled with encrypted. setting the buffer size and keeping alive.
 */
void jceSSLClient::setEncrypted()
{
    qDebug() << Q_FUNC_INFO << "connection has been ENCRYPTED";
    setReadBufferSize(packetSize);
    setSocketOption(QAbstractSocket::KeepAliveOption,true);
    loggedIAndConnectedFlag = true;
    if (!isConnected())
    {
        qWarning() << Q_FUNC_INFO <<  "Connection status didnt change! reseting flag to false";
        loggedIAndConnectedFlag = false;
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
            reConnectionFlag = true;
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
    else if (!readingFlag)
    {
        qWarning() << Q_FUNC_INFO << "isConnected?: " << isConnected() << "is timeout?" << timeout;
        qWarning() << Q_FUNC_INFO << "isOnline?: " << isConnectedToNetwork()  << "state is: " << state();
        qWarning() << Q_FUNC_INFO << "Error: " << errorString();

    }
    else
    {
        //timeout when reading
    }
    showIfErrorMsg();
}

/** written by KARAN BALKAR
 * @brief jceSSLClient::isConnectedToNetwork
 *
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

