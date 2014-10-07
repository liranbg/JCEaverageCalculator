#ifndef CALENDARCOURSE_H
#define CALENDARCOURSE_H

#include "../course.h"
#include <QTime>

#define	CALENDAR_COURSE_FIELDS	8

class calendarCourse : public Course
{
public:
    enum CourseCalendarType
    {
        ExamSchedule,
        CoursesSchedule
    };

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
                   QString room, calendarCourse::CourseCalendarType type = calendarCourse::CourseCalendarType::CoursesSchedule);
    ~calendarCourse(){}

    int getDay() const;
    QString getLecturer() const;
    QString getRoom() const;
    double getSemesterHours() const;
    int getHourBegin() const;
    int getMinutesBegin() const;
    int getHourEnd() const;
    int getMinutesEnd() const;

    void setDay(const QString &value);
    void setLecturer(const QString &value);
    void setRoom(const QString &value);
    void setSemesterHours(double value);
    void setHourBegin(int value);
    void setMinutesBegin(int value);
    void setHourEnd(int value);
    void setMinutesEnd(int value);

    QString courseToString();


private:

    void setDayAndHour(QString parse, CourseCalendarType type);

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
