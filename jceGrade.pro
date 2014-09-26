#-------------------------------------------------
#
# Project created by QtCreator 2014-05-31T19:49:42
#
#-------------------------------------------------

QT       += core gui network widgets

CONFIG += qt c++11

TARGET = jceManager
TEMPLATE = app
VERSION = 1.0.0

RC_FILE = appConfigure.rc
TRANSLATIONS = jce_en.ts \
                jce_he.ts

FORMS    += \
    main/mainscreen.ui \
    src/jceData/Calendar/calendarDialog.ui

RESOURCES += \
    resources/connectionstatus.qrc

HEADERS += \
    main/CalendarTab/CalendarManager.h \
    main/CourseTab/coursestablemanager.h \
    main/LoginTab/loginhandler.h \
    main/mainscreen.h \
    src/appDatabase/savedata.h \
    src/jceConnection/jcesslclient.h \
    src/jceData/Calendar/calendarPage.h \
    src/jceData/Grades/gradeCourse.h \
    src/jceData/Grades/gradePage.h \
    src/jceData/course.h \
    src/jceData/page.h \
    src/jceSettings/jcelogin.h \
    src/jceSettings/jceLoginHtmlScripts.h \
    src/jceSettings/user.h \
    src/jceData/Calendar/calendarCourse.h \
    src/jceData/Calendar/calendarSchedule.h \
    src/jceData/CSV/csv_exporter.h \
    src/appDatabase/simplecrypt.h \
    src/jceData/Calendar/calendarDialog.h \
    src/appDatabase/jce_logger.h

SOURCES += \
    main/CalendarTab/CalendarManager.cpp \
    main/CourseTab/coursestablemanager.cpp \
    main/LoginTab/loginhandler.cpp \
    main/main.cpp \
    main/mainscreen.cpp \
    src/appDatabase/savedata.cpp \
    src/jceConnection/jcesslclient.cpp \
    src/jceData/Calendar/calendarPage.cpp \
    src/jceData/Grades/gradeCourse.cpp \
    src/jceData/Grades/gradePage.cpp \
    src/jceData/page.cpp \
    src/jceSettings/jcelogin.cpp \
    src/jceSettings/user.cpp \
    src/jceData/Calendar/calendarCourse.cpp \
    src/jceData/Calendar/calendarSchedule.cpp \
    src/jceData/CSV/csv_exporter.cpp \
    src/appDatabase/simplecrypt.cpp \
    src/jceData/Calendar/calendarDialog.cpp \
    src/appDatabase/jce_logger.cpp
