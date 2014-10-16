#ifndef CALENDARSCHEDULE_H
#define CALENDARSCHEDULE_H

#include <QTableWidget>
#include <QHeaderView>
#include <QTextStream>

#include "calendarPage.h"


#define HOURS_BEGIN 8
#define HOURS_END 20
#define ACADEMIN_HOUR 45

/**
 * @brief The calendarSchedule class
 *
 * This class combining the courses schedule with each course into a table
 * setpage -> insertingIntoTable -> showing data
 *
 * Made By liran ben gida, LiranBG@gmail.com On 31/8/2014
 */
class calendarSchedule : public QTableWidget, public CalendarPage
{
    Q_OBJECT
public:
    calendarSchedule(QWidget *parent = 0);
    ~calendarSchedule() { clearTableItems(); }
    void setPage(QString html);
    void clearTableItems();

signals:

public slots:
private:

   void insertCourseIntoTable();

};

#endif // CALENDARSCHEDULE_H
