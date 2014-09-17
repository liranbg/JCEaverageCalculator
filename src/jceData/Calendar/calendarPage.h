#ifndef CALENDARPAGE_H
#define CALENDARPAGE_H

#include "../page.h"
#include "calendarCourse.h"
#include <list>
//#include <string.h> //strlen and strtok to phrase the html file
//#include <ctype.h> //checks if character is numeric

class CalendarPage : public Page
{
public:

    QString htmlToString();
    std::list<calendarCourse*>* getCourses() { return courses; }


protected:
    virtual void setPage(QString html);
    CalendarPage() { }


private:
    QString tokenToLines(QString &textToParse);
    void calendarListInit(QString &linesTokinzedString);
    calendarCourse* lineToCourse(QString line);

    QString tempHtml;
    std::list<calendarCourse*>* courses;


};

#endif // CALENDARPAGE_H
