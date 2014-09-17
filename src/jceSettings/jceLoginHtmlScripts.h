#ifndef JCELOGINHTMLSCRIPTS_H
#define JCELOGINHTMLSCRIPTS_H

#include "./src/jceSettings/user.h"

#define dst_host "yedion.jce.ac.il"
#define dst_port 443

/**
 * @brief The jceLoginHtmlScripts class
 *  this class gets parameters and making the httml request string to be sent
 *  the headers of the http request were written according to JCE's student portal login page
 */

class jceLoginHtmlScripts
{

private:

    jceLoginHtmlScripts();

public:

    static QString makeRequest(QString parameters)
    {
        QString msg;
        msg = "POST /yedion/fireflyweb.aspx HTTP/1.1\r\n";
        msg += "Host: " + QString(dst_host) + "\r\n";
        msg += "Content-Type: application/x-www-form-urlencoded\r\n";
        msg += "Content-Length: " + QString::number(parameters.length()) + "\r\n";
        msg += "Proxy-Connection: Keep-Alive\r\n";
        msg += "Accept-Charset: UTF-8";
        msg += "Accept: text/plain\r\n";
        msg += "Connection: Keep-Alive\r\n";
        msg += "\r\n";
        msg += parameters;
        return msg;
    }

    const static QString getFirstValidationStep(const user &usr)
    {
        QString parameters = "?appname=BSHITA&prgname=LoginValidation&arguments=-N";
        parameters += usr.getUsername();
        parameters += ",-N";
        parameters += usr.getPassword();
        return parameters;
    }

    const static QString getSecondValidationStep(const user &usr)
    {
        QString parameters;
        parameters = "prgname=LoginValidtion1&Arguments=-N";
        parameters += usr.getUserID();
        parameters += ",-A,-N";
        parameters += usr.getHashedPassword();
        parameters += ",-A,-A";
        return parameters;
    }
    const static QString getGradesPath(const user &usr,
                                           int fromYear,
                                           int toYear,
                                           int fromSemester,
                                           int toSemester)
    {
        QString parameters;
        parameters = "PRGNAME=HADPASAT_MISMAHIM_LETALMID&ARGUMENTS=TZ,-N4,R1C2,R1C4,R1C1,R1C3,-A,-A,R1C5,-A,UNIQ&";
        parameters += "TZ=" + usr.getUserID() + "&";
        parameters += "UNIQ=" + usr.getHashedPassword() + "&";
        parameters += "R1C2=" + QString::number(fromYear) + "&";
        parameters += "R1C1=" + QString::number(toYear) + "&";
        parameters += "R1C3=" + QString::number(toSemester) + "&";
        parameters += "R1C4=" + QString::number(fromSemester) + "&";
        parameters += "R1C5=0";
        return parameters;
    }
    const static QString getCalendar(const user &usr,int year, int semester)
    {
        QString parameters;
        parameters = "PRGNAME=Bitsua_maarechet_shaot&ARGUMENTS=TZ,UNIQ,MisparSheilta,R1C1,R1C2&";
        parameters += "TZ=" + usr.getUserID() + "&";
        parameters += "UNIQ=" + usr.getHashedPassword() + "&";
        parameters += "MisparSheilta=3&";
        parameters += "R1C1=" + QString::number(year) + "&";
        parameters += "R1C2=" + QString::number(semester) + "&";
        return parameters;
    }


};

#endif // JCELOGINHTMLSCRIPTS_H
