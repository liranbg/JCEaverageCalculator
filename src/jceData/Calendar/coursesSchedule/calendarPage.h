#ifndef CALENDARPAGE_H
#define CALENDARPAGE_H

#include "../../page.h"
#include "calendarPageCourse.h"
#include <QList>

#define ROOM_DEFAULT_STRING "nullRoom"
#define LECTURER_DEFAULT_STRING "nullLecturer"

/**
 * @brief The CalendarPage class
 *
 * This class generating html string to a list of calendarCourses
 * Each item in a list is a course with its information (hour, day, name, serial and etc)
 *
 * Made By liran ben gida, LiranBG@gmail.com On 31/8/2014
 */
class CalendarPage : public Page
{
public:

    QList<calendarCourse*> getCourses() { return courses; }


protected:

    virtual void setPage(QString html);
    CalendarPage() {  }


private:

    void calendarListInit(QString &linesTokinzedString);
    calendarCourse * lineToCourse(QString line);

    QString tempHtml;
    QList<calendarCourse*> courses;

};

#endif // CALENDARPAGE_H
