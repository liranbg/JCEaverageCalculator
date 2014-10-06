#ifndef JCELOGIN_H
#define JCELOGIN_H

#include "./src/jceConnection/jcesslclient.h"
#include "./src/jceSettings/user.h"
#include "jceLoginHtmlScripts.h"

#include <QObject>
#include <QProgressBar>
#include <QString>


class jceLogin : public QObject
{
  Q_OBJECT
public:

  jceLogin(user* username,QProgressBar *progressbarPtr);
  ~jceLogin();

  enum jceStatus {
    JCE_NOT_CONNECTED,
    ERROR_ON_VALIDATION,
    ERROR_ON_VALIDATION_USER_BLOCKED,
    ERROR_ON_OPEN_SOCKET,
    ERROR_ON_SEND_REQUEST,
    ERROR_ON_GETTING_INFO,
    ERROR_ON_GETTING_PAGE,

    JCE_START_VALIDATING_PROGRESS,
    JCE_VALIDATION_PASSED,
    JCE_YOU_ARE_IN,
    JCE_PAGE_PASSED
  };


  int makeConnection();
  void closeAll();

  bool checkConnection() const;
  bool isLoginFlag() const;

  int getCalendar(int year, int semester);
  int getGrades(int fromYear, int toYear, int fromSemester, int toSemester);

  QString getPage();

private slots:
  void reValidation();
  void reMakeConnection();

signals:
  void connectionReadyAfterDisconnection();

private:

  int makeFirstVisit();
  int makeSecondVisit();
  bool checkValidation();
  void setLoginFlag(bool x);

  bool loginFlag;
  QString * recieverPage;
  user * jceA;
  jceSSLClient * JceConnector;
  QProgressBar *progressBar;


};

#endif // JCELOGIN_H
