#include "mainscreen.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include "../src/appDatabase/savedata.h"
#include "../src/appDatabase/jce_logger.h"

//TODO: Project todo list
//update translation, update site spelling, release notes, update help
int main(int argc, char *argv[])
{
#ifdef QT_DEBUG // Incase QtCreator is in Debug mode all qDebug messages will go to terminal
  qDebug() << Q_FUNC_INFO << "Running a debug build";
#else          // If QtCreator is on  Release mode , qDebug messages will be logged in a log file.
//  qDebug() << "Running a release build";
  qInstallMessageHandler(jce_logger::customMessageHandler);
#endif

    qDebug() << Q_FUNC_INFO << "Start : JCE Manager Launched";

    QApplication a(argc, argv);
    QTranslator translator;
    QString loco;
    SaveData data;
    loco = data.getLocal();
    //Loading Local (From Settings file (SaveData.cpp)
    if(loco == "en")
    {
        translator.load("jce_" + loco , a.applicationDirPath());
        qDebug() << Q_FUNC_INFO << "Locale : English Local Loaded";
    }else if(loco == "he"){
        translator.load("jce_" + loco , a.applicationDirPath());
        qDebug() << Q_FUNC_INFO << "Local : Hebrew Local Loaded";
    }else{
        translator.load("jce_en" , a.applicationDirPath());
        data.reset();
        qCritical() << Q_FUNC_INFO << "save data corrupted, reset file.";
    }
    a.installTranslator(&translator); //Setting local
    a.setApplicationVersion(APP_VERSION);
    MainScreen w;
    w.show();

    //Getting the exit code from QApplication. for debug reasons
    int returnCode =  a.exec();
    if(returnCode == 0)
        qDebug() << Q_FUNC_INFO << "End : JCE Manager Ended Successfully With A Return Code: " << returnCode;
    else
        qCritical() << Q_FUNC_INFO << "End : JCE Manager Ended Unusccessfully With A Return Code: " << returnCode;
    return returnCode;
}
