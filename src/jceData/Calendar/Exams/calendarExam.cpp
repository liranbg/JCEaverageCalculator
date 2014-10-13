#include "calendarExam.h"

calendarExam::calendarExam()
{
    htmlDataHolderParsed = "";
}
/**
 * @brief calendarExam::calendarExam
 * copy constructor
 * @param other
 */
calendarExam::calendarExam(calendarExam &other)
{
    examsCounter = 0;
    exams.clear();
    for (calendarExamCourse * tempExam: other.getExams())
    {
        this->exams.push_back(new calendarExamCourse(*tempExam));
        examsCounter++;
    }
}

/**
 * @brief calendarExam::setPage -   getting a html page and stripping it into a exams schedule in a list
 * @param html - html page with tags
 */
void calendarExam::setPage(QString html)
{
    examsCounter = 0;
    exams.clear();
    this->htmlDataHolderParsed = getString(html);
    examListInit(htmlDataHolderParsed);
}

/**
 * @brief calendarExam::examListInit spliting the stripped html into lines, and using linetocourse function to make it an object
 * @param linesTokinzedString - list of lines, each line has differen exam information
 */
void calendarExam::examListInit(QString &linesTokinzedString)
{
    QString tempToken;

    QStringList holder = linesTokinzedString.split("\n");
    QStringList::iterator iterator;
    for (iterator = holder.begin(); iterator != holder.end(); ++iterator)
    {
        tempToken = (*iterator);
        if (!tempToken.isEmpty())
        {
            calendarExamCourse *cTemp = lineToCourse(tempToken);
            if (cTemp != NULL)
                this->exams.push_back(cTemp);
        }
    }
}

/**
 * @brief calendarExam::lineToCourse getting line of exam with data and make it an object containing data (date, time, course name and etc)
 * @param line - parsed line with tabs between each relevant data
 * @return object of examcourse with its data
 */
calendarExamCourse *calendarExam::lineToCourse(QString line)
{
    calendarExamCourse *tempC = NULL;
    QString templinearray[EXAM_SCHEDULE_FIELDS];
    //SERIAL, NAME, LECTURER, FIELD, TYPE, FIRST_DATE, FIRST_HOUR_BEGIN, SECOND_DATE, SECOND_HOUR_BEGIN

    int serial;
    QString name, lecturer, field, type, firstDate, firstHourbegin, secondDate, secondHourbegin;

    QString tempToken;
    int i = 0;
    QStringList holder = line.split("\t");
    QStringList::iterator iterator;
    for (iterator = holder.begin(); iterator != holder.end(); ++iterator)
    {

        tempToken = (*iterator);
        templinearray[i] = tempToken.trimmed();
        i++;
        if (i >= EXAM_SCHEDULE_FIELDS)
            break;
    }

    if (templinearray[0] == "") //empty parsing
        return NULL;


    serial = templinearray[calendarExamCourse::CourseScheme::SERIAL].toInt();
    name = templinearray[calendarExamCourse::CourseScheme::NAME];

    lecturer = templinearray[calendarExamCourse::CourseScheme::LECTURER];
    if (lecturer.isEmpty())
        lecturer = LECTURER_DEFAULT_STRING;

    field = templinearray[calendarExamCourse::CourseScheme::FIELD];
    type = templinearray[calendarExamCourse::CourseScheme::TYPE];

    firstDate = templinearray[calendarExamCourse::CourseScheme::FIRST_DATE];
    if (firstDate.isEmpty())
        return NULL; //can't set a default date to an exam. must be an error

    firstHourbegin = templinearray[calendarExamCourse::CourseScheme::FIRST_HOUR_BEGIN];
    if (firstHourbegin.isEmpty())
        firstHourbegin = HOUR_DEFAULT_STRING;
    secondDate = templinearray[calendarExamCourse::CourseScheme::SECOND_DATE];
    if (secondDate.isEmpty())
    {
        secondDate = SECOND_DATE_DEFAULT_STRING;
        secondHourbegin = HOUR_DEFAULT_STRING;
    }
    else
    {
        secondHourbegin = templinearray[calendarExamCourse::CourseScheme::SECOND_HOUR_BEGIN];
        if (secondHourbegin.isEmpty())
            secondHourbegin = HOUR_DEFAULT_STRING;
    }

    tempC = new calendarExamCourse(serial,name,lecturer,field,type,firstDate,firstHourbegin,secondDate,secondHourbegin);
    examsCounter++;
    return tempC;
}
int calendarExam::getExamsCounter() const
{
    return examsCounter;
}

