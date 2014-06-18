#ifndef QTSSLSOCKET_H
#define QTSSLSOCKET_H
#include <QObject>

#include <QSslSocket>
#include <QtNetwork/QSslCipher>
#include <QString>
#include <iostream>
#include <string>
#define milisTimeOut 3000

class qtsslsocket : public QObject
{
    Q_OBJECT

public:
    qtsslsocket();
    ~qtsslsocket();

    bool makeConnect(std::string server,int port);
    bool isCon();
    bool send(std::string str);
    bool recieve(std::string &str);


private:

    QSslSocket *socket;
    bool flag;
};

#endif // QTSSLSOCKET_H
