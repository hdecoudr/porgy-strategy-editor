#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "codeeditorview.h"

#include <QDesktopWidget>


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
    QRect screenRect = QApplication::desktop()->screenGeometry();
    int w = screenRect.width() / 2;
    int h = screenRect.height() / 2;

    setGeometry(w / 2, h / 2, w, h);
}
