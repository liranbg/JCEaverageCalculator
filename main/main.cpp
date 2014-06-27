#include "mainscreen.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QTranslator translator;
    translator.load("jce_he");
    QApplication a(argc, argv);
    a.installTranslator(&translator);
    MainScreen w;
    w.show();

    return a.exec();
}
