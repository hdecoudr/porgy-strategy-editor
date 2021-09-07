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

#include "codeeditorview.h"
#include "ui_codeeditorview.h"

#include "codeeditorlistview.h"
#include "codeeditorplaintextview.h"
#include "xmlsyntaxparser.h"
#include "sourcecodemodel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>
#include <QToolButton>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QSplitter>
#include <QMenu>

/****************************************************************************************
 ****************************************************************************************
 *
 * Constructor.
 *
 ****************************************************************************************/
CodeEditorView::CodeEditorView(SourceCodeModel* model, const QString* syntaxFile, QWidget* parent) : QWidget(parent), ui(new Ui::CodeEditor), deleteLast(false), sourceCodeModel(model)
{
    ui->setupUi(this);

    mainVBox = new QVBoxLayout(this);
    mainVBox->setSpacing(0);
    mainHBox = new QHBoxLayout();

    initToolBar();
    initToolBarButtons();

    codeEditorListView = new CodeEditorListView(this);
    codeEditorListView->setMinimumWidth(codeEditorListView->width() * 10.0f / 100.0f);

    codeEditorPlainTextView = new CodeEditorPlainTextView(this, 0, syntaxFile);
    codeEditorPlainTextView->setMinimumWidth(codeEditorPlainTextView->width() * 10.0f / 100.0f);
    codeEditorPlainTextView->setEnabled(false);

    QSplitter* splitter = new QSplitter(this);
    splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    splitter->addWidget(codeEditorListView);
    splitter->addWidget(codeEditorPlainTextView);

    QString name = ":/styles/code-editor/splitter/splitter.qss";
    QFile f(name);

    if(f.open(QFile::ReadOnly))
    {
        splitter->setStyleSheet(f.readAll());
    }
    else
    {
        QMessageBox::warning(this, "I/O Error", "Unable to load file: " + name);
    }

    QList<int> widgetsWidth = splitter->sizes();
    widgetsWidth[0] = codeEditorListView->width() - (codeEditorListView->width() * 50.0f / 100.0f);
    widgetsWidth[1] = codeEditorListView->width() + (codeEditorListView->width() * 50.0f / 100.0f);
    splitter->setSizes(widgetsWidth);

    mainHBox->addWidget(splitter);
    mainVBox->addLayout(mainHBox);

    if(sourceCodeModel)
    {
        codeEditorListView->setModel(sourceCodeModel);
    }

    initConnections();

    setAcceptDrops(true);
    codeEditorListView->setDragEnabled(true);
    codeEditorListView->setAcceptDrops(true);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Destructor.
 *
 ****************************************************************************************/
CodeEditorView::~CodeEditorView()
{
    delete ui;
    //delete sourceCodeModel; Not needed anymore.
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Insert more widgets.
 *
 ****************************************************************************************/
void CodeEditorView::insertWidget(QWidget* widget, WidgetPosition where)
{
    switch(where)
    {
        case WidgetPosition::WIDGET_POSITION_TOP:
        {
            widget->setParent(this);
            mainVBox->insertWidget(0, widget);

            break;
        }

        case WidgetPosition::WIDGET_POSITION_BOTTOM:
        {
            widget->setParent(this);
            mainVBox->addWidget(widget);

            break;
        }

        case WidgetPosition::WIDGET_POSITION_LEFT:
        {
            widget->setParent(this);
            mainHBox->insertWidget(0, widget);

            break;
        }

        case WidgetPosition::WIDGET_POSITION_RIGHT:
        {
            widget->setParent(this);
            mainHBox->addWidget(widget);

            break;
        }
    }
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Display current source code.
 *
 ****************************************************************************************/
void CodeEditorView::displaySourceCode(int modelIndex)
{
    setCodeEditorPlainTextViewCode(sourceCodeModel->getSourceCode(modelIndex));
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Get content of code edititing area.
 *
 ****************************************************************************************/
QString CodeEditorView::getCodeEditorPlainTextViewCode() const
{
    return codeEditorPlainTextView->document()->toPlainText();
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Set code editor code.
 *
 ****************************************************************************************/
void CodeEditorView::setCodeEditorPlainTextViewCode(const QString &code)
{
    codeEditorPlainTextView->setPlainText(code);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Set list model.
 *
 ****************************************************************************************/
void CodeEditorView::setCodeEditorListViewModel(QAbstractListModel* model)
{
    sourceCodeModel = static_cast<SourceCodeModel*>(model);
    codeEditorListView->setModel(model);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Tool bar initialization.
 *
 ****************************************************************************************/
void CodeEditorView::initToolBar()
{
    toolBar = new QToolBar();

    mainVBox->addWidget(toolBar);
    mainVBox->setAlignment(toolBar, Qt::AlignTop);

    QString name = ":/styles/code-editor/toolbar/toolbar.qss";
    QFile f(name);

    if(f.open(QFile::ReadOnly))
    {
        toolBar->setStyleSheet(f.readAll());
    }
    else
    {
        QMessageBox::warning(this, "I/O Error", "Unable to load file: " + name);
    }
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Tool bar buttons initialization.
 *
 ****************************************************************************************/
void CodeEditorView::initToolBarButtons()
{
    toolButtonRun = new QToolButton();

    toolButtonNew = new QToolButton();
    toolButtonNew->setPopupMode(QToolButton::InstantPopup);

    toolButtonSave = new QToolButton();
    toolButtonSave->setPopupMode(QToolButton::InstantPopup);

    toolButtonOpen = new QToolButton();
    toolButtonOpen->setPopupMode(QToolButton::InstantPopup);

    toolButtonUndo = new QToolButton();
    toolButtonRedo = new QToolButton();
    toolButtonCopy = new QToolButton();
    toolButtonPaste = new QToolButton();
    toolButtonZoomIn = new QToolButton();
    toolButtonZoomOut = new QToolButton();
    toolButtonZoomOriginal = new QToolButton();

    toolButtonClear = new QToolButton();

    toolButtonDelete = new QToolButton();
    toolButtonDelete->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Delete));

    toolButtonRun->setToolTip("Run");
    toolButtonNew->setToolTip("New");
    toolButtonOpen->setToolTip("Open");
    toolButtonSave->setToolTip("Save");
    toolButtonUndo->setToolTip("Undo");
    toolButtonRedo->setToolTip("Redo");
    toolButtonCopy->setToolTip("Copy");
    toolButtonPaste->setToolTip("Paste");
    toolButtonZoomIn->setToolTip("Zoom-in");
    toolButtonZoomOut->setToolTip("Zoom-out");
    toolButtonZoomOriginal->setToolTip("Zoom-restore");
    toolButtonClear->setToolTip("Clear");
    toolButtonDelete->setToolTip("Delete");

    QString name = ":/styles/code-editor/toolbar/items/toolbaritems.qss";
    QFile f(name);

    if(f.open(QFile::ReadOnly))
    {
        QString commonStyles = f.readAll();

        toolButtonRun->setIcon(QIcon(":/icons/code-editor/run.svg"));
        toolButtonRun->setStyleSheet(commonStyles);

        toolButtonNew->setIcon(QIcon(":/icons/code-editor/new.svg"));
        toolButtonNew->setStyleSheet(commonStyles);

        toolButtonOpen->setIcon(QIcon(":/icons/code-editor/open.svg"));
        toolButtonOpen->setStyleSheet(commonStyles);

        toolButtonSave->setIcon(QIcon(":/icons/code-editor/save.svg"));
        toolButtonSave->setStyleSheet(commonStyles);

        toolButtonUndo->setIcon(QIcon(":/icons/code-editor/undo.svg"));
        toolButtonUndo->setStyleSheet(commonStyles);

        toolButtonRedo->setIcon(QIcon(":/icons/code-editor/redo.svg"));
        toolButtonRedo->setStyleSheet(commonStyles);

        toolButtonCopy->setIcon(QIcon(":/icons/code-editor/copy.svg"));
        toolButtonCopy->setStyleSheet(commonStyles);

        toolButtonPaste->setIcon(QIcon(":/icons/code-editor/paste.svg"));
        toolButtonPaste->setStyleSheet(commonStyles);

        toolButtonZoomIn->setIcon(QIcon(":/icons/code-editor/zoom-in.svg"));
        toolButtonZoomIn->setStyleSheet(commonStyles);

        toolButtonZoomOut->setIcon(QIcon(":/icons/code-editor/zoom-out.svg"));
        toolButtonZoomOut->setStyleSheet(commonStyles);

        toolButtonZoomOriginal->setIcon(QIcon(":/icons/code-editor/zoom-original.svg"));
        toolButtonZoomOriginal->setStyleSheet(commonStyles);

        toolButtonClear->setIcon(QIcon(":/icons/code-editor/clear.svg"));
        toolButtonClear->setStyleSheet(commonStyles);

        toolButtonDelete->setIcon(QIcon(":/icons/code-editor/delete.svg"));
        toolButtonDelete->setStyleSheet(commonStyles);
    }
    else
    {
        QMessageBox::warning(this, "I/O Error", "Unable to load file: " + name);
    }

    menuNew = new QMenu(this);
    menuOpen = new QMenu(this);
    menuSave = new QMenu(this);

    actionNew = new QAction("New source", menuNew);
    actionNew->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));

    actionDuplicate = new QAction("Duplicate selected source", menuNew);
    actionDuplicate->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_D));

    actionOpen = new QAction("Open", menuOpen);
    actionOpen->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));

    actionOpenAll = new QAction("Open all", menuOpen);
    actionOpenAll->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_O));

    actionSave = new QAction("Save selected source", menuSave);
    actionSave->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));

    actionSaveAll = new QAction("Save all", menuSave);
    actionSaveAll->setShortcut(QKeySequence(Qt::CTRL + Qt::ALT + Qt::Key_S));

    menuNew->addAction(actionNew);
    menuNew->addAction(actionDuplicate);
    menuOpen->addAction(actionOpen);
    menuOpen->addAction(actionOpenAll);
    menuSave->addAction(actionSave);
    menuSave->addAction(actionSaveAll);

    toolBar->addWidget(toolButtonRun);
    toolBar->addSeparator();
    toolBar->addWidget(toolButtonNew);
    toolBar->addWidget(toolButtonOpen);
    toolBar->addWidget(toolButtonSave);
    toolBar->addSeparator();
    toolBar->addWidget(toolButtonUndo);
    toolBar->addWidget(toolButtonRedo);
    toolBar->addSeparator();
    toolBar->addWidget(toolButtonCopy);
    toolBar->addWidget(toolButtonPaste);
    toolBar->addSeparator();
    toolBar->addWidget(toolButtonZoomIn);
    toolBar->addWidget(toolButtonZoomOut);
    toolBar->addWidget(toolButtonZoomOriginal);
    toolBar->addSeparator();
    toolBar->addWidget(toolButtonClear);
    toolBar->addWidget(toolButtonDelete);

    toolButtonNew->setMenu(menuNew);
    toolButtonOpen->setMenu(menuOpen);
    toolButtonSave->setMenu(menuSave);

    toolButtonRun->setEnabled(false);
    toolButtonUndo->setEnabled(false);
    toolButtonRedo->setEnabled(false);
    toolButtonCopy->setEnabled(false);
    toolButtonPaste->setEnabled(false);
    toolButtonSave->setEnabled(false);
    toolButtonClear->setEnabled(false);
    toolButtonDelete->setEnabled(false);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Connections setup.
 *
 ****************************************************************************************/
