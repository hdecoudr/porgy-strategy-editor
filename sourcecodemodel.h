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

#ifndef SOURCECODEMODEL_H
#define SOURCECODEMODEL_H

#include "sourcecode.h"

#include <QString>
#include <QVector>
#include <QList>
#include <QAbstractListModel>
#include <QTextDocument>

class SourceCodeModel : public QAbstractListModel
{
public:
    enum SourceCodeErrors
    {
        SOURCE_CODE_NO_ERROR = 0,      
        SOURCE_CODE_EMPTY_NAME,      
        SOURCE_CODE_EXISTING_NAME,    
        SOURCE_CODE_INDEX_OUT_OF_RANGE
    };

public:
    SourceCodeModel(QObject* parent = nullptr);
    ~SourceCodeModel();

public:
    int count() const;
    bool empty() const;
    int addSourceCode(const QString& name, SourceCodeErrors& error);
    int addSourceCode(const QString& name, const QString& code, SourceCodeErrors& error);
    int removeSourceCode(int index);
    QString getSourceCodeName(int index) const;
    QString getSourceCode(int index) const;
    SourceCode getSourceCode(int index, SourceCodeErrors& error) const;
    QTextDocument& getSourceCodeDocument(int index);
    SourceCodeErrors setSourceCodeName(int index, const QString& newName);
    void setSourceCodeContent(int index, const QString& newCode);
    int indexOfSourceCode(const QString& sourceCodeName) const;
    void updateLatestSelected(int index, const QString& sourceCodeName, const QString& newCode);

public:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;

public:
    static void errorMessage(const QString& msg, SourceCodeErrors error);

private:
    QVector<std::pair<QString, QTextDocument*>> sourceCodes;
};

#endif // SOURCECODEMODEL_H
