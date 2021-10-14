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

#ifndef XMLSYNTAXPARSER_H
#define XMLSYNTAXPARSER_H

#include <QBrush>
#include <QFont>
#include <QXmlStreamReader>

class GeneralPurposeSyntaxHighlightingRules;


class XmlSyntaxParser
{
public:
    static bool parse(const QString& file, GeneralPurposeSyntaxHighlightingRules& rules, QString* error = nullptr);

public:
    inline static QBrush intToRgb(int color)
    {
        return QBrush(QColor((color >> 0x10) & 0xFF, (color >> 0x08) & 0xFF, color & 0xFF));
    }

    inline static int isBold(const QString& style)
    {
        if(style.contains("bold"))
        {
            return QFont::Bold;
        }

        return QFont::Normal;
    }

    inline static bool isItalic(const QString& style)
    {
        return (style.contains("italic"));
    }

    inline static bool hasWordBoundary(const QString& word)
    {
        return (word.contains("\\b"));
    }

    inline static bool removeWordBoundary(QString& word)
    {
        if(XmlSyntaxParser::hasWordBoundary(word))
        {
            word = word.mid(2, word.size() - 4);
            return true;
        }

        return false;
    }
};


#endif // XMLSYNTAXPARSER_H
