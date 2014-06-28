#include "mainscreen.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    QString locale = QLocale::system().name();
    translator.load("jce_"+locale , a.applicationDirPath());
    a.installTranslator(&translator);
    MainScreen w;
    w.show();

    return a.exec();
}
