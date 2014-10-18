#include "page.h"

Page::Page() { dateHeader = "";}
/**
 * @brief Page::getString
 * @param htmlToPhrased
 * @return
 */
QString Page::getString(QString &htmlToParse)
{
    makeText(htmlToParse);
    return this->text;
}
void Page::makeText(QString &html)
{
    int index = 0;
    index = html.indexOf("<tbody>",0); //set index into the place where the data is
    manageTableContent(html, index);
}
/**
 * @brief Page::manageTableContent strip html, make it string
 * @param html html to parse
 * @param index index to start looking for data
 */
void Page::manageTableContent(QString &html, int index)
{
    if (index == -1)
        return;
    QString temp;
    for (int i = index; i < html.length(); i++)
    {
        if (html.at(i) == '<')
        {
            //<tr> / <td> / <th>
            QString endofTable = "</tbody>";
            QString tableTag = html.mid(i, 4); //legth of "tr/td"
            if (tableTag == "<tr>")
            {
                if (!dateHeader.isEmpty())
                    temp += dateHeader;
                i = stitchText(html, temp, i+4);
                if (i == -1) //EOF
                    break;

            }
            else if (tableTag == "</tr")
            {
                temp += "\n"; //end row -> new line
                i+=5;
            }
            else if (tableTag == "<td>" || tableTag == "<th>")
            {
                if (!dateHeader.isEmpty())
                    temp += "\t"; // new cell -> tab between data
                if (html.mid(i, 6) == "<td><a") //link to lecturer portal, need to be deleted
                {
                    i += 6;
                    while (html.at(++i) != '>');
                    i = stitchText(html, temp, i+1);
                }
                else
                    i = stitchText(html, temp, i+4);

                if (dateHeader.isEmpty())
                    temp += "\t";

                if (i == -1) //EOF
                    break;
            }
            else if (tableTag == "<td ") // a Year title (in grades table) or Day and Hour (in calendar page)
            {

                if (!dateHeader.isEmpty())
                {
                    //checking if theres a need to fill a timestamp of course
                    //if the string is not empty, then we will chop the last date stamp to avoid multiple date stamp in empty rows
                    if (!temp.isEmpty())
                        if (temp.lastIndexOf(dateHeader) == temp.length()-dateHeader.length())
                        {
                            temp.chop(dateHeader.length()+5);
                            temp += "\t";
                        }
                }
                while ((html.mid(i,5) != "</td>") && (i < (int)html.length()))
                {
                    if (html.mid(i,3) == "<b>") //for gpa. year & semester  title
                    {
                        break;
                    }
                    else if ((html.at(i) == '>') && (html.mid(i+4,3) != "<b>")) //for calendar. day and hours
                    {
                        i += 1; //lenght of >
                        break;
                    }
                    i++;
                }
                i = stitchText(html, temp, i);
                temp += "\t";
            }
            if (html.mid(i,(endofTable).length()) == endofTable) //is end of table
            {
                break;
            }
        }

    }

    this->text = temp;
}

int Page::stitchText(QString &from, QString &to, int index)
{
    if (from.mid(index,3) == "<b>")
    {
        QString bTag = from.mid(index, 3);
        QString dateline = from.mid(index,from.indexOf("</b>",index+4)-index);
        QString temp;
        QString date = "";
        dateline.remove(":");
        QStringList holder = dateline.split("&nbsp;");
        QStringList::iterator iterator;
        int i = 0;
        for (iterator = holder.begin(); iterator != holder.end(); ++iterator)
        {
            temp = (*iterator);
            if (i == 1) //year
            {
                date += temp + "\t";
            }
            else if (i == 4) //semester
            {
                date += temp;
            }
            i++;
        }
        dateHeader = date;
        if (bTag != "<b>")
            return index-1; //go back one step - for the main function to inc i
        index += dateline.length();
    }

    while (from.at(index) != '<' && index < (int)from.length())
    {
        if (from[index] == '&')
        {
            //&nbsp;
            QString nbspChr = from.mid(index, 6);
            if (nbspChr == "&nbsp;")
            {
                index += 5;
                from.replace(index,1,' ');
            }

        }

        if (endOfString(index,(int) from.length()))
            return -1; //EOF

        else if (from.at(index) == '<')
            return index - 1; //go back one step - for the main function to inc i

        if ((from.at(index) != '\n') && (from.at(index) != '\t')) //check the actuall data before continue
            to += from.at(index);
        index++;
    }

    return index-1;
}
bool Page::endOfString(int index, int length)
{
    if(index < length)
        return false;
    return true;
}
