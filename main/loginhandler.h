#ifndef LOGINHANDLER_H
#define LOGINHANDLER_H
#include <QString>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPixmap>

#include "src/jce/jcelogin.h"
#include "./src/data/savedata.h"

class loginHandler
{
public:
    loginHandler();
    void setPointers(jceLogin *ptr,QLabel *statusLabelPtr,QLineEdit *pswdEditPtr,QLineEdit *usrnmEditPtr);
    bool makeConnection();

private:

    void popMessage(QString message, bool addInfo = true);


    jceLogin *jceLog;

    QLabel *statusLabelPtr;
    QLineEdit *pswdEditPtr;
    QLineEdit *usrnmEditPtr;
};

#endif // LOGINHANDLER_H
