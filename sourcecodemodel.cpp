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

#include "sourcecodemodel.h"

#include <typeinfo>
#include <iostream>

#include <QRegExp>

/****************************************************************************************
 ****************************************************************************************
 *
 * Default constructor.
 *
 ****************************************************************************************/
SourceCodeModel::SourceCodeModel(QObject* parent):QAbstractListModel(parent) {}

/****************************************************************************************
 ****************************************************************************************
 *
 * Default destructor.
 *
 ****************************************************************************************/
SourceCodeModel::~SourceCodeModel()
{
    for(int i = 0; i < sourceCodes.size(); ++i)
    {
        delete sourceCodes.at(i).second;
    }
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Get number of source code files.
 *
 ****************************************************************************************/
int SourceCodeModel::count() const
{
    return sourceCodes.size();
}

/****************************************************************************************
 ****************************************************************************************
 *
 * No source code file.
 *
 ****************************************************************************************/
bool SourceCodeModel::empty() const
{
    return sourceCodes.empty();
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Add an empty source code file.
 *
 ****************************************************************************************/
int SourceCodeModel::addSourceCode(const QString& name, SourceCodeErrors& error)
{
    return addSourceCode(name, "", error);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Add a source code file.
 *
 ****************************************************************************************/
int SourceCodeModel::addSourceCode(const QString& name, const QString& code, SourceCodeErrors& error)
{
    if(indexOfSourceCode(name) == -1)
    {
        if(name.isEmpty())
        {
            error = SOURCE_CODE_EMPTY_NAME;
            SourceCodeModel::errorMessage("SourceCodeModel::addSourceCode ", error);

            return -1;
        }
        else
        {
            int newIndex = sourceCodes.size();

            beginInsertRows(QModelIndex(), newIndex, newIndex);

            QTextDocument* document = new QTextDocument(this);
            document->setPlainText(code);
            document->setModified(false);
            sourceCodes.push_back(std::make_pair(name, document));

            endInsertRows();

            error = SOURCE_CODE_NO_ERROR;

            return newIndex;
        }
    }
    else
    {
        error = SOURCE_CODE_EXISTING_NAME;
        SourceCodeModel::errorMessage("SourceCodeModel::addSourceCode ", error);

        return -1;
    }
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Remove a source code file.
 *
 ****************************************************************************************/
int SourceCodeModel::removeSourceCode(int index)
{
    if(sourceCodes.size() < index)
    {
        SourceCodeModel::errorMessage("SourceCodeModel::removeSourceCode ", SourceCodeErrors::SOURCE_CODE_INDEX_OUT_OF_RANGE);
        return SOURCE_CODE_INDEX_OUT_OF_RANGE;
    }

    beginRemoveRows(QModelIndex(), index, index);

    sourceCodes[index].second->deleteLater();
    sourceCodes.remove(index);

    endRemoveRows();

    return SOURCE_CODE_NO_ERROR;
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Get source code name.
 *
 ****************************************************************************************/
QString SourceCodeModel::getSourceCodeName(int index) const
{
    if(sourceCodes.size() < index)
    {
        SourceCodeModel::errorMessage("SourceCodeModel::getSourceCodeName ", SourceCodeErrors::SOURCE_CODE_INDEX_OUT_OF_RANGE);
        return QString();
    }

    return sourceCodes[index].first;
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Get source code document.
 *
 ****************************************************************************************/
QTextDocument& SourceCodeModel::getSourceCodeDocument(int index)
{
    if(sourceCodes.size() < index)
    {
        SourceCodeModel::errorMessage("SourceCodeModel::getSourceCodeDocument ", SourceCodeErrors::SOURCE_CODE_INDEX_OUT_OF_RANGE);
    }   // Crash test.

    return *sourceCodes[index].second;
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Get source code.
 *
 ****************************************************************************************/
QString SourceCodeModel::getSourceCode(int index) const
{
    if(sourceCodes.size() < index)
    {
        SourceCodeModel::errorMessage("SourceCodeModel::getSourceCode ", SourceCodeErrors::SOURCE_CODE_INDEX_OUT_OF_RANGE);
        return QString();
    }

    return sourceCodes[index].second->toPlainText();
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Get source code object.
 *
 ****************************************************************************************/
SourceCode SourceCodeModel::getSourceCode(int index,SourceCodeErrors& error) const
{
    if(sourceCodes.size() < index)
    {
        error = SOURCE_CODE_INDEX_OUT_OF_RANGE;
        SourceCodeModel::errorMessage("SourceCodeModel::getSourceCode ", error);

        return SourceCode();
    }

    return SourceCode(getSourceCodeName(index), getSourceCode(index));
}


/****************************************************************************************
 ****************************************************************************************
 *
 * Set source code name.
 *
 ****************************************************************************************/
SourceCodeModel::SourceCodeErrors SourceCodeModel::setSourceCodeName(int currentIndex, const QString& newName)
{
    if(sourceCodes.size() < currentIndex)
    {
        SourceCodeModel::errorMessage("SourceCodeModel::setSourceCodeName ", SourceCodeErrors::SOURCE_CODE_INDEX_OUT_OF_RANGE);
        return SOURCE_CODE_INDEX_OUT_OF_RANGE;
    }

    if(!newName.isEmpty())
    {
        int existingStrategyIndex = indexOfSourceCode(newName);

        if(existingStrategyIndex == -1 || existingStrategyIndex == currentIndex )
        {
            sourceCodes[currentIndex].first = newName;
            emit dataChanged(index(currentIndex), index(currentIndex));

            return SOURCE_CODE_NO_ERROR;
        }
        else
        {
            SourceCodeModel::errorMessage("SourceCodeModel::setSourceCodeName ", SourceCodeErrors::SOURCE_CODE_EXISTING_NAME);
            return SOURCE_CODE_EXISTING_NAME;
        }
    }
    else
    {
        SourceCodeModel::errorMessage("SourceCodeModel::setSourceCodeName ", SourceCodeErrors::SOURCE_CODE_EMPTY_NAME);
        return SOURCE_CODE_EMPTY_NAME;
    }
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Set source code content.
 *
 ****************************************************************************************/
void SourceCodeModel::setSourceCodeContent(int currentIndex, const QString& newCode)
{
    if(currentIndex < 0 || sourceCodes.size() < currentIndex)
    {
        return;
    }

    sourceCodes[currentIndex].second->setPlainText(newCode);
    emit dataChanged(index(currentIndex), index(currentIndex));
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Get the number of source code files in model.
 *
 ****************************************************************************************/
int SourceCodeModel::rowCount(const QModelIndex&) const
{
    return count();
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Get the name of a source code file in model.
 *
 ****************************************************************************************/
QVariant SourceCodeModel::data(const QModelIndex& index, int role) const
{
    if(index.isValid())
    {
        if(role == Qt::DisplayRole)
        {
            return getSourceCodeName(index.row());
        }
    }

    return QVariant();
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Garbage.
 *
 ****************************************************************************************/
QVariant SourceCodeModel::headerData(int ,Qt::Orientation, int role) const
{
    if(role == Qt::DisplayRole)
    {
        return QVariant(QString("Name"));
    }

    return QVariant();
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Set the source code name.
 *
 ****************************************************************************************/
bool SourceCodeModel::setData(const QModelIndex& currentIndex, const QVariant& value, int role)
{
    if(currentIndex.isValid())
    {
        if(role == Qt::EditRole)
        {
            QString sourceCodeName = value.toString();
            SourceCodeErrors errorCode = setSourceCodeName(currentIndex.row(), sourceCodeName);

            if(errorCode == SOURCE_CODE_NO_ERROR)
            {
                return true;
            }
            else
            {
                SourceCodeModel::errorMessage("SourceCodeModel::setData ", errorCode);
                return false;
            }
        }
    }

    return false;
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Allow edition and DND.
 *
 ****************************************************************************************/
Qt::ItemFlags SourceCodeModel::flags(const QModelIndex& index) const
{
    return (QAbstractListModel::flags(index) | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | Qt::ItemIsEditable);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Get index of source code file.
 *
 ****************************************************************************************/
int SourceCodeModel::indexOfSourceCode(const QString& strategyName) const
{
    for(int i = 0; i <  sourceCodes.size(); ++i)
    {
        if(sourceCodes[i].first.compare(strategyName) == 0)
        {
            return i;
        }
    }

    return -1;
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Update last.
 *
 ****************************************************************************************/
void SourceCodeModel::updateLatestSelected(int index, const QString& sourceCodeName, const QString& newCode)
{
    if(index < 0 || sourceCodes.count() < index)
    {
        SourceCodeModel::errorMessage("SourceCodeModel::updateLatestSelected ", SourceCodeErrors::SOURCE_CODE_INDEX_OUT_OF_RANGE);
        return;
    }

    sourceCodes[index].first = sourceCodeName;
    sourceCodes[index].second->setPlainText(newCode);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Display enum error.
 *
 ****************************************************************************************/
void SourceCodeModel::errorMessage(const QString& msg, SourceCodeErrors error)
{
    switch(error)
    {
        case SOURCE_CODE_EMPTY_NAME:
        {
            std::cerr << msg.toUtf8().constData() << "SOURCE_CODE_EMPTY_NAME\n\tThe source code cannot have an empty name\n";
            break;
        }

        case SOURCE_CODE_EXISTING_NAME:
        {
            std::cerr << msg.toUtf8().constData() << "SOURCE_CODE_EXISTING_NAME\n\tThe source code must have unique names";
            break;
        }

        case SOURCE_CODE_INDEX_OUT_OF_RANGE:
        {
            std::cerr << msg.toUtf8().constData() << "SOURCE_CODE_INDEX_OUT_OF_RANGE\n\tThe selected source code is out of range\n";
            break;
        }

        default:
            break;
    }
}
