#ifndef LOGINHANDLER_H
#define LOGINHANDLER_H
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QMessageBox>
#include <QPixmap>

#include "src/jce/jcelogin.h"
#include "./src/data/savedata.h"

class loginHandler
{
public:
    loginHandler(user *ptr);
    void setPointers(QLabel *statusLabelPtr,QLineEdit *pswdEditPtr,QLineEdit *usrnmEditPtr);
    bool makeConnection();
    bool isLoggedInFlag();
    void setLoginFlag(bool flag);

    QString getCurrentPageContect();
    int makeGradeRequest();

    void makeDisconnectionRequest();

private:

    void popMessage(QString message, bool addInfo = true);

    bool logggedInFlag;
    jceLogin *jceLog;

    QLabel *statusLabelPtr;
    QLineEdit *pswdEditPtr;
    QLineEdit *usrnmEditPtr;
};

#endif // LOGINHANDLER_H
