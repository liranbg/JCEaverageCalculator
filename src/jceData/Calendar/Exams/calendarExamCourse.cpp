#include "calendarExamCourse.h"


calendarExamCourse::calendarExamCourse(int serial, QString name, QString lecturer, QString field,
                                       QString type, QString firstDate, QString firstHourbegin,
                                       QString secondDate, QString secondHourbegin) : Course (serial,name,type)
{
    this->lecturer = lecturer;
    this->field = field;
    setDate(firstDate,true);
    setDate(secondDate,false);
    setTime(firstHourbegin,true);
    setTime(secondHourbegin,false);

}
/**
 * @brief calendarExamCourse::setDate
 * @param date
 * @param isFirst if true > first. otherwise > second
 */
void calendarExamCourse::setDate(QString date, bool isFirst)
{
    if (isFirst)
        this->firstDate = QDate::fromString(date,"dd/MM/yyyy");
    else
        this->secondDate = QDate::fromString(date,"dd/MM/yyyy");
}
/**
 * @brief calendarExamCourse::setTime
 * @param time
 * @param isFirst if true > first. otherwise > second
 */
void calendarExamCourse::setTime(QString time, bool isFirst)
{
//    qDebug() << "time string is: " << time;
    if (isFirst)
        this->firstHourbegin = QTime::fromString(time,"hh:mm");
    else
        this->secondHourbegin = QTime::fromString(time,"hh:mm");
}
QTime calendarExamCourse::getSecondHourbegin() const
{
    return secondHourbegin;
}

void calendarExamCourse::setSecondHourbegin(const QTime &value)
{
    secondHourbegin = value;
}

QDate calendarExamCourse::getSecondDate() const
{
    return secondDate;
}

void calendarExamCourse::setSecondDate(const QDate &value)
{
    secondDate = value;
}

QTime calendarExamCourse::getFirstHourbegin() const
{
    return firstHourbegin;
}

void calendarExamCourse::setFirstHourbegin(const QTime &value)
{
    firstHourbegin = value;
}

QDate calendarExamCourse::getFirstDate() const
{
    return firstDate;
}

void calendarExamCourse::setFirstDate(const QDate &value)
{
    firstDate = value;
}

QString calendarExamCourse::getField() const
{
    return field;
}

void calendarExamCourse::setField(const QString &value)
{
    field = value;
}

QString calendarExamCourse::getLecturer() const
{
    return lecturer;
}

void calendarExamCourse::setLecturer(const QString &value)
{
    lecturer = value;
}

