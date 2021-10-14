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

#ifndef CODEEDITORVIEW_H
#define CODEEDITORVIEW_H

#include <QWidget>
#include <QModelIndex>

class QVBoxLayout;
class QHBoxLayout;
class QToolBar;
class QToolButton;
class QAbstractListModel;
class QMenu;
class QAction;

class CodeEditorListView;
class CodeEditorPlainTextView;
class SourceCodeModel;

namespace Ui {
class CodeEditor;
}


enum WidgetPosition
{
    WIDGET_POSITION_TOP = 0,    
    WIDGET_POSITION_BOTTOM,     
    WIDGET_POSITION_LEFT,       
    WIDGET_POSITION_RIGHT       
};


class CodeEditorView : public QWidget
{
    Q_OBJECT

public:
    explicit CodeEditorView(SourceCodeModel* model, const QString* syntaxFile = nullptr, QWidget* parent = nullptr);
    ~CodeEditorView();

public:
    void insertWidget(QWidget* widget, WidgetPosition where);
    QString getCodeEditorPlainTextViewCode() const;
    void setCodeEditorPlainTextViewCode(const QString& code);
    void setCodeEditorListViewModel(QAbstractListModel* model);

signals:
    void zoomIn(int);
    void zoomOut(int);

private slots:
    void codeEditorPlainTextViewEnable(bool);
    void duplicateEnable(bool);
    void undoEnable(bool);
    void redoEnable(bool);
    void copyEnable(bool);
    void pasteEnable(bool);
    void saveEnable(bool);
    void clearEnable(bool);
    void deleteEnable(bool);
    void open();
    void openDrop(QStringList fileNames);
    void openAll();
    void save();
    void saveAll();
    void zoomIn();
    void zoomOut();
    void clear();
    void addNewEmptySourceCode();
    void addNewSourceCode(QString name, QString code);
    void duplicateSourceCode();
    void selectedSourceCodeChanged(const QModelIndex& current, const QModelIndex& previous);
    void deleteSourceCode();

private:
    void initToolBar();
    void initToolBarButtons();
    void initConnections();
    void displaySourceCode(int modelIndex);

private:
    Ui::CodeEditor* ui;                         

private:
    bool deleteLast;               

private:
    QVBoxLayout* mainVBox;                  
    QHBoxLayout* mainHBox;                  

private:
    QToolBar* toolBar;                 

private:
    QToolButton* toolButtonRun;             
    QToolButton* toolButtonNew;            
    QToolButton* toolButtonOpen;            
    QToolButton* toolButtonUndo;            
    QToolButton* toolButtonRedo;            
    QToolButton* toolButtonCopy;            
    QToolButton* toolButtonPaste;           
    QToolButton* toolButtonSave;           
    QToolButton* toolButtonZoomIn;          
    QToolButton* toolButtonZoomOut;        
    QToolButton* toolButtonZoomOriginal;   
    QToolButton* toolButtonClear;           
    QToolButton* toolButtonDelete;          

private:
    QMenu* menuNew;               
    QMenu* menuOpen;                 
    QMenu* menuSave;                  

private:
    QAction* actionNew;                 
    QAction* actionDuplicate;           
    QAction* actionOpen;                
    QAction* actionOpenAll;             
    QAction* actionSave;                
    QAction* actionSaveAll;             

private:
    SourceCodeModel* sourceCodeModel;

private:
    CodeEditorListView* codeEditorListView;
    CodeEditorPlainTextView* codeEditorPlainTextView;
};


#endif // CODEEDITORVIEW_H
