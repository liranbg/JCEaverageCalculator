#include "mainscreen.h"
#include <QApplication>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator translator;
    bool is;
    is = translator.load("jce_he.qm", a.applicationDirPath());
    qDebug() << " =============== " << is ;
    a.installTranslator(&translator);
    MainScreen w;
    w.show();

    return a.exec();
}
