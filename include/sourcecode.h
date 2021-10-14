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

#ifndef SOURCECODE_H
#define SOURCECODE_H

#include <QString>


class SourceCode
{
public:
    SourceCode();
    SourceCode(const QString& name, const QString& code = QString());
    ~SourceCode();

public:
    inline QString getName() const
    {
        return name;
    }

    inline QString getCode() const
    {
        return code;
    }

    inline void setName(const QString& sourceName)
    {
        name = sourceName;
    }

    inline void setCode(const QString& sourceCode)
    {
        code = sourceCode;
    }

    inline bool isNameAndCodeNotEmpty() const
    {
        return (!name.isEmpty() && !code.isEmpty());
    }

public:
    QByteArray toByteArray() const;
	
public:
    static SourceCode fromByteArray(const QByteArray& byteArray);

private:
    QString name;   
    QString code;    
};


#endif // SOURCECODE_H
