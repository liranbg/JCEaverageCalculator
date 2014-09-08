#ifndef GRADE_PAGE_H
#define GRADE_PAGE_H

/*  This Code Made By Sagi Dayan
 *  SagiDayan@gmail.com
 *
 *  Minor changes  has been made by Liran Ben Gida
 *  LiranBG@gmail.com
*/

#include "../page.h"
#include "../Grades/gradeCourse.h"

#include <list>
#include <string.h> //strlen and strtok to phrase the html file
#include <ctype.h> //checks if character is numeric

class GradePage : public Page
{
	
public:
    GradePage(std::string html);
	~GradePage();

    void addCourse(gradeCourse *);
    void removeCourse(std::string courseSerialID);
	double getAvg();

    std::list<gradeCourse*>* getCourses() { return courses; }

private:

    std::string tokenToLines(std::string& textToPhrase);
	void coursesListInit(std::string& linesTokinzedString);
    gradeCourse* lineToCourse(std::string line);

	bool isGradedYet(std::string grade);

    std::list<gradeCourse*>* courses;
	std::string tempHtml;

};


#endif
