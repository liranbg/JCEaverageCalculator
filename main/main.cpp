#include "mainscreen.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>
#include "../src/appDatabase/savedata.h"


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    QString loco;
    SaveData data;
    loco = data.getLocal();
    if(loco == "default")
    {
        QString locale = QLocale::system().name();
        translator.load("jce_"+locale , a.applicationDirPath());
    }else if(loco == "he")
        translator.load("jce_he" , a.applicationDirPath());
    else
        translator.load("jce_en" , a.applicationDirPath());
    a.installTranslator(&translator);
    MainScreen w;
    w.show();

    return a.exec();
}
