#ifndef JCESSLCLIENT_H
#define JCESSLCLIENT_H

#include <QObject>
#include <QSslSocket>
#include <QThread>
#include <QEventLoop>

#include <iostream>
#include <string>
#define milisTimeOut 3500

class jceSSLClient : QSslSocket
{
    Q_OBJECT
public:
    jceSSLClient();

    bool makeConnect(std::string server,int port);
    bool isConnected();
    bool sendData(std::string str);
    bool recieveData(std::string &str, bool fast);
    bool makeDiconnect();

private slots:
    void checkErrors(QAbstractSocket::SocketError);
    void setConnected();
    void setEncrypted();
    void setDisconnected();
    void readIt();

private:

    bool flag;
    QString packet;

};

#endif // JCESSLCLIENT_H
