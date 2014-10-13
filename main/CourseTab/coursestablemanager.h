#ifndef COURSESTABLEMANAGER_H
#define COURSESTABLEMANAGER_H


#include <QtGlobal>
#include <QHeaderView>
#include <QMessageBox>
#include <QTableWidget>
#include <QString>
#include <QMessageBox>

#include "./src/jceData/Grades/graph/gradegraph.h"
#include "./src/jceData/Grades/gradePage.h"
#include "./src/jceSettings/user.h"

class coursesTableManager
{
public:
    coursesTableManager(QTableWidget *, user *usrPtr);
    ~coursesTableManager();
    void insertJceCoursesIntoTable();
    void setCoursesList(QString &htmlPage);
    bool changes(QString change, int row, int col);
    void addRow(const gradeCourse * courseToAdd = 0);
    double getAvg();

    bool showGraph();

    void influnceCourseChanged(bool status);
    void clearTable();

private:
    gradegraph *graph;
    QTableWidget *courseTBL;
    GradePage *gp;
    user *us;

    gradeCourse * getCourseByRow(int row);
    bool isCourseAlreadyInserted(double courseID);
    bool isCourseInfluence(const gradeCourse *courseToCheck);

    void colorTheGrade(int rowIndex);
};

#endif // COURSESTABLEMANAGER_H
