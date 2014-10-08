#ifndef LOGINHANDLER_H
#define LOGINHANDLER_H
#include <QObject>
#include <QString>
#include <QTextEdit>
#include <QLabel>
#include <QMessageBox>
#include <QPixmap>
#include <QStatusBar>
#include <QProgressBar>
#include <QPushButton>

#include "./src/jceSettings/jcelogin.h"
#include "./src/appDatabase/savedata.h"


class loginHandler : public QObject
{
    Q_OBJECT
public:
    loginHandler(user *ptr, QStatusBar *statusBarPtr, QPushButton *loginButtonPtr, QProgressBar *progressbarPtr);
    ~loginHandler()
    {
        delete iconButtomStatusLabel;
        delete jceLog;
    }

    bool login(QString username,QString password);
    void logout();
    void setIconConnectionStatus(jceLogin::jceStatus statusDescription);

    bool makeConnection();

    bool isLoggedInFlag();
    void setLoginFlag(bool flag);
    QString getCurrentPageContect();

    int makeGradeRequest(int fromYear, int toYear, int fromSemester, int toSemester);
    int makeCalendarRequest(int year,int semester);
    int makeExamsScheduleRequest(int year, int semester);

private slots:
    void readyAfterConnectionLost();

private:

    void popMessage(QString message, bool addInfo = true);

    bool logggedInFlag;
    jceLogin * jceLog;
    user * userPtr;

    QStatusBar *statusBar;
    QLabel *iconButtomStatusLabel;
    QPushButton *loginButtonPtr;
    QProgressBar *progressBar;

};

#endif // LOGINHANDLER_H
