#ifndef JCESSLCLIENT_H
#define JCESSLCLIENT_H

#include <QObject>
#include <QSslSocket>
#include <QThread>
#include <QEventLoop>
#include <QTimer>
#include <QMutex>
#include <QMessageBox>
#include <QNetworkConfigurationManager>
#include <QtNetwork/QNetworkInterface>

#include "./main/jceWidgets/jcestatusbar.h"

#define packetSize 4096 //4k
#define milisTimeOut 5000 //4 seconds

class jceSSLClient : public QSslSocket
{
  Q_OBJECT
public:
  jceSSLClient(jceStatusBar *progressbarPtr);

  bool makeConnect(QString server = "yedion.jce.ac.il", int port = 443);
  bool makeDiconnect();
  bool isConnected();
  bool sendData(QString str);
  bool recieveData(QString *str);
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

  bool loggedIAndConnectedFlag;
  bool readingFlag;
  bool reConnectionFlag; //used for remote host disconnecting

  QNetworkConfigurationManager networkConf; //checking if online

  QString packet;
  bool recieveLastPacket;
  int packetSizeRecieved;

  QEventLoop loginThreadLoop; //handle the connection as thread
  QEventLoop readerLoop;

  QMutex readerAppendingLocker; //locking packet when appending
  QTimer timer; //uses to check if reading has reached its timeout



  jceStatusBar *progressBar; //progressbar pointer

};

#endif // JCESSLCLIENT_H