void CodeEditorView::initConnections()
{
    connect(codeEditorPlainTextView, SIGNAL(undoAvailable(bool)), this, SLOT(undoEnable(bool)));
    connect(codeEditorPlainTextView, SIGNAL(redoAvailable(bool)), this, SLOT(redoEnable(bool)));
    connect(codeEditorPlainTextView, SIGNAL(copyAvailable(bool)), this, SLOT(copyEnable(bool)));

    connect(toolButtonUndo, SIGNAL(clicked(bool)), codeEditorPlainTextView, SLOT(undo()));
    connect(toolButtonRedo, SIGNAL(clicked(bool)), codeEditorPlainTextView, SLOT(redo()));

    connect(toolButtonCopy, SIGNAL(clicked(bool)), codeEditorPlainTextView, SLOT(copy()));
    connect(toolButtonPaste, SIGNAL(clicked(bool)), codeEditorPlainTextView, SLOT(paste()));

    connect(actionSave, SIGNAL(triggered(bool)), this, SLOT(save()));
    connect(actionSaveAll, SIGNAL(triggered(bool)), this, SLOT(saveAll()));
    connect(toolButtonClear, SIGNAL(clicked(bool)), this, SLOT(clear()));
    connect(toolButtonZoomIn, SIGNAL(clicked(bool)), this, SLOT(zoomIn()));
    connect(toolButtonZoomOut, SIGNAL(clicked(bool)), this, SLOT(zoomOut()));
    connect(toolButtonZoomOriginal, SIGNAL(clicked(bool)), codeEditorPlainTextView, SLOT(zoomRestore()));
    connect(toolButtonZoomOriginal, SIGNAL(clicked(bool)), codeEditorListView, SLOT(zoomRestore()));

    connect(this, SIGNAL(zoomIn(int)), codeEditorPlainTextView, SLOT(zoomIn(int)));
    connect(this, SIGNAL(zoomIn(int)), codeEditorListView, SLOT(zoomIn(int)));
    connect(this, SIGNAL(zoomOut(int)), codeEditorPlainTextView, SLOT(zoomOut(int)));
    connect(this, SIGNAL(zoomOut(int)), codeEditorListView, SLOT(zoomOut(int)));

    connect(codeEditorPlainTextView, SIGNAL(wheelZoomIn(int)), codeEditorPlainTextView, SLOT(zoomIn(int)));
    connect(codeEditorPlainTextView, SIGNAL(wheelZoomOut(int)), codeEditorPlainTextView, SLOT(zoomOut(int)));
    connect(codeEditorPlainTextView, SIGNAL(wheelZoomIn(int)), codeEditorListView, SLOT(zoomIn(int)));
    connect(codeEditorPlainTextView, SIGNAL(wheelZoomOut(int)), codeEditorListView, SLOT(zoomOut(int)));

    connect(codeEditorListView, SIGNAL(wheelZoomIn(int)), codeEditorListView, SLOT(zoomIn(int)));
    connect(codeEditorListView, SIGNAL(wheelZoomOut(int)), codeEditorListView, SLOT(zoomOut(int)));
    connect(codeEditorListView, SIGNAL(wheelZoomIn(int)), codeEditorPlainTextView, SLOT(zoomIn(int)));
    connect(codeEditorListView, SIGNAL(wheelZoomOut(int)), codeEditorPlainTextView, SLOT(zoomOut(int)));

    if(sourceCodeModel)
    {
        connect(actionNew, SIGNAL(triggered(bool)), this, SLOT(addNewEmptySourceCode()));
        connect(actionDuplicate, SIGNAL(triggered(bool)), this, SLOT(duplicateSourceCode()));
        connect(toolButtonDelete, SIGNAL(clicked(bool)), this, SLOT(deleteSourceCode()));

        connect(actionOpen, SIGNAL(triggered(bool)), this, SLOT(open()));
        connect(actionOpenAll, SIGNAL(triggered(bool)), this, SLOT(openAll()));
        connect(codeEditorListView, SIGNAL(requestDropLoadSourceFiles(QStringList)), this, SLOT(openDrop(QStringList)));

        connect(codeEditorListView->selectionModel(), SIGNAL(currentChanged(const QModelIndex&, const QModelIndex&)), this, SLOT(selectedSourceCodeChanged(const QModelIndex&, const QModelIndex&)));
    }
    else
    {
        QMessageBox::warning(this, "No model defined", "No model is associated with the source code editor !");
    }
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Enable code editing area.
 *
 ****************************************************************************************/
void CodeEditorView::codeEditorPlainTextViewEnable(bool value)
{
    codeEditorPlainTextView->setEnabled(value);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Enable undo.
 *
 ****************************************************************************************/
void CodeEditorView::duplicateEnable(bool value)
{
    actionDuplicate->setEnabled(value);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Enable undo.
 *
 ****************************************************************************************/
void CodeEditorView::undoEnable(bool value)
{
    toolButtonUndo->setEnabled(value);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Enable Redo.
 *
 ****************************************************************************************/
void CodeEditorView::redoEnable(bool value)
{
    toolButtonRedo->setEnabled(value);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Enable copy.
 *
 ****************************************************************************************/
void CodeEditorView::copyEnable(bool value)
{
    toolButtonCopy->setEnabled(value);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Enable paste.
 *
 ****************************************************************************************/
void CodeEditorView::pasteEnable(bool value)
{
    toolButtonPaste->setEnabled(value);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Enable save.
 *
 ****************************************************************************************/
void CodeEditorView::saveEnable(bool value)
{
    toolButtonSave->setEnabled(value);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Enable clear.
 *
 ****************************************************************************************/
void CodeEditorView::clearEnable(bool value)
{
    toolButtonClear->setEnabled(value);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Enable delete.
 *
 ****************************************************************************************/
void CodeEditorView::deleteEnable(bool value)
{
    toolButtonDelete->setEnabled(value);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Open strategy.
 *
 ****************************************************************************************/
void CodeEditorView::open()
{
    QStringList fNames = QFileDialog::getOpenFileNames(this, "Open", "", "Strategy File (*.sty)");
    openDrop(fNames);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Drop open.
 *
 ****************************************************************************************/
void CodeEditorView::openDrop(QStringList fileNames)
{
    QFile f;
    QFileInfo fInfo;
    QTextStream o;

    int index = -1;

    for(int i = 0; i < fileNames.size(); ++i)
    {
        f.setFileName(fileNames.at(i));

        if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::information(this, "Unable to open file", f.errorString());
            return;
        }

        o.setDevice(&f);
        QString content = o.readAll();

        index = sourceCodeModel->indexOfSourceCode(fileNames.at(i));
        fInfo.setFile(f);

        if(index == -1)
        {
            SourceCodeModel::SourceCodeErrors error;
            index = sourceCodeModel->addSourceCode(fInfo.completeBaseName(), content, error);
        }
        else
        {
            int overwrite = QMessageBox::question(this, "Overwrite source?", "A source with the same name already exists.\n Are you sure you want to overwrite it?", QMessageBox::Yes, QMessageBox::No);

            if(overwrite == QMessageBox::Yes)
            {
                sourceCodeModel->setSourceCodeContent(index, content);
            }
        }

        f.close();
    }

    if(sourceCodeModel->count() == 1)
    {
        codeEditorPlainTextViewEnable(true);
        duplicateEnable(true);
        pasteEnable(true);
        saveEnable(true);
        clearEnable(true);
        deleteEnable(true);
    }

    if(index > -1 && index < sourceCodeModel->count())
    {
        codeEditorListView->selectionModel()->setCurrentIndex(sourceCodeModel->index(index), QItemSelectionModel::ClearAndSelect);
    }
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Open all files.
 *
 ****************************************************************************************/
void CodeEditorView::openAll()
{
    QString dName = QFileDialog::getExistingDirectory(this, "Open all");
    QDir directory(dName);

    QStringList list;

    if(directory.exists())
    {
        foreach(QString file, directory.entryList(QDir::Files | QDir::NoDot | QDir::NoDotDot, QDir::Name))
        {
            if(file.contains(".sty"))
            {
                list << directory.absoluteFilePath(file);
            }
        }

        if(!list.empty())
        {
            openDrop(list);
        }
    }
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Save strategy.
 *
 ****************************************************************************************/
void CodeEditorView::save()
{
    QString fName = QFileDialog::getSaveFileName(this, "Save", codeEditorListView->selectionModel()->currentIndex().data().toString(), "Strategy File (*.sty)");

    if(!fName.isEmpty())
    {
        if(fName.size() < 3)
        {
            fName.append(".sty");
        }
        else
        {
            QString sub = fName.right(4);

            if(sub != ".sty")
            {
                fName.append(".sty");
            }
        }

        QFile f(fName);

        if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QMessageBox::information(this, "Unable to open file", f.errorString());
            return;
        }

        QTextStream o(&f);
        o << codeEditorPlainTextView->toPlainText().toUtf8().constData();
    }
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Save all sources.
 *
 ****************************************************************************************/
void CodeEditorView::saveAll()
{
    QString dName = QFileDialog::getExistingDirectory(this, "Save all");

    if(!dName.isEmpty())
    {
        QFile       f;
        QTextStream o;
        QString     savePath;

        for(int i = 0; i < sourceCodeModel->count(); ++i)
        {
            if(i == codeEditorListView->selectionModel()->currentIndex().row())
            {
                sourceCodeModel->updateLatestSelected(i, codeEditorListView->selectionModel()->currentIndex().data().toString(), codeEditorPlainTextView->document()->toPlainText());
            }

            savePath = dName + "/" + sourceCodeModel->getSourceCodeName(i) + ".sty";
            f.setFileName(savePath);

            if(!f.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                QMessageBox::information(this, "Unable to open file", f.errorString());
                continue;
            }

            o.setDevice(&f);
            o << sourceCodeModel->getSourceCode(i);

            f.close();
        }
    }
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Zoom-in emitter.
 *
 ****************************************************************************************/
void CodeEditorView::zoomIn()
{
    emit zoomIn(1);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Zoom-out emitter.
 *
 ****************************************************************************************/
void CodeEditorView::zoomOut()
{
    emit zoomOut(1);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Clear code area.
 *
 ****************************************************************************************/
void CodeEditorView::clear()
{
    QString text = codeEditorPlainTextView->toPlainText();

    if(!text.isEmpty())
    {
        int res = QMessageBox::question(this, "Clear code?", "The content of the code editor is non-empty.\nAre you sure you want to clear it?", QMessageBox::Yes, QMessageBox::No);

        if(res == QMessageBox::Yes)
        {
            codeEditorPlainTextView->clear();
        }
    }
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Delete source code.
 *
 ****************************************************************************************/
void CodeEditorView::deleteSourceCode()
{
    int res = QMessageBox::question(this, "Delete code?", "The code file and its associeted content will be deleted.\nAre you sure you want to delete it?", QMessageBox::Yes, QMessageBox::No);

    if(res == QMessageBox::Yes)
    {
        deleteLast = (sourceCodeModel->count() == 1);
        sourceCodeModel->removeSourceCode(codeEditorListView->selectionModel()->currentIndex().row());

        if(deleteLast)
        {
            codeEditorPlainTextViewEnable(false);
            duplicateEnable(false);
            pasteEnable(false);
            saveEnable(false);
            clearEnable(false);
            deleteEnable(false);
        }
    }
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Add empty source code.
 *
 ****************************************************************************************/
void CodeEditorView::addNewEmptySourceCode()
{
    addNewSourceCode("New source", "");
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Add Source code.
 *
 ****************************************************************************************/
void CodeEditorView::addNewSourceCode(QString name, QString code)
{
    deleteLast = false;

    QString origName(name);
    int cpt = 0;

    while(sourceCodeModel->indexOfSourceCode(name) != -1)
    {
        ++cpt;
        name = origName + QString("_") + QString::number(cpt);
    }

    SourceCodeModel::SourceCodeErrors error;
    int index = sourceCodeModel->addSourceCode(name, code, error);
    codeEditorListView->selectionModel()->setCurrentIndex(sourceCodeModel->index(index), QItemSelectionModel::ClearAndSelect);

    if(sourceCodeModel->count() == 1)
    {
        codeEditorPlainTextViewEnable(true);
        duplicateEnable(true);
        pasteEnable(true);
        saveEnable(true);
        clearEnable(true);
        deleteEnable(true);
    }
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Duplicate source code.
 *
 ****************************************************************************************/
void CodeEditorView::duplicateSourceCode()
{
    addNewSourceCode(sourceCodeModel->getSourceCodeName(codeEditorListView->selectionModel()->currentIndex().row()), getCodeEditorPlainTextViewCode());
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Update model with written source code.
 *
 ****************************************************************************************/
void CodeEditorView::selectedSourceCodeChanged(const QModelIndex& current, const QModelIndex& previous)
{
    if(!deleteLast)
    {
        if(sourceCodeModel->count() > previous.row())
        {
            sourceCodeModel->setSourceCodeContent(previous.row(), codeEditorPlainTextView->document()->toPlainText());
        }

        displaySourceCode(current.row());
    }
    else
    {
        codeEditorPlainTextView->clear();
    }
}
