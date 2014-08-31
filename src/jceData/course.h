#ifndef COURSE_H
#define COURSE_H

/*  This Code Made By Sagi Dayan
 *  SagiDayan@gmail.com
 *
 *  minor changes by Liran Ben Gida
 *  LiranBG@gmail.com
*/

#include <string>
#include <iostream>
#include <list>


class Course {

public:

    Course(int serial,std::string name, std::string type, double points) {
        this->serialNum = serial;
        this->name = name;
        this->type = type;
        this->points = points;
    }
    virtual ~Course() { }

    int getSerialNum() const {return this->serialNum;}
    virtual std::string getName() const {return this->name;}
    virtual std::string getType() const {return this->type;}
    virtual double getPoints() const {return this->points;}

    virtual void setName(std::string name) { this->name = name;}
    virtual void setType(std::string type){ this->type = type;}
    virtual void setPoints(double points){ this->points = points;}


private:

 	int serialNum;
    std::string name;
    std::string type;
	double points;

};



#endif
