#ifndef GRADE_COURSE_H
#define GRADE_COURSE_H

#include "../course.h"

#define	COURSE_FIELDS 10
#define NO_GRADE_YET 101;

/**
 * @brief The gradeCourse class
 *
 * This class holds a list of course in GPA
 * the course scheme can be found below, inside the enum CourseScheme
 *
 * The class's constructor gets the data and manipulate it into an object
 * with its relevant information.
 *
 * Made By:
 *      Sagi Dayan,  SagiDayan@gmail.com
 *      Liran Ben Gida, LiranBG@gmail.com
 * On 31/8/2014
 */
class gradeCourse : public Course {

public:
    enum CourseScheme
    {
        YEAR,
        SEMESTER,
        COURSE_NUMBER_IN_LIST,
        SERIAL,
        NAME,
        TYPE,
        POINTS,
        HOURS,
        GRADE,
        ADDITION
    };

    gradeCourse(int year, int semester, int courseNumInList, int serial, QString name, QString type, double points,double hours, double grade, QString additions);
    ~gradeCourse();

    int getYear() const { return this->year; }
    int getSemester() const { return this->semester; }
    double getHours() const {return this->hours;}
    double getGrade() const;
    QString getAddidtions() const {return this->additions;}
    int getCourseNumInList() const;
    double getPoints() const;

    void setHours(double hours);
    void setGrade(double grade);
    void setAdditions(QString additions);
    void setYear(int year);
    void setSemester(int semester);
    void setCourseNumInList(int value);
    void setPoints(double value);

private:

    double points;
    double hours;
    double grade;
    QString additions;
    int year;
    int semester;
    int courseNumInList;
};



#endif
