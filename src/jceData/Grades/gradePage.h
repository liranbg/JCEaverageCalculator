#ifndef GRADE_PAGE_H
#define GRADE_PAGE_H

/*  This Code Made By Sagi Dayan
 *  SagiDayan@gmail.com
 *
 *  Changes  has been made by Liran Ben Gida
 *  LiranBG@gmail.com
*/

#include "../page.h"
#include "../Grades/gradeCourse.h"

#include <QList>

/**
 * @brief The GradePage class
 *
 * This class generating html string to a list of gradeCourse
 * Each item in a list is a course with its information (grade, points, name, serial and etc)
 *
 * Made By:
 *      Sagi Dayan,  SagiDayan@gmail.com
 *      Liran Ben Gida, LiranBG@gmail.com
 * On 31/8/2014
 */
class GradePage : public Page
{

public:
    GradePage(QString html);
    GradePage(GradePage &other);
    ~GradePage();

    void removeCourse(QString courseSerialID);

    double getAvg();
    double getAvg(int year);
    double getAvg(int year, int semester);

    int getMinYearInList();
    int getMaxYearInList();


    QList<gradeCourse*> getCourses() { return courses; }

private:

    void coursesListInit(QString &linesTokinzedString);
    gradeCourse* lineToCourse(QString line);

    bool isGradedYet(QString grade);

    QList<gradeCourse*> courses;

    QString tempHtml;

};


#endif
