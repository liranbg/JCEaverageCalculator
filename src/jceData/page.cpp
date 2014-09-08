#include "page.h"

Page::Page()
{

}
std::string Page::getString(std::string& htmlToPhrased)
{
    makeText(htmlToPhrased);
    return this->text;
}
void Page::makeText(std::string& html)
{
    int index = 0;
    index = runToActualText(html, index); //set index into the actual place where the data is
    manageTableContent(html, index);
}

int Page::runToActualText(std::string& from, int index)
{
    while (index < (int)from.length())
    {
        if (from[index] == '<')
        {
                if (from.substr(index,7) == "<tbody>")
                    return index+7;
         }
        index++;
    }
//    while(index < (int)from.length())
//    {
//        if(from[index] == '<')
//        {
//            index++;
//            if(from[index] == '!')
//            {
//                //!--FileName
//                std::string bodyTag = from.substr(index, 11); //!--FileName

//                if(bodyTag == "!--FileName") //check if the tag is body tag
//                {
//                    while(from[index] != '>')
//                        index++;
//                    return index;
//                }
//            }
//        }
//        index++;
//}
    return -1;
}

void Page::manageTableContent(std::string& html, int index)
{
    std::string temp;
    for (int i = index; i < (int)html.length(); i++)
    {
        if(html[i] == '<')
        {
            //<tr> / <td> / <th>
            std::string endofTable = "</tbody>";
            std::string tableTag = html.substr(i, 4); //legth of "tr/td"
            if(tableTag == "<tr>")
            {
                temp += "\n"; //new row -> new line
                i = stitchText(html, temp, i+4);
                if(i == -1) //EOF
                    break;
            }
            else if(tableTag == "<td>" || tableTag == "<th>")
            {
                temp += "\t"; // new cell -> tab between data
                if (html.substr(i, 6) == "<td><a") //link to lecturer portal, need to be deleted
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
                while(html[i] != '>')
                    i++;
                i = stitchText(html, temp, i+1);
            }
            else if (html.substr(i,(endofTable).length()) == endofTable) //is end of table
            {
                break;
            }
        }

    }

    this->text = temp;
}

int Page::stitchText(std::string& from, std::string& to, int index)
{
    if (from[index] == '<')
    {
        std::string bTag = from.substr(index, 3);
        if (bTag != "<b>")
            return index-1; //go back one step - for the main function to inc i
        index += 3;
    }

    while (from[index] != '<' && index < (int)from.length())
    {
        if (from[index] == '&')
        {
            //&nbsp;
            std::string nbspChr = from.substr(index, 6);
            if (nbspChr == "&nbsp;")
            {
                index += 5;
                from.at(index) = ' ';
            }

        }

        if (endOfString(index,(int) from.length()))
            return -1; //EOF

        else if (from[index] == '<')
            return index - 1; //go back one step - for the main function to inc i

        if (from[index] != '\n') //check the actuall data before continue
            to += from[index];
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
