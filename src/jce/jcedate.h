#ifndef JCEDATE_H
#define JCEDATE_H

#include <iostream>

class jceDate
{

public:

    jceDate(std::string fromYear, std::string fromSemester, std::string toYear, std::string toSemester);
    ~jceDate();

    void setFYear(std::string fromYear);
    void setTYear(std::string toYear);
    void setFSemester(std::string fromSemester);
    void setTSemester(std::string toSemester);

    std::string getFYear() { return this->fYear;}
    std::string getTYear() { return this->tYear;}
    std::string getFSemester() { return this->fSemester;}
    std::string getTSemester() { return this->tSemester;}

private:
    std::string fYear; //from
    std::string fSemester;

    std::string tYear; //to
    std::string tSemester;

};

#endif // JCEDATE_H
