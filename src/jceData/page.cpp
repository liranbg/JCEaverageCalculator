#include "page.h"

Page::Page() {}
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
        if(html.at(i) == '<')
        {
            //<tr> / <td> / <th>
            QString endofTable = "</tbody>";
            QString tableTag = html.mid(i, 4); //legth of "tr/td"
            if (tableTag == "<tr>")
            {
                temp += "\n"; //new row -> new line
                i = stitchText(html, temp, i+4);
                if(i == -1) //EOF
                    break;
            }
            else if (tableTag == "<td>" || tableTag == "<th>")
            {
                temp += "\t"; // new cell -> tab between data
                if (html.mid(i, 6) == "<td><a") //link to lecturer portal, need to be deleted
                {
                    i += 6;
                    while (html.at(++i) != '>');
                    i = stitchText(html, temp, i+1);
                }
                else
                    i = stitchText(html, temp, i+4);
                if (i == -1) //EOF
                    break;
            }
            else if(tableTag == "<td ") // a Year title (in grades table)
            {
                temp += "\t";
                while(html.at(i) != '>')
                    i++;
                i = stitchText(html, temp, i+1);
            }
            else if (html.mid(i,(endofTable).length()) == endofTable) //is end of table
            {
                break;
            }
        }

    }

    this->text = temp;
}

int Page::stitchText(QString &from, QString &to, int index)
{
    if (from.at(index) == '<')
    {
        QString bTag = from.mid(index, 3);
        if (bTag != "<b>")
            return index-1; //go back one step - for the main function to inc i
        index += 3;
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

        if (from.at(index) != '\n') //check the actuall data before continue
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
