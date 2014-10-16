#include "gradeCourse.h"

gradeCourse::gradeCourse(int year, int semester, int courseNumInList, int serial, QString name, QString type, double points,double hours, double grade, QString additions) :  Course(serial,name,type)
{
    this->points = points;
    this->hours = hours;
    this->grade = grade;
    this->additions = additions;
    this->year = year;
    this->semester = semester;
    this->courseNumInList = courseNumInList;
}

gradeCourse::gradeCourse(gradeCourse &other) :  Course(other.getSerialNum(),other.getName(),other.getType())
{
    this->points = other.points;
    this->hours = other.hours;
    this->grade = other.grade;
    this->additions = other.additions;
    this->year = other.year;
    this->semester = other.semester;
    this->courseNumInList = other.courseNumInList;
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
double gradeCourse::getPoints() const
{
    return points;
}

void gradeCourse::setPoints(double value)
{
    points = value;
}


