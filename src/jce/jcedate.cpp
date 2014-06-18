#include "jcedate.h"

jceDate::jceDate(std::string fromYear,std::string fromSemester,std::string toYear,std::string toSemester)
{
    this->fSemester = fromSemester;
    this->fYear = fromYear;
    this->tSemester = toSemester;
    this->tYear = toYear;
}

void jceDate::setFYear(std::string fromYear)
{
    this->fYear = fromYear;
}

void jceDate::setTYear(std::string toYear)
{
    this->tYear = toYear;
}

void jceDate::setFSemester(std::string fromSemester)
{
    this->fSemester = fromSemester;
}

void jceDate::setTSemester(std::string toSemester)
{
    this->tSemester = toSemester;
}

jceDate::~jceDate()
{

}
