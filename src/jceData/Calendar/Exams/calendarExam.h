#ifndef CALENDAREXAM_H
#define CALENDAREXAM_H

#include "../../page.h"
#include "calendarExamCourse.h"

#include <QString>
#include <QStringList>
#include <QList>


class calendarExam : public Page
{
public:
    calendarExam();
    void setPage(QString html);

    QList<calendarExamCourse*>* getExams() { return exams; }
    int getExamsCounter() const;



private:

    void examListInit(QString &linesTokinzedString);
    calendarExamCourse * lineToCourse(QString line);

    QString htmlDataHolderParsed;
    QList<calendarExamCourse*> *exams;

    int examsCounter; //not including madei b
};

#endif // CALENDAREXAM_H
