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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "codeeditorview.h"

#include <QScreen>


/****************************************************************************************
 ****************************************************************************************
 *
 * Default constructor.
 *
 ****************************************************************************************/
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowSizeAndPos();

    QString syntax = ":/syntax/code-editor/porgy-script.xml";
    CodeEditorView* codeEditor = new CodeEditorView(&model, &syntax, this);

    setCentralWidget(codeEditor);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Default destructor.
 *
 ****************************************************************************************/
MainWindow::~MainWindow()
{
    delete ui;
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Size and center.
 *
 ****************************************************************************************/
void MainWindow::setWindowSizeAndPos()
{
    QRect screenRect = QGuiApplication::primaryScreen()->geometry();
    int w = screenRect.width() / 2;
    int h = screenRect.height() / 2;

    setGeometry(w / 2, h / 2, w, h);
}
