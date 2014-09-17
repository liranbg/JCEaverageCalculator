#ifndef GRADE_COURSE_H
#define GRADE_COURSE_H

/*  This Code Made By Sagi Dayan
 *  SagiDayan@gmail.com
 *
 *  Minor changes  has been made by Liran Ben Gida
 *  LiranBG@gmail.com
*/
#include "../course.h"
#include <string>
#include <iostream>
#include <list>

#define	COURSE_FIELDS	7
#define NO_GRADE_YET 101;


class gradeCourse : public Course {

public:
    enum CourseScheme
    {
        SERIAL,
        NAME,
        TYPE,
        POINTS,
        HOURS,
        GRADE,
        ADDITION
    };

    gradeCourse(int serial, QString name, QString type, double points,double hours, double grade, QString additions);
    ~gradeCourse();

    double getHours() const {return this->hours;}
    double getGrade() const ;
    QString getAddidtions() const {return this->additions;}

    void setHours(double hours);
    void setGrade(double grade);
    void setAdditions(QString additions);

private:

	double hours;
	double grade;
    QString additions;
};



#endif
