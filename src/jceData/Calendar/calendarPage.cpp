#include "calendarPage.h"

CalendarPage::CalendarPage(std::string html)
{
    courses = new std::list<calendarCourse*>();
    tempHtml = getString(html);
    tempHtml = tokenToLines(tempHtml);
    std::cout << "last one..." << std::endl;
    calendarListInit(tempHtml);
}

std::string CalendarPage::htmlToString()
{
    return tempHtml;
}

std::string CalendarPage::tokenToLines(std::string &textToPhrase)
{
    int ctr = 0;
    std::string temp = "";
    char *tok;
    char* textToTok = strdup(textToPhrase.c_str());
    tok = strtok(textToTok, "\n");
    while(tok != NULL)
    {
        //amount of data before the actual needed data and no empty lines
        if (strcmp(tok," \t ") != 0)
        {
            temp += tok;
            temp += "\n";
        }
        ctr++;
        tok = strtok(NULL, "\n");
    }
    return temp;
}

void CalendarPage::calendarListInit(std::string &linesTokinzedString)
{
    std::list<std::string> stringHolder;
    std::string temp;
    calendarCourse * cTemp = NULL;
    char* tok;
    char* textToTok = strdup(linesTokinzedString.c_str());
    tok = strtok(textToTok,"\n");
    while (tok != NULL)
    {
        temp = tok;
        stringHolder.push_back(temp);
        tok = strtok(NULL, "\n");
    }
    for(std::string temp: stringHolder)
    {
        cTemp = lineToCourse(temp);
        if (cTemp != NULL)
            courses->push_back(cTemp);
    }
}

calendarCourse *CalendarPage::lineToCourse(std::string line)
{

    calendarCourse *tempC = NULL;
    std::string templinearray[CALENDAR_COURSE_FIELDS];//[serial,name,type,lecturer,points,semesterhours,dayandhours,room]
    int serial;
    double points,semesterHours;
    std::string name,type, lecturer,dayAndHour,room;
    std::string tempS = "";
    std::string emptyTab = " ";
    int i = 0;
    char* tok;
    char* cLine = strdup(line.c_str());
    tok = strtok(cLine, "\t");
    while(tok != NULL)
    {

        tempS = tok;
        if (i>=1)
            templinearray[i-1] = tempS;
        i++;
        if (i > 8)
            break;
        tok=strtok(NULL, "\t");
    }
    if (templinearray[0] == "") //empty phrasing
        return NULL;
    for (int p = 0; p < 8; ++p)
    {
        std::cout << "index : " << p << " is: " << templinearray[p] << std::endl;
    }
    serial = stoi(templinearray[calendarCourse::CourseScheme::SERIAL]);
    name = templinearray[calendarCourse::CourseScheme::NAME];
    type = templinearray[calendarCourse::CourseScheme::TYPE];
    lecturer = templinearray[calendarCourse::CourseScheme::LECTURER];

    if (templinearray[calendarCourse::CourseScheme::POINTS].compare(" ") == 0)
        points = stod(templinearray[calendarCourse::CourseScheme::POINTS]);
    else
        points = 0;
    if (templinearray[calendarCourse::CourseScheme::SEM_HOURS].compare(" ") == 0)
        semesterHours = stod(templinearray[calendarCourse::CourseScheme::SEM_HOURS]);
    else
        semesterHours = 0;
    dayAndHour = templinearray[calendarCourse::CourseScheme::DAY_AND_HOURS];
    room = templinearray[calendarCourse::CourseScheme::ROOM];


    tempC = new calendarCourse(serial,name,type,lecturer,points,semesterHours,dayAndHour,room);

    return tempC;
}
