#ifndef CALENDARCOURSE_H
#define CALENDARCOURSE_H

#include "../course.h"
#include <string>
#include <iostream>
#include <list>

#include <QTime>

#define	CALENDAR_COURSE_FIELDS	8

class calendarCourse : public Course
{
public:
    enum CourseScheme
    {
        SERIAL,
        NAME,
        TYPE,
        LECTURER,
        POINTS,
        SEM_HOURS,
        DAY_AND_HOURS,
        ROOM
    };
    calendarCourse(int serial, std::string name, std::string type, std::string lecturer,
                   double points, double semesterHours, std::string dayAndHour, std::string room);
    ~calendarCourse(){}

    int getDay() const;
    std::string getLecturer() const;
    std::string getRoom() const;
    double getSemesterHours() const;
    int getHourBegin() const;
    int getMinutesBegin() const;
    int getHourEnd() const;
    int getMinutesEnd() const;

    void setDay(const std::string &value);
    void setLecturer(const std::string &value);
    void setRoom(const std::string &value);
    void setSemesterHours(double value);
    void setHourBegin(int value);
    void setMinutesBegin(int value);
    void setHourEnd(int value);
    void setMinutesEnd(int value);

    std::string courseToString();


private:

    void setDayAndHour(std::string phrase);

    std::string lecturer;
    double semesterHours;
    int day;
    int hourBegin;
    int minutesBegin;
    int hourEnd;
    int minutesEnd;
    std::string room;

};


#endif // CALENDARCOURSE_H
