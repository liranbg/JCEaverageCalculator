#ifndef JCELOGINHTMLSCRIPTS_H
#define JCELOGINHTMLSCRIPTS_H

#include <string>

#define dst_host "yedion.jce.ac.il"
#define dst_port 443

#include "../user.h"

class jceLoginHtmlScripts
{

private:

    jceLoginHtmlScripts();

public:

    static std::string makeRequest(std::string parameters)
    {
        std::string msg;
        msg = "POST /yedion/fireflyweb.aspx HTTP/1.1\r\n";
        msg += "Host: " + std::string(dst_host) + "\r\n";
        msg += "Content-Type: application/x-www-form-urlencoded\r\n";
        msg += "Content-Length: " + to_string(parameters.length()) + "\r\n";
        msg += "Proxy-Connection: Keep-Alive\r\n";
        msg += "Accept-Charset: utf-8";
        msg += "Accept: text/plain\r\n";
        msg += "Connection: Keep-Alive\r\n";
        msg += "\r\n";
        msg += parameters;
        return msg;
    }

    const static std::string getFirstValidationStep(const user &usr)
    {
        std::string parameters = "?appname=BSHITA&prgname=LoginValidation&arguments=-N";
        parameters += usr.getUsername();
        parameters += ",-N";
        parameters += usr.getPassword();
        return parameters;
    }

    const static std::string getSecondValidationStep(const user &usr)
    {
        std::string parameters;
        parameters = "prgname=LoginValidtion1&Arguments=-N";
        parameters += usr.getUserID();
        parameters += ",-A,-N";
        parameters += usr.getHashedPassword();
        parameters += ",-A,-A";
        return parameters;
    }
    const static std::string getGradesPath(const user &usr)
    {
        std::string parameters;
        parameters = "PRGNAME=HADPASAT_MISMAHIM_LETALMID&ARGUMENTS=TZ,-N4,R1C2,R1C4,R1C1,R1C3,-A,-A,R1C5,-A,UNIQ&";
        parameters += "TZ=" + usr.getUserID() + "&";
        parameters += "UNIQ=" + usr.getHashedPassword() + "&";
        parameters += "R1C2=" + usr.date->getFYear() + "&";
        parameters += "R1C1=" + usr.date->getTYear() + "&";
        parameters += "R1C3=" + usr.date->getTSemester() + "&";
        parameters += "R1C4=" + usr.date->getFSemester() + "&";
        parameters += "R1C5=0";
        return parameters;
    }


};

#endif // JCELOGINHTMLSCRIPTS_H
