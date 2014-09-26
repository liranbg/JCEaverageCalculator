#ifndef JCESSLCLIENT_H
#define JCESSLCLIENT_H

#include <QObject>
#include <QSslSocket>
#include <QThread>
#include <QEventLoop>
#include <QMessageBox>
#include <QNetworkConfigurationManager>
#include <QtNetwork/QNetworkInterface>

#define milisTimeOut 4000

class jceSSLClient : public QSslSocket
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

signals:
    void serverDisconnectedbyRemote();
    void noInternetLink();
    void socketDisconnected();

private slots:
    void checkErrors(QAbstractSocket::SocketError a);
    void setConnected();
    void setEncrypted();
    void setDisconnected();
    void readIt();
    void setOnlineState(bool isOnline);

private:
    bool isConnectedToNetwork(); //checking if online
    bool flag;
    QString packet;
    QEventLoop loop; //handle the connection as thread
    QNetworkConfigurationManager networkConf; //checking if online
    bool reConnection; //used for remote host disconnecting

};

#endif // JCESSLCLIENT_H
