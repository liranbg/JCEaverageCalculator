#ifndef COURSESTABLEMANAGER_H
#define COURSESTABLEMANAGER_H


#include <QApplication>
#include <QDesktopWidget>
#include <QtCore/QCoreApplication>
#include <QHeaderView>
#include <QMessageBox>
#include <QTableWidget>
#include <QString>
#include <QMessageBox>

#include <list>

#include "src/grades/Course.h"
#include "src/grades/GradePage.h"
#include "src/user.h"

class coursesTableManager
{
public:
    coursesTableManager(QTableWidget *, user *usrPtr);
    ~coursesTableManager();
    void insertJceCoursesIntoTable();
    void setCoursesList(std::string &htmlPage);
    bool changes(QString change, int row, int col);
    void addRow(const Course * courseToAdd = 0);
    double getAvg();

    void influnceCourseChanged(bool status);
    void clearTable();

private:
    QTableWidget *courseTBL;
    GradePage *gp;
    user *us;

    Course * getCourseByRow(int row);
    bool isCourseAlreadyInserted(double courseID);
    bool isCourseInfluence(const Course *courseToCheck);
};

#endif // COURSESTABLEMANAGER_H
