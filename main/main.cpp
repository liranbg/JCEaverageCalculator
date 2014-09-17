#include "mainscreen.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include "../src/appDatabase/savedata.h"
#include "../src/jce_logger.h"


int main(int argc, char *argv[])
{
#ifdef QT_DEBUG
  qDebug() << "Running a debug build";
#else
  qDebug() << "Running a release build";
  qInstallMessageHandler(jce_logger::customMessageHandler);
#endif

    qDebug() << "Start : JCE Manager Launched" << Q_FUNC_INFO;
    QApplication a(argc, argv);
    QTranslator translator;
    QString loco;
    SaveData data;
    loco = data.getLocal();
    if(loco == "default")
    {
        QString locale = QLocale::system().name();
        translator.load("jce_"+locale , a.applicationDirPath());
        qDebug() << "Local : Default Local Loaded";
    }else if(loco == "he"){
        translator.load("jce_he" , a.applicationDirPath());
        qDebug() << "Local : Hebrew Local Loaded";
    }else{
        translator.load("jce_en" , a.applicationDirPath());
        qDebug() << "Local : English Local Loaded";
    }
    a.installTranslator(&translator);
    MainScreen w;
    w.show();

    int returnCode =  a.exec();
    if(returnCode == 0)
        qDebug() << "End : JCE Manager Ended Successfully With A Return Code: " << returnCode;
    else
        qCritical() << "End : JCE Manager Ended Unusccessfully With A Return Code: " << returnCode;
    return returnCode;
}
