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

#include "codeeditorlistview.h"

#include <QWheelEvent>
#include <QUrl>
#include <QMessageBox>
#include <QMimeData>

/****************************************************************************************
 ****************************************************************************************
 *
 * Default constructor.
 *
 ****************************************************************************************/
CodeEditorListView::CodeEditorListView(QWidget* parent, int zoom, QAbstractItemModel* model) : QListView(parent)
{
    QString name = ":/styles/code-editor/listview/items/listviewitems.qss";
    QFile f(name);

    if(f.open(QIODevice::ReadOnly))
    {
        setStyleSheet(f.readAll());
        f.close();
    }
    else
    {
        QMessageBox::warning(this, "I/O Error", "Unable to load file: " + name);
    }

    if(model)
    {
        setModel(model);
    }

    defaultZoom = font().pointSize() + zoom;
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Default destructor.
 *
 ****************************************************************************************/
CodeEditorListView::~CodeEditorListView() {}

/****************************************************************************************
 ****************************************************************************************
 *
 * Zoom-in/Zoom-out and scroll with the mouse wheel.
 *
 ****************************************************************************************/
void CodeEditorListView::wheelEvent(QWheelEvent* event)
{
    if(event->delta() > 0 && event->modifiers() == Qt::ControlModifier)
    {
        emit wheelZoomIn(1);
    }
    else if(event->modifiers() == Qt::ControlModifier)
    {
        emit wheelZoomOut(1);
    }
    else
    {
        QListView::wheelEvent(event);
    }
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Accept everything
 *
 ****************************************************************************************/
void CodeEditorListView::dragEnterEvent(QDragEnterEvent* event)
{
    event->acceptProposedAction();
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Drop.
 *
 ****************************************************************************************/
void CodeEditorListView::dropEvent(QDropEvent* event)
{
    if(event->mimeData()->hasUrls())
    {
        QList<QUrl> urls = event->mimeData()->urls();
        QStringList list;
        QString     tmp;

        for(int i = 0; i < urls.size(); ++i)
        {
            tmp = urls.at(i).toString();
            tmp = tmp.mid(7, tmp.size() - 1);

            if(tmp.indexOf(".sty") == tmp.size() - 4)
            {
                list << tmp;
            }
        }

        if(!list.isEmpty())
        {
            emit requestDropLoadSourceFiles(list);
        }
    }
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Weird fix for the 2 DND functions above.
 *
 ****************************************************************************************/
void CodeEditorListView::dragMoveEvent(QDragMoveEvent* event)
{
    event->acceptProposedAction();
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Zoom-in.
 *
 ****************************************************************************************/
void CodeEditorListView::zoomIn(int range)
{
    QFont f = font();
    const int newSize = f.pointSize() + range;

    if(newSize <= 0)
    {
        return;
    }

    f.setPointSize(newSize);
    setFont(f);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * zoom-out.
 *
 ****************************************************************************************/
void CodeEditorListView::zoomOut(int range)
{
    zoomIn(-range);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * zoom-restore.
 *
 ****************************************************************************************/
void CodeEditorListView::zoomRestore()
{
    QFont f = font();
    f.setPointSize(defaultZoom);
    setFont(f);
}
