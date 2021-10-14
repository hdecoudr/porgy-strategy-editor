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
    CodeEditorListView(QWidget* parent = nullptr, int zoom = 0, QAbstractItemModel* model = nullptr);
    ~CodeEditorListView();

protected:
    void wheelEvent(QWheelEvent* event) override;
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    void dragMoveEvent(QDragMoveEvent* event) override;

signals:
    void wheelZoomIn(int);
    void wheelZoomOut(int);

signals:
    void requestDropLoadSourceFiles(QStringList);

private slots:
    void zoomIn(int);
    void zoomOut(int);
    void zoomRestore();

private:
    int defaultZoom;
};


#endif // CODEEDITORLISTVIEW_H
