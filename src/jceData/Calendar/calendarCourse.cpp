#include "calendarCourse.h"

calendarCourse::calendarCourse(int serial, std::string name, std::string type, std::string lecturer, double points,
                               double semesterHours, std::string dayAndHour,
                               std::string room) : Course(serial,name, type,points)
{
    this->lecturer = lecturer;
    this->semesterHours = semesterHours;
    this->room = room;
    setDayAndHour(dayAndHour);

}
void calendarCourse::setDayAndHour(std::string phrase)
{
    int ctr = 0;
    std::string temp = "";
    QTime timetemp;
    char *tok;
    char* textToTok = strdup(phrase.c_str());
    tok = strtok(textToTok, " -");
    while(tok != NULL)
    {
        temp = tok;
        switch (ctr)
        {
        case 0:
            setDay(temp);
            break;
        case 1:
            timetemp = QTime::fromString(QString::fromStdString(temp),"hh:mm");
            setHourBegin(timetemp.hour());
            setMinutesBegin(timetemp.minute());
            break;
        case 2:
            timetemp = QTime::fromString(QString::fromStdString(temp),"hh:mm");
            setHourEnd(timetemp.hour());
            setMinutesEnd(timetemp.minute());
            break;
        }

        ctr++;
        tok = strtok(NULL, " -");
    }
}

std::string calendarCourse::getLecturer() const
{
    return lecturer;
}

void calendarCourse::setLecturer(const std::string &value)
{
    lecturer = value;
}
double calendarCourse::getSemesterHours() const
{
    return semesterHours;
}

void calendarCourse::setSemesterHours(double value)
{
    semesterHours = value;
}
int calendarCourse::getHourBegin() const
{
    return hourBegin;
}

void calendarCourse::setHourBegin(int value)
{
    hourBegin = value;
}
int calendarCourse::getMinutesBegin() const
{
    return minutesBegin;
}

void calendarCourse::setMinutesBegin(int value)
{
    minutesBegin = value;
}
int calendarCourse::getHourEnd() const
{
    return hourEnd;
}

void calendarCourse::setHourEnd(int value)
{
    hourEnd = value;
}
int calendarCourse::getMinutesEnd() const
{
    return minutesEnd;
}

void calendarCourse::setMinutesEnd(int value)
{
    minutesEnd = value;
}

std::string calendarCourse::courseToString()
{
    std::string courseText = "";
    courseText += " " + std::to_string(this->getSerialNum());
    courseText += " " + this->getName();
    courseText += " " + this->getType();
    courseText += " " + this->lecturer;
    courseText += " " + std::to_string(this->getPoints());
    courseText += " " + std::to_string(this->semesterHours);
    courseText += " " + this->day;
    courseText += " " + std::to_string(this->hourBegin) + ":" + std::to_string(this->minutesBegin) + "-" + std::to_string(this->hourEnd) + ":" + std::to_string(this->minutesEnd);
    courseText += " " + this->room;
    courseText += "\n";
    return courseText;

}
std::string calendarCourse::getDay() const
{
    return day;
}

void calendarCourse::setDay(const std::string &value)
{
    day = value;
}

std::string calendarCourse::getRoom() const
{
    return room;
}

void calendarCourse::setRoom(const std::string &value)
{
    room = value;
}







