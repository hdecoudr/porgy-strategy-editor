/**
 *
 * This file is part of Porgy (http://tulip.labri.fr/TulipDrupal/?q=porgy)
 *
 * from LaBRI, University of Bordeaux, Inria and King's College London
 *
 * Porgy is free software;  you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * Porgy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this Porgy.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "generalpurposesyntaxhighlighter.h"
#include "highlightingrule.h"
#include "generalpurposesyntaxhighlightingrules.h"
#include "xmlsyntaxparser.h"

#include <QMessageBox>


/****************************************************************************************
 ****************************************************************************************
 *
 * Constructor.
 *
 ****************************************************************************************/
GeneralPurposeSyntaxHighlighter::GeneralPurposeSyntaxHighlighter(const QString* syntaxFile, QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    highlightingRules = new GeneralPurposeSyntaxHighlightingRules();
    QString err;

    if(!XmlSyntaxParser::parse(*syntaxFile, highlightingRules, &err))
    {
        error = err;
    }
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Destructor
 *
 ****************************************************************************************/
GeneralPurposeSyntaxHighlighter::~GeneralPurposeSyntaxHighlighter()
{
    delete highlightingRules;
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Get highlightning rule.
 * Keywords always appear first.
 *
 ****************************************************************************************/
QStringList GeneralPurposeSyntaxHighlighter::getKeywords()
{
    QStringList keywords;
    std::vector<HighlightingRule*> vec = (*highlightingRules)["instruction"];

    for(std::size_t i = 0; i < vec.size(); ++i)
    {
        QString str = vec.at(i)->toString();
        XmlSyntaxParser::removeWordBoundary(str);
        keywords << str;
    }

    vec = (*highlightingRules)["function"];

    for(std::size_t i = 0; i < vec.size(); ++i)
    {
        QString str = vec.at(i)->toString();
        XmlSyntaxParser::removeWordBoundary(str);
        keywords << str;
    }

    return keywords;
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Get highlightning rule.
 * Keywords always appear first.
 *
 ****************************************************************************************/
QString GeneralPurposeSyntaxHighlighter::getParseError() const
{
    return error;
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Highlight if pattern is matched.
 *
 ****************************************************************************************/
void GeneralPurposeSyntaxHighlighter::highlightBlock(const QString& text)
{
    using map_it = std::map<std::string, std::vector<HighlightingRule*>>::iterator;
    using vec_it = std::vector<HighlightingRule*>::iterator;

    for(map_it itM = highlightingRules->begin(); itM != highlightingRules->end(); ++itM)
    {    
        for(vec_it itV = itM->second.begin(); itV != itM->second.end(); ++itV)
        {
            QRegExp expression((*itV)->getPattern());
            int index = expression.indexIn(text);

            while(index >= 0)
            {
                int length = expression.matchedLength();
                setFormat(index, length, (*itV)->getFormat());
                index = expression.indexIn(text, index + length);
            }
        }
    }

    setCurrentBlockState(0);

    std::vector<HighlightingRule*> vec = (*highlightingRules)["multicomment"];

    int startIndex = 0;

    if (previousBlockState() != 1)
    {
        startIndex = vec.at(0)->getPattern().indexIn(text);
    }

    while (startIndex >= 0)
    {
        int endIndex = vec.at(1)->getPattern().indexIn(text, startIndex);
        int commentLength;

        if(endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex + vec.at(1)->getPattern().matchedLength();
        }

        setFormat(startIndex, commentLength, vec.at(0)->getFormat());
        startIndex = vec.at(0)->getPattern().indexIn(text, startIndex + commentLength);
    }
}
