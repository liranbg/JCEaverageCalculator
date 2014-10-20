#ifndef CALENDARCOURSE_H
#define CALENDARCOURSE_H

#include "../../course.h"
#include <QTime>
#include <QDebug>

#define	CALENDAR_COURSE_FIELDS	8

/**
 * @brief The calendarCourse class
 *
 * This class holds each scheduled course
 * the course scheme can be found below, inside the enum CourseScheme
 *
 * The class's constructor gets the data and manipulate it into an object
 * with its relevant information.
 *
 * Made By liran ben gida, LiranBG@gmail.com On 31/8/2014
 */
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

    calendarCourse(int serial, QString name, QString type, QString lecturer,
                   double points, double semesterHours, QString dayAndHour,
                   QString room);

    ~calendarCourse(){}

    int getDay() const;
    QString getLecturer() const;
    QString getRoom() const;
    double getSemesterHours() const;
    int getHourBegin() const;
    int getMinutesBegin() const;
    int getHourEnd() const;
    int getMinutesEnd() const;
    double getPoints() const;

    void setDay(const QString &value);
    void setLecturer(const QString &value);
    void setRoom(const QString &value);
    void setSemesterHours(double value);
    void setHourBegin(int value);
    void setMinutesBegin(int value);
    void setHourEnd(int value);
    void setMinutesEnd(int value);
    void setPoints(double value);

    QString toString();

private:

    void setDayAndHour(QString parse);

    double points;
    QString lecturer;
    double semesterHours;
    int day;
    int hourBegin;
    int minutesBegin;
    int hourEnd;
    int minutesEnd;
    QString room;

};


#endif // CALENDARCOURSE_H
