#include "calendarCourse.h"

calendarCourse::calendarCourse(int serial, QString name, QString type, QString lecturer, double points,
                               double semesterHours, QString dayAndHour,
                               QString room) : Course(serial,name, type,points)
{
    this->lecturer = lecturer;
    this->semesterHours = semesterHours;
    this->room = room;
    setDayAndHour(dayAndHour);

}
void calendarCourse::setDayAndHour(QString phrase)
{
    int ctr = 0;
    QString temp = "";
    QTime timetemp;
    char *tok;
    char* textToTok = strdup(phrase.toStdString().c_str());
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
            timetemp = QTime::fromString(temp,"hh:mm");
            setHourBegin(timetemp.hour());
            setMinutesBegin(timetemp.minute());
            break;
        case 2:
            timetemp = QTime::fromString(temp,"hh:mm");
            setHourEnd(timetemp.hour());
            setMinutesEnd(timetemp.minute());
            break;
        }

        ctr++;
        tok = strtok(NULL, " -");
    }
}

QString calendarCourse::getLecturer() const
{
    return lecturer;
}

void calendarCourse::setLecturer(const QString &value)
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

QString calendarCourse::courseToString()
{
    QString courseText = "";
    courseText += " " + QString::number(this->getSerialNum());
    courseText += " " + this->getName();
    courseText += " " + this->getType();
    courseText += " " + this->lecturer;
    courseText += " " + QString::number(this->getPoints());
    courseText += " " + QString::number(this->semesterHours);
    courseText += " " + QString::number(this->day);
    courseText += " " + QString::number(this->hourBegin) + ":" + QString::number(this->minutesBegin) + "-" + QString::number(this->hourEnd) + ":" + QString::number(this->minutesEnd);
    courseText += " " + this->room;
    courseText += "\n";
    return courseText;

}
int calendarCourse::getDay() const
{
    return day;
}

void calendarCourse::setDay(const QString &value)
{
    std::string dayTemp = value.toStdString().substr(0,2); //recieving two characters respresting days
    if (dayTemp.compare("\u05D0") == 0) //alef
        day = 1;
    else if (dayTemp.compare("\u05D1") == 0) //bet
        day = 2;
    else if (dayTemp.compare("\u05D2") == 0) //gimel
        day = 3;
    else if (dayTemp.compare("\u05D3") == 0) //dalet
        day = 4;
    else if (dayTemp.compare("\u05D4") == 0) //hey
        day = 5;
    else if (dayTemp.compare("\u05D5") == 0) //vav
        day = 6;
    else
        day= -1;
}

QString calendarCourse::getRoom() const
{
    return room;
}

void calendarCourse::setRoom(const QString &value)
{
    room = value;
}







