#ifndef COURSE_H
#define COURSE_H

/*  This Code Made By Sagi Dayan
 *  SagiDayan@gmail.com
 *  & Liran Ben Gida
 *  LiranBG@gmail.com
*/

#include <QString>
#include <list>

/**
 * @brief The Course class
 *
 * This class is a prototype of a basic course structure
 *
 * Made By:
 *      Sagi Dayan,  SagiDayan@gmail.com
 *      Liran Ben Gida, LiranBG@gmail.com
 * On 31/8/2014
 */
class Course {

public:

    Course(int serial,QString name, QString type) {
        this->serialNum = serial;
        this->name = name;
        this->type = type;
    }
    virtual ~Course() { }

    int getSerialNum() const {return this->serialNum;}
    virtual QString getName() const {return this->name;}
    virtual QString getType() const {return this->type;}

    virtual void setName(QString name) { this->name = name;}
    virtual void setType(QString type){ this->type = type;}


private:

 	int serialNum;
    QString name;
    QString type;

};



#endif
