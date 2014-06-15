#ifndef GRADE_PAGE_H
#define GRADE_PAGE_H

/*  This Code Made By Sagi Dayan
 *  SagiDayan@gmail.com
 *
 *  Minor changes  has been made by Liran Ben Gida
 *  LiranBG@gmail.com
*/

#include "Page.h"
#include "Course.h"

#include <list>
#include <string.h> //strlen and strtok to phrase the html file
#include <ctype.h> //checks if character is numeric

#define INFO_DATA_LINES_BEFORE_GRADES_DATA 5

class GradePage : public Page
{
	
public:
    GradePage(std::string html);
	~GradePage();

	void addCourse(Course *);
    void removeCourse(std::string courseSerialID);
	double getAvg();

    std::list<Course*>* getCourses() { return courses; }

private:

	void genList();
	std::string tokenToLines(std::string& textToPhrase,int fromLine = 0);
	void coursesListInit(std::string& linesTokinzedString);
	Course* lineToCourse(std::string line);

	bool isGradedYet(std::string grade);

    std::list<Course*>* courses;
	std::string tempHtml;

};


#endif
