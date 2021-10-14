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

#include "xmlsyntaxparser.h"
#include "highlightingrule.h"
#include "generalpurposesyntaxhighlightingrules.h"

#include <QFile>
#include <QXmlStreamReader>


bool XmlSyntaxParser::parse(const QString& file, GeneralPurposeSyntaxHighlightingRules& rules, QString* error)
{
    QFile f(file);

    if(f.open(QIODevice::ReadOnly))
    {
        QXmlStreamReader reader(&f);

        QXmlStreamAttributes        attributes;
        QXmlStreamReader::TokenType token;
        QString                     element;

        QTextCharFormat             cFormat;

        while(!reader.atEnd())
        {
            token = reader.readNext();

            if(reader.hasError())
            {
                if(error)
                {
                    *error = reader.errorString();
                }

                return false;
            }

            if(token == QXmlStreamReader::StartDocument)
            {
                continue;
            }

            if(token == QXmlStreamReader::StartElement)
            {
                if(reader.name() == "name")
                {
                    //
                    // Do something with name.
                    //
                }
                else if(reader.name() == "attributes")
                {
                    while(!reader.atEnd())
                    {
                        token = reader.readNext();

                        if(reader.hasError())
                        {
                            if(error)
                            {
                                *error = reader.errorString();
                            }

                            return false;
                        }

                        if(token == QXmlStreamReader::StartElement)
                        {

                            if(reader.name() == "instructions")
                            {
                                attributes = reader.attributes();
                                cFormat.setForeground(XmlSyntaxParser::intToRgb(attributes.at(0).value().toString().toInt()));
                                cFormat.setFontWeight(XmlSyntaxParser::isBold(attributes.at(1).value().toString()));
                                cFormat.setFontItalic(XmlSyntaxParser::isItalic(attributes.at(1).value().toString()));
                                element = "instruction";
                            }
                            else if(reader.name() == "functions")
                            {
                                attributes = reader.attributes();
                                cFormat.setForeground(XmlSyntaxParser::intToRgb(attributes.at(0).value().toString().toInt()));
                                cFormat.setFontWeight(XmlSyntaxParser::isBold(attributes.at(1).value().toString()));
                                cFormat.setFontItalic(XmlSyntaxParser::isItalic(attributes.at(1).value().toString()));
                                element = "function";
                            }
                            else if(reader.name() == "operators")
                            {
                                attributes = reader.attributes();
                                cFormat.setForeground(XmlSyntaxParser::intToRgb(attributes.at(0).value().toString().toInt()));
                                cFormat.setFontWeight(XmlSyntaxParser::isBold(attributes.at(1).value().toString()));
                                cFormat.setFontItalic(XmlSyntaxParser::isItalic(attributes.at(1).value().toString()));
                                element = "operator";
                            }
                            else if(reader.name() == "symbols")
                            {
                                attributes = reader.attributes();
                                cFormat.setForeground(XmlSyntaxParser::intToRgb(attributes.at(0).value().toString().toInt()));
                                cFormat.setFontWeight(XmlSyntaxParser::isBold(attributes.at(1).value().toString()));
                                cFormat.setFontItalic(XmlSyntaxParser::isItalic(attributes.at(1).value().toString()));
                                element = "symbol";
                            }
                            else if(reader.name() == "singlecomments")
                            {
                                attributes = reader.attributes();
                                cFormat.setForeground(XmlSyntaxParser::intToRgb(attributes.at(0).value().toString().toInt()));
                                cFormat.setFontWeight(XmlSyntaxParser::isBold(attributes.at(1).value().toString()));
                                cFormat.setFontItalic(XmlSyntaxParser::isItalic(attributes.at(1).value().toString()));
                                element = "singlecomment";
                            }
                            else if(reader.name() == "multicomments")
                            {
                                attributes = reader.attributes();
                                cFormat.setForeground(XmlSyntaxParser::intToRgb(attributes.at(0).value().toString().toInt()));
                                cFormat.setFontWeight(XmlSyntaxParser::isBold(attributes.at(1).value().toString()));
                                cFormat.setFontItalic(XmlSyntaxParser::isItalic(attributes.at(1).value().toString()));
                                element = "multicomment";
                            }
                            else if(reader.name() == "quotations")
                            {
                                attributes = reader.attributes();
                                cFormat.setForeground(XmlSyntaxParser::intToRgb(attributes.at(0).value().toString().toInt()));
                                cFormat.setFontWeight(XmlSyntaxParser::isBold(attributes.at(1).value().toString()));
                                cFormat.setFontItalic(XmlSyntaxParser::isItalic(attributes.at(1).value().toString()));
                                element = "quotation";
                            }
                            else
                            {
                                if(error)
                                {
                                    *error = "Unnown XML element: " + reader.name().toString();
                                }

                                return false;
                            }

                            while(!reader.atEnd())
                            {
                                token = reader.readNext();

                                if(reader.hasError())
                                {
                                    if(error)
                                    {
                                        *error = reader.errorString();
                                    }

                                    return false;
                                }

                                if(token == QXmlStreamReader::StartElement && reader.name() == element)
                                {
                                    if(element == "function")
                                    {
                                        QRegExp rExp(reader.attributes().at(0).value().toString());
                                        rules.appendRule(element.toUtf8().constData(), HighlightingRule(rExp, cFormat));
                                    }
                                    else
                                    {
                                        QRegExp rExp(reader.readElementText());
                                        rules.appendRule(element.toUtf8().constData(), HighlightingRule(rExp, cFormat));
                                    }
                                }
                                else
                                {
                                    if(token == QXmlStreamReader::EndElement && reader.name() != element)
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    else
    {
        *error = "Unable to open file : " + file + "\n" + f.errorString();
        return false;
    }

    f.close();
    return true;
}
