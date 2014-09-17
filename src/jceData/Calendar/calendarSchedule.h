#ifndef CALENDARSCHEDULE_H
#define CALENDARSCHEDULE_H

#include <QTableWidget>
#include <QTextStream>
#include <iostream>

#include "calendarPage.h"
#include <iostream>
#include <QHeaderView>

#define HOURS_BEGIN 8
#define HOURS_END 20
#define ACADEMIN_HOUR 45

class calendarSchedule : public QTableWidget, public CalendarPage
{
    Q_OBJECT
public:
    calendarSchedule();
    ~calendarSchedule() { clearTableItems(); }
    void setPage(QString html);
    void clearTableItems();

signals:

public slots:
private:

   void insertCourseIntoTable();

};

#endif // CALENDARSCHEDULE_H
