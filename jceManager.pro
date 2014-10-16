#-------------------------------------------------
#
# Project created by QtCreator 2014-05-31T19:49:42
#
#-------------------------------------------------

QT       += core gui network widgets printsupport

CONFIG += qt c++11
#CONFIG-=app_bundle

TARGET = jceManager
VERSION = 1.1.0
TEMPLATE = app
DEFINES += APP_VERSION=\\\"$$VERSION\\\"

win32 {
RC_FILE += winConf.rc
}
unix {
RC_FILE += appConfigure.rc
}

TRANSLATIONS = jce_en.ts \
                jce_he.ts

FORMS    += \
    main/mainscreen.ui \
    src/jceData/Grades/graph/gradegraph.ui \
    src/jceData/Calendar/Exams/examDialog.ui \
    src/jceData/Calendar/coursesSchedule/calendarDialog.ui \
    main/HelpBox/helpbox.ui

RESOURCES += \
    resources/connectionstatus.qrc

HEADERS += \
    main/CalendarTab/CalendarManager.h \
    main/CourseTab/coursestablemanager.h \
    main/LoginTab/loginhandler.h \
    main/mainscreen.h \
    src/appDatabase/savedata.h \
    src/jceConnection/jcesslclient.h \
    src/jceData/Grades/gradeCourse.h \
    src/jceData/Grades/gradePage.h \
    src/jceData/course.h \
    src/jceData/page.h \
    src/jceSettings/jcelogin.h \
    src/jceSettings/jceLoginHtmlScripts.h \
    src/jceSettings/user.h \
    src/jceData/CSV/csv_exporter.h \
    src/appDatabase/simplecrypt.h \
    src/appDatabase/jce_logger.h \
    src/jceData/Grades/graph/qcustomplot.h \
    src/jceData/Grades/graph/gradegraph.h \
    src/jceData/Calendar/Exams/examDialog.h \
    src/jceData/Calendar/Exams/calendarExam.h \
    src/jceData/Calendar/Exams/calendarExamCourse.h \
    src/jceData/Calendar/coursesSchedule/calendarDialog.h \
    src/jceData/Calendar/coursesSchedule/calendarPage.h \
    src/jceData/Calendar/coursesSchedule/calendarPageCourse.h \
    src/jceData/Calendar/coursesSchedule/calendarSchedule.h \
    main/jceWidgets/jcestatusbar.h \
    main/HelpBox/helpbox.h

SOURCES += \
    main/CalendarTab/CalendarManager.cpp \
    main/CourseTab/coursestablemanager.cpp \
    main/LoginTab/loginhandler.cpp \
    main/main.cpp \
    main/mainscreen.cpp \
    src/appDatabase/savedata.cpp \
    src/jceConnection/jcesslclient.cpp \
    src/jceData/Grades/gradeCourse.cpp \
    src/jceData/Grades/gradePage.cpp \
    src/jceData/page.cpp \
    src/jceSettings/jcelogin.cpp \
    src/jceSettings/user.cpp \
    src/jceData/CSV/csv_exporter.cpp \
    src/appDatabase/simplecrypt.cpp \
    src/appDatabase/jce_logger.cpp \
    src/jceData/Grades/graph/qcustomplot.cpp \
    src/jceData/Grades/graph/gradegraph.cpp \
    src/jceData/Calendar/Exams/examDialog.cpp \
    src/jceData/Calendar/Exams/calendarExam.cpp \
    src/jceData/Calendar/Exams/calendarExamCourse.cpp \
    src/jceData/Calendar/coursesSchedule/calendarDialog.cpp \
    src/jceData/Calendar/coursesSchedule/calendarPage.cpp \
    src/jceData/Calendar/coursesSchedule/calendarPageCourse.cpp \
    src/jceData/Calendar/coursesSchedule/calendarSchedule.cpp \
    main/jceWidgets/jcestatusbar.cpp \
    main/HelpBox/helpbox.cpp

