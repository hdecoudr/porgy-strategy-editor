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

#include "sourcecode.h"

#include <QDataStream>

/****************************************************************************************
 ****************************************************************************************
 *
 * Blank constructor.
 *
 ****************************************************************************************/
SourceCode::SourceCode() {}

/****************************************************************************************
 ****************************************************************************************
 *
 * Default constructor.
 *
 ****************************************************************************************/
SourceCode::SourceCode(const QString& name, const QString& code) : name(name), code(code) {}

/****************************************************************************************
 ****************************************************************************************
 *
 * Default destructor.
 *
 ****************************************************************************************/
SourceCode::~SourceCode() {}

/****************************************************************************************
 ****************************************************************************************
 *
 * Name and code to QByteArray.
 *
 ****************************************************************************************/
QByteArray SourceCode::toByteArray() const
{
    QByteArray data;
    QDataStream stream(&data,QIODevice::WriteOnly);
    stream<< name << code;

    return data;
}

/****************************************************************************************
 ****************************************************************************************
 *
 * SourceCode from QByteArray.
 *
 ****************************************************************************************/
SourceCode SourceCode::fromByteArray(const QByteArray& byteArray)
{
    QDataStream stream(byteArray);
    QString sourceName;
    QString sourceCode;
    stream >> sourceName >> sourceCode;

    return SourceCode(sourceName, sourceCode);
}
