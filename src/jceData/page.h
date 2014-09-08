#ifndef PAGE_H
#define PAGE_H

/*  This Code Made By Sagi Dayan
 *  SagiDayan@gmail.com
 *
 *  Minor changes  has been made by Liran Ben Gida
 *  LiranBG@gmail.com
*/

#include <iostream>
#include <string>

class Page
{

public:

	~Page() {}

protected:
    Page();
    std::string getString(std::string& htmlToPhrased);
	void makeText(std::string& html);


private:

	int runToActualText(std::string& from, int index);
	void manageTableContent(std::string& html, int index);
	int stitchText(std::string& from, std::string& to, int index);
	bool endOfString(int index, int length);

	std::string text;
	std::string title;

};

#endif
