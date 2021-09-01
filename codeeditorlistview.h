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

#ifndef CODEEDITORLISTVIEW_H
#define CODEEDITORLISTVIEW_H

#include <QListView>

class CodeEditorListView : public QListView
{
    Q_OBJECT

public:
    CodeEditorListView(QWidget* parent = 0, int zoom = 0, QAbstractItemModel* model = 0);
    ~CodeEditorListView();

protected:
    void wheelEvent(QWheelEvent* event);
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);
    void dragMoveEvent(QDragMoveEvent* event);

signals:
    void wheelZoomIn(int);
    void wheelZoomOut(int);

signals:
    void requestDropLoadSourceFiles(QStringList);

private slots:
    virtual void zoomIn(int);
    virtual void zoomOut(int);
    virtual void zoomRestore();

private:
    int defaultZoom;
};

#endif // CODEEDITORLISTVIEW_H
