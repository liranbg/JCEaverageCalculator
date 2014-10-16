#ifndef PAGE_H
#define PAGE_H

#include <QDebug>
#include <QString>

/**
 * @brief The Page class
 * parsing given page - strip and clean html tags
 * use only with JCE
 *
 * Made By:
 *      Sagi Dayan,  SagiDayan@gmail.com
 *      Liran Ben Gida, LiranBG@gmail.com
 * On 31/8/2014
 */
class Page
{

public:
	~Page() {}

protected:
    Page();
    QString getString(QString &htmlToParse);
    void makeText(QString &html);

private:

    void manageTableContent(QString &html, int index);
    int stitchText(QString &from, QString &to, int index);
	bool endOfString(int index, int length);

    QString text;
    QString dateHeader;
    QString title;

};

#endif
