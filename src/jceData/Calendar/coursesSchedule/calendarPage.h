#ifndef CALENDARPAGE_H
#define CALENDARPAGE_H

#include "../../page.h"
#include "calendarPageCourse.h"
#include <list>

#define ROOM_DEFAULT_STRING "nullRoom"
#define LECTURER_DEFAULT_STRING "nullLecturer"

class CalendarPage : public Page
{
public:

    std::list<calendarCourse*>* getCourses() { return courses; }


protected:

    virtual void setPage(QString html);
    CalendarPage() { courses = NULL; }


private:

    void calendarListInit(QString &linesTokinzedString);
    calendarCourse * lineToCourse(QString line);

    QString tempHtml;
    std::list<calendarCourse*>* courses;

};

#endif // CALENDARPAGE_H
