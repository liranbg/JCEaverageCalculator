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

#include <list>

class GradePage : public Page
{
	
public:
    GradePage(QString html);
	~GradePage();

    void removeCourse(QString courseSerialID);
	double getAvg();

    std::list<gradeCourse*>* getCourses() { return courses; }

private:

    QString tokenToLines(QString &textToPhrase);
    void coursesListInit(QString &linesTokinzedString);
    gradeCourse* lineToCourse(QString line);

    bool isGradedYet(QString grade);

    std::list<gradeCourse*>* courses;
    QString tempHtml;

};


#endif
