#ifndef COURSE_H
#define COURSE_H

/*  This Code Made By Sagi Dayan
 *  SagiDayan@gmail.com
 *
 *  Minor changes  has been made by Liran Ben Gida
 *  LiranBG@gmail.com
*/

#include <string>
#include <iostream>
#include <list>

#define	COURSE_FIELDS	7
#define NO_GRADE_YET 101;


class Course{

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

    Course(int serial,std::string name, std::string type, double points, double hours, double grade, std::string additions);
	~Course();

    int getSerialNum() const {return this->serialNum;}
    std::string getName() const {return this->name;}
    std::string getType() const {return this->type;}
    double getPoints() const {return this->points;}
    double getHours() const {return this->hours;}
    double getGrade() const ;
    std::string getAddidtions() const {return this->additions;}

    void setName(std::string name);
    void setType(std::string type);
    void setPoints(double points);
    void setHours(double hours);
    void setGrade(double grade);
    void setAdditions(std::string additions);

private:

 	int serialNum;
    std::string name;
    std::string type;
	double points;
	double hours;
	double grade;
    std::string additions;
};



#endif
