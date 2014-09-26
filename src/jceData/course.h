#ifndef COURSE_H
#define COURSE_H

/*  This Code Made By Sagi Dayan
 *  SagiDayan@gmail.com
 *  & Liran Ben Gida
 *  LiranBG@gmail.com
*/

#include <QString>
#include <list>


class Course {

public:

    Course(int serial,QString name, QString type, double points) {
        this->serialNum = serial;
        this->name = name;
        this->type = type;
        this->points = points;
    }
    virtual ~Course() { }

    int getSerialNum() const {return this->serialNum;}
    virtual QString getName() const {return this->name;}
    virtual QString getType() const {return this->type;}
    virtual double getPoints() const {return this->points;}

    virtual void setName(QString name) { this->name = name;}
    virtual void setType(QString type){ this->type = type;}
    virtual void setPoints(double points){ this->points = points;}


private:

 	int serialNum;
    QString name;
    QString type;
	double points;

};



#endif
