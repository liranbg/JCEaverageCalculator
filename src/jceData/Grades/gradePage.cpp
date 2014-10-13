#include "gradePage.h"

static int maxYear = 0;
static int minYear = 9999;

GradePage::GradePage(QString html) : Page()
{
    tempHtml = getString(html);
    coursesListInit(tempHtml);
}
GradePage::~GradePage()
{
    for (Course* c : courses)
        delete c;
}

/**
 * @brief GradePage::removeCourse
 * @param courseSerialID - course ID to remove
 */
void GradePage::removeCourse(QString courseSerialID)
{
    for(gradeCourse* c : courses)
    {
        if (c->getSerialNum() == courseSerialID.toInt())
        {
            courses.removeAll(c);
            delete c;
            return;
        }
    }

}

/**
 * @brief GradePage::coursesListInit
 * using lineToCourse function, its making a list of gradeCourse object from a given string of information
 * @param linesTokinzedString list of courses, tokenized by lines. containing data of each course
 */
void GradePage::coursesListInit(QString &linesTokinzedString)
{
    QString tempToken;

    QStringList holder = linesTokinzedString.split("\n");
    QStringList::iterator iterator;
    for (iterator = holder.begin(); iterator != holder.end(); ++iterator)
    {
        tempToken = (*iterator);
        if ((!tempToken.isEmpty()) && (tempToken.length() > 1))
        {
            gradeCourse *cTemp = lineToCourse(tempToken);
            if (cTemp != NULL)
                this->courses.push_back(cTemp);
        }
    }
}

/**
 * @brief GradePage::lineToCourse
 * making an object of gradepage with the given information from string
 * @param line - lines tokenized by tabs containing each course information (year, serial, name, points and etc..)
 * @return
 */
gradeCourse* GradePage::lineToCourse(QString line)
{
    gradeCourse *tempC = NULL;
    QString templinearray[COURSE_FIELDS];//[year,semester,numInList,serial,name,type,points,hours,grade,additions]
    int serial,year,semester,courseNumInList;
    double points,hours,grade;
    QString name,type, additions;

    QString tempToken;
    int i = 0;
    QStringList holder = line.split("\t");
    QStringList::iterator iterator;
    for (iterator = holder.begin(); iterator != holder.end(); ++iterator)
    {

        tempToken = (*iterator);
        tempToken = tempToken.trimmed();
        //we are checking it because in GPA, serial and course name are mixed
        if (i == gradeCourse::CourseScheme::SERIAL)
        {
            QString tempDataOfSerialCourseName;

            //getting serial
            QStringList secHolder = tempToken.split(" ");
            QStringList::iterator secIterator = secHolder.begin();
            tempDataOfSerialCourseName = *secIterator;
            templinearray[i] = tempDataOfSerialCourseName.trimmed();
            //getting course name;
            ++secIterator;
            tempDataOfSerialCourseName.clear();
            while (secIterator != secHolder.end())
            {
                tempDataOfSerialCourseName.append(*secIterator + " ");
                secIterator++;
            }
            templinearray[++i] = tempDataOfSerialCourseName.trimmed();
        }
        else
        {
            templinearray[i] = tempToken;
        }

        i++;
        if (i >= COURSE_FIELDS)
            break;
    }

    if (templinearray[0] == "") //empty parsing
    {
        qCritical() << Q_FUNC_INFO << "empty parsing";
        return NULL;
    }

    year = templinearray[gradeCourse::CourseScheme::YEAR].toInt();
    semester = templinearray[gradeCourse::CourseScheme::SEMESTER].toInt();
    courseNumInList = templinearray[gradeCourse::CourseScheme::COURSE_NUMBER_IN_LIST].toInt();
    serial = templinearray[gradeCourse::CourseScheme::SERIAL].toInt();

    name = templinearray[gradeCourse::CourseScheme::NAME];
    type = templinearray[gradeCourse::CourseScheme::TYPE];

    points = templinearray[gradeCourse::CourseScheme::POINTS].toDouble();
    hours = templinearray[gradeCourse::CourseScheme::HOURS].toDouble();

    if (isGradedYet(templinearray[gradeCourse::CourseScheme::GRADE]))
        grade = templinearray[gradeCourse::CourseScheme::GRADE].toDouble();
    else
        grade = NO_GRADE_YET;

    additions = templinearray[gradeCourse::CourseScheme::ADDITION];

    if (year >= maxYear)
        maxYear = year;

    if ((year <= minYear) && (points > 0)) //not graded yet isnt influced year!
    {
        minYear = year;
    }

    tempC = new gradeCourse(year,semester,courseNumInList,serial,name,type,points,hours,grade,additions);
    return tempC;
}

/**
 * @brief GradePage::isGradedYet checking if one of the chars inside grade is not a number
 * @param grade
 * @return if has bee  graded or not
 */
bool GradePage::isGradedYet(QString grade)
{
    if (strlen(grade.toStdString().c_str()) <= 1)
        return false;

    for (QChar c: grade)
    {
        if (c == '\0')
            break;

        if (((!c.isDigit())  && (!c.isSpace()))) //48 = 0, 57 = 9
            return false;

    }
    return true;
}

/**
 * @brief GradePage::getAvg  getting avg
 * @return -  gpa avg of all courses
 */
double GradePage::getAvg()
{
    double avg = 0;
    double points = 0;
    for (gradeCourse* c : courses)
    {
        if ((c->getGrade() != 0))
        {
            avg += c->getGrade() * c->getPoints();
            points += c->getPoints();
        }
    }

    avg /= points;
    return avg;
}

/**
 * @brief GradePage::getAvg getting avg of given year
 * @param year  - year (yyyy)
 * @return -  gpa avg of given year
 */
double GradePage::getAvg(int year)
{
    double avg = 0;
    double points = 0;
    for (gradeCourse* c : courses)
    {
        if ((c->getGrade() != 0) && (c->getYear() == year))
        {
            avg += c->getGrade() * c->getPoints();
            points += c->getPoints();
        }
    }
    if (points != 0)
        avg /= points;
    else
        avg=0;
    return avg;
}

/**
 * @brief GradePage::getAvg
 * @param year  - year (yyyy)
 * @param semester - semeser (1-3)
 * @return -gpa avg of given year in given semester
 */

double GradePage::getAvg(int year, int semester)
{
    double avg = 0;
    double points = 0;
    for (gradeCourse* c : courses)
    {
        if ((c->getGrade() != 0) && (c->getYear() == year) && (c->getSemester() == semester))
        {
            avg += c->getGrade() * c->getPoints();
            points += c->getPoints();
        }
    }
    if (points != 0)
        avg /= points;
    else
        avg=0;
    return avg;
}

/**
 * @brief GradePage::getMinYearInList
 * @return the minimal year inside courses list
 */
int GradePage::getMinYearInList()
{
    return minYear;
}

int GradePage::getMaxYearInList()
{
    return maxYear;
}
