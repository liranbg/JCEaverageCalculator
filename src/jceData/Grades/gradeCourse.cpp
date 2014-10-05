#include "gradeCourse.h"

gradeCourse::gradeCourse(int year, int semester, int courseNumInList, int serial, QString name, QString type, double points,double hours, double grade, QString additions) :  Course(serial,name,type,points)
{
    this->hours = hours;
    this->grade = grade;
    this->additions = additions;
    this->year = year;
    this->semester = semester;
    this->courseNumInList = courseNumInList;
}

gradeCourse::~gradeCourse()
{

}
double gradeCourse::getGrade() const
{
    double noGrade = NO_GRADE_YET;
    if (grade == noGrade)
        return 0;
    else
        return this->grade;
}

void gradeCourse::setHours(double hours)
{
    this->hours = hours;
}

void gradeCourse::setGrade(double grade)
{
    this->grade = grade;
}

void gradeCourse::setAdditions(QString additions)
{
    this->additions = additions;
}

void gradeCourse::setYear(int year)
{
    this->year = year;

}
void gradeCourse::setSemester(int semester)
{
    this->semester = semester;
}
int gradeCourse::getCourseNumInList() const
{
    return courseNumInList;
}

void gradeCourse::setCourseNumInList(int value)
{
    courseNumInList = value;
}

