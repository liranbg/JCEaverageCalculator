#ifndef CALENDARPAGE_H
#define CALENDARPAGE_H

#include "../page.h"
#include "calendarCourse.h"
#include <list>

#define ROOM_DEFAULT_STRING "nullRoom"
#define LECTURER_DEFAULT_STRING "nullLecturer"

class CalendarPage : public Page
{
public:

    QString htmlToString();
    std::list<calendarCourse*>* getCourses() { return courses; }


protected:
    virtual void setPage(QString html);
    CalendarPage() { courses = NULL; }


private:
    QString tokenToLines(QString &textToParse);
    void calendarListInit(QString &linesTokinzedString);
    calendarCourse* lineToCourse(QString line);

    QString tempHtml;
    std::list<calendarCourse*>* courses;


};

#endif // CALENDARPAGE_H
