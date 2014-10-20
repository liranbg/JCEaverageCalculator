#include "calendarPageCourse.h"

calendarCourse::calendarCourse(int serial, QString name, QString type, QString lecturer, double points,
                               double semesterHours, QString dayAndHour,
                               QString room)  : Course(serial,name, type)
{
    this->points = points;
    this->lecturer = lecturer;
    this->semesterHours = semesterHours;
    this->room = room;
    setDayAndHour(dayAndHour);
}
/**
 * @brief calendarCourse::setDayAndHour
 * given a string of time and day - parsing it into day, hour it begins and hour it ends seperated
 * @param parse -
 */
void calendarCourse::setDayAndHour(QString parse)
{
        int ctr = 0;
        QString temp = "";
        QTime timetemp;
        char *tok;
        char* textToTok = strdup(parse.toStdString().c_str());
        tok = strtok(textToTok, " -");
        while(tok != NULL)
        {
            temp = tok;
            switch (ctr)
            {
            case 0: //day
                setDay(temp);
                break;
            case 1: //hour it begins
                timetemp = QTime::fromString(temp,"hh:mm");
                setHourBegin(timetemp.hour());
                setMinutesBegin(timetemp.minute());
                break;
            case 2: //hour it ends
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
int calendarCourse::getDay() const
{
    return day;
}
/**
 * @brief calendarCourse::setDay
 * translating a day that written with hebrew unicode character to an integer
 * @param value     -   contains the day in unicode
 */
void calendarCourse::setDay(const QString &value)
{
    QString dayTemp = value.mid(0,1); //recieving two characters respresting days
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
      {
        qDebug() << "error on parsing day!";
        day= -1;
      }
}
QString calendarCourse::getRoom() const
{
    return room;
}

void calendarCourse::setRoom(const QString &value)
{
    room = value;
}
double calendarCourse::getPoints() const
{
    return points;
}

void calendarCourse::setPoints(double value)
{
    points = value;
}

double points;
QString lecturer;
double semesterHours;
int day;
int hourBegin;
int minutesBegin;
int hourEnd;
int minutesEnd;
QString room;

QString calendarCourse::toString()
{
    QTime begin,end;
    begin.setHMS(hourBegin,minutesBegin,0);
    end.setHMS(hourEnd,minutesEnd,0);
    return QString("%1 %2 %3\n%4 %5\n%6 - %7").arg(QString::number(this->getSerialNum()),this->getName(),QString::number(this->points),this->getLecturer(),this->getRoom(),
                              begin.toString("hh:mm"),end.toString(("hh:mm")));
}
