#include "qtsslsocket.h"


bool qtsslsocket::makeConnect(std::string server,int  port)
{
    if (this->socket != NULL) //already connected?
    {
        socket->close();
        socket = NULL;
    }

    if (this->socket == NULL) //if was connected, we deleted it and remake it
        socket = new QSslSocket();


    if (this->socket != NULL) //now we will connect it to host
    {
        socket->connectToHostEncrypted(server.c_str(), port);
        if (socket->waitForEncrypted()) //waiting for encryption
            flag = true;
    }

    return isCon(); //return true/false upon isCon function
}

qtsslsocket::qtsslsocket() :  flag(false)
{
    socket = new QSslSocket();
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
