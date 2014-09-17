#ifndef JCESSLCLIENT_H
#define JCESSLCLIENT_H

#include <QObject>
#include <QSslSocket>
#include <QThread>
#include <QEventLoop>
#include <QMessageBox>

#include <iostream>
#include <string>
#define milisTimeOut 3500

class jceSSLClient : QSslSocket
{
    Q_OBJECT
public:
    jceSSLClient();

    bool makeConnect(QString server, int port);
    bool isConnected();
    bool sendData(QString str);
    bool recieveData(QString &str, bool fast);
    bool makeDiconnect();
    void showIfErrorMsg();

private slots:
    void checkErrors(QAbstractSocket::SocketError a);
    void setConnected();
    void setEncrypted();
    void setDisconnected();
    void readIt();

private:

    bool flag;
    QString packet;
    QEventLoop loop; //handle the connection as thread

};

#endif // JCESSLCLIENT_H
