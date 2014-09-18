#ifndef JCESSLCLIENT_H
#define JCESSLCLIENT_H

#include <QObject>
#include <QSslSocket>
#include <QThread>
#include <QEventLoop>
#include <QMessageBox>
#include <QNetworkConfigurationManager>

#define milisTimeOut 4000

class jceSSLClient : QSslSocket
{
    Q_OBJECT
public:
    jceSSLClient();

    bool makeConnect(QString server = "yedion.jce.ac.il", int port = 443);
    bool makeDiconnect();
    bool isConnected();
    bool sendData(QString str);
    bool recieveData(QString &str, bool fast);
    void showIfErrorMsg();

private slots:
    void checkErrors(QAbstractSocket::SocketError a);
    void setConnected();
    void setEncrypted();
    void setDisconnected();
    void readIt();
    void setOnlineState(bool isOnline);

private:

    bool flag;
    QString packet;
    QEventLoop loop; //handle the connection as thread
    QNetworkConfigurationManager networkConf; //checking online

};

#endif // JCESSLCLIENT_H
