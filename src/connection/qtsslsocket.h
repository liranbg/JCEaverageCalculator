#ifndef QTSSLSOCKET_H
#define QTSSLSOCKET_H
#include <QObject>

#include <QSslSocket>
#include <QtNetwork/QSslCipher>
#include <QString>
#include <QProgressBar>
#include <iostream>
#include <string>
#define milisTimeOut 3000

class qtsslsocket : public QObject
{
    Q_OBJECT

public:
    qtsslsocket(std::string server,int  port);
    ~qtsslsocket() {
        disconnect(this->socket,SIGNAL(encryptedBytesWritten(qint64)),this,SLOT(on_read()));
        socket->close();
        socket = NULL;
        pb = NULL;

    }
    bool isCon();
    bool send(std::string str);
    bool recieve(std::string &str);
    void setProgressBar(QProgressBar *ptr);

private slots:
    void on_read();

private:
    QSslSocket *socket;
    QProgressBar *pb;
    bool flag;
};

#endif // QTSSLSOCKET_H
