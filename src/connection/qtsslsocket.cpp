#include "qtsslsocket.h"


bool qtsslsocket::makeConnect(std::string server,int  port)
{
    if (isCon())
        socket->abort();


    socket->connectToHostEncrypted(server.c_str(), port);
    socket->waitForEncrypted();

    return true; //return true/false upon isCon function
}

qtsslsocket::qtsslsocket() :  flag(false)
{
    socket = new QSslSocket();
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(checkErrors(QAbstractSocket::SocketError)));
    connect(socket,SIGNAL(connected()),this,SLOT(setConnected()));
    connect(socket,SIGNAL(encrypted()),this,SLOT(setEncrypted()));
    this->flag = true;

}


qtsslsocket::~qtsslsocket() {
    socket->close();
    socket = NULL;
}

bool qtsslsocket::isCon()
{
    return ((flag) && (this->socket != NULL));

}
//need to fix the method
bool qtsslsocket::send(std::string str)
{
    //    int status;
    bool flag = isCon();
    if (flag) //if connected
    {
        socket->write(str.c_str(),str.length());
        while (socket->waitForBytesWritten());
    }
    return flag;
}
bool qtsslsocket::recieve(std::string &str)
{
    bool flag = false;
    QString s = "";
    while (socket->waitForReadyRead(milisTimeOut))
        s.append(socket->readAll());

    str = s.toStdString();
    if (s.size() > 0)
        flag = true;

    return flag;
}

bool qtsslsocket::makeDiconnect()
{
    this->socket->abort();
    if (socket->isOpen())
        return false;
    return true;
}

void qtsslsocket::checkErrors(QAbstractSocket::SocketError info)
{
}

void qtsslsocket::setConnected()
{
//    socket->waitForEncrypted();

}

void qtsslsocket::setEncrypted()
{

//    this->flag = true;
}


