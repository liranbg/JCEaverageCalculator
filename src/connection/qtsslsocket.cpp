#include "qtsslsocket.h"

qtsslsocket::qtsslsocket(std::string server,int  port) :  flag(false)
{
    socket = new QSslSocket();
    socket->connectToHostEncrypted(server.c_str(), port);
    if (socket->waitForEncrypted())
        flag = true;

}

bool qtsslsocket::isCon()
{
    return flag;

}

bool qtsslsocket::send(std::string str)
{
    bool flag = true;
    this->socket->write(str.c_str());

    return flag;
}

bool qtsslsocket::recieve(std::string &str)
{
    bool flag = false;
    QString s = "";
    while (socket->waitForReadyRead(milisTimeOut))
        s.append((socket->readAll().data()));
    str = s.toStdString();
    if (s.size() > 0)
        flag = true;
    return flag;
}

void qtsslsocket::setProgressBar(QProgressBar *ptr)
{
    if (ptr != NULL)
    {
        this->pb=ptr;
        connect(this->socket,SIGNAL(encryptedBytesWritten(qint64)),this,SLOT(on_read()));
        //connect(this->socket,SIGNAL(readChannelFinished()),this,SLOT(on_read()));
    }

}

void qtsslsocket::on_read()
{
    if (this->pb != NULL)
    {
        if (pb->value() <= 90)
            pb->setValue(pb->value() + 10);
    }
}
