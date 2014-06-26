#include "jcesslclient.h"

jceSSLClient::jceSSLClient() : flag(false), packet("")
{
    connect(this,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(checkErrors(QAbstractSocket::SocketError)));
    connect(this,SIGNAL(connected()),this,SLOT(setConnected()));
    connect(this,SIGNAL(encrypted()),this,SLOT(setEncrypted()));
    connect(this,SIGNAL(disconnected()),this,SLOT(setDisconnected()));

}

bool jceSSLClient::makeConnect(std::string server, int port)
{
    QEventLoop loop;
    QObject::connect(this, SIGNAL(encrypted()), &loop, SLOT(quit()));
    if (isConnected())
        abort();
    if (isOpen())
        abort();

    connectToHostEncrypted(server.c_str(), port);
    loop.exec();
    return isConnected();

}
bool jceSSLClient::makeDiconnect()
{

    abort();
    if (state() == QAbstractSocket::SocketState::ConnectedState)
    {
        flag = false;
    }
    else
        flag = true;

    return flag;
}


bool jceSSLClient::isConnected()
{

    return flag;
}

bool jceSSLClient::sendData(std::string str)
{
    if (isConnected()) //if connected
    {
        write(str.c_str(),str.length());
        while (waitForBytesWritten());
        return true;
    }
    return false;
}
/**
 * @brief jceSSLClient::recieveData
 * @param str   this variable will store the recieved data
 * @param fast  true for LOGIN ONLY, false to retrieve all data
 * @return  true if recieved data bigger than zero
 */
bool jceSSLClient::recieveData(std::string &str, bool fast)
{
    packet = "";
    bool sflag = false;

    if (fast) //fast connection, good for login only!!
    {
        QEventLoop loop;
        connect(this, SIGNAL(readyRead()), &loop, SLOT(quit()));
        connect(this, SIGNAL(readyRead()), this, SLOT(readIt()));
        loop.exec();
        disconnect(this, SIGNAL(readyRead()), &loop, SLOT(quit()));
        disconnect(this, SIGNAL(readyRead()), this, SLOT(readIt()));
    }
    else
    {
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

    str = packet.toStdString();
    if (str.size() > 0)
        sflag = true;

    return sflag;

}
void jceSSLClient::readIt()
{
    QString p;
    do
    {
        p = readAll();
        packet.append(p);
    }while (p.size() > 0);



}
void jceSSLClient::setConnected()
{
    waitForEncrypted();
}

void jceSSLClient::setEncrypted()
{
    setReadBufferSize(10000);
    setSocketOption(QAbstractSocket::KeepAliveOption,1);
    flag = true;
}

void jceSSLClient::setDisconnected()
{
    abort();
    flag = false;
}


void jceSSLClient::checkErrors(QAbstractSocket::SocketError)
{

}

