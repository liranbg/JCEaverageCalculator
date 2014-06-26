#-------------------------------------------------
#
# Project created by QtCreator 2014-05-31T19:49:42
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = jceGrade
TEMPLATE = app
RC_FILE = appConfigure.rc
CONFIG += c++11 static

SOURCES +=     main/coursestablemanager.cpp \
    main/loginhandler.cpp \
    main/main.cpp \
    main/mainscreen.cpp \
    src/connection/jcesslclient.cpp \
    src/user.cpp \
    src/grades/Course.cpp \
    src/grades/GradePage.cpp \
    src/grades/Page.cpp \
    src/jce/jcedate.cpp \
    src/jce/jcelogin.cpp \
    src/data/savedata.cpp \




HEADERS  +=     main/coursestablemanager.h \
    main/loginhandler.h \
    main/mainscreen.h \
    src/connection/jcesslclient.h \
    src/user.h \
    src/grades/Course.h \
    src/grades/GradePage.h \
    src/grades/Page.h \
    src/jce/jcedate.h \
    src/jce/jcelogin.h \
    src/jce/jceLoginHtmlScripts.h \
    src/data/savedata.h \




FORMS    += \
    main/mainscreen.ui

OTHER_FILES +=

RESOURCES += \
    resources/connectionstatus.qrc
