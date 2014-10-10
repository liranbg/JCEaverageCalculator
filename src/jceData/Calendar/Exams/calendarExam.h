#ifndef CALENDAREXAM_H
#define CALENDAREXAM_H

#include "../../page.h"
#include "calendarExamCourse.h"

#include <QString>
#include <QStringList>
#include <QList>

/**
 * @brief The calendarExam class
 *
 * This class generating html string to a list of examcourses
 * Each item in a list is a course with its information (hour, day, name, serial and etc)
 *
 * Made By liran ben gida, LiranBG@gmail.com On 08/10/2014
 */
class calendarExam : public Page
{
public:
    calendarExam();
    void setPage(QString html);

    QList<calendarExamCourse*> getExams() { return exams; }
    int getExamsCounter() const;



private:

    void examListInit(QString &linesTokinzedString);
    calendarExamCourse * lineToCourse(QString line);

    QString htmlDataHolderParsed;
    QList<calendarExamCourse*> exams;

    int examsCounter; //not including madei b
};

#endif // CALENDAREXAM_H
