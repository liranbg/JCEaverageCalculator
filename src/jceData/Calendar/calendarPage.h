#ifndef CALENDARPAGE_H
#define CALENDARPAGE_H

#include "../page.h"
#include "calendarCourse.h"
#include <list>
#include <string.h> //strlen and strtok to phrase the html file
#include <ctype.h> //checks if character is numeric

class CalendarPage : public Page
{
public:

    std::string htmlToString();
    std::list<calendarCourse*>* getCourses() { return courses; }


protected:
    virtual void setPage(std::string html);
CalendarPage() { }


private:
    std::string tokenToLines(std::string& textToPhrase);
    void calendarListInit(std::string& linesTokinzedString);
    calendarCourse* lineToCourse(std::string line);

    std::string tempHtml;
    std::list<calendarCourse*>* courses;


};

#endif // CALENDARPAGE_H
