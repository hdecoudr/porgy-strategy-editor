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

#ifndef CODEEDITORPLAINTEXTVIEW_H
#define CODEEDITORPLAINTEXTVIEW_H

#include <QPlainTextEdit>

class GeneralPurposeSyntaxHighlighter;
class QAbstractListModel;
class QCompleter;


class CodeEditorPlainTextView : public QPlainTextEdit
{
    Q_OBJECT

    class LineNumberArea : public QWidget
    {
    public:
        LineNumberArea(CodeEditorPlainTextView* parent);
        ~LineNumberArea();

	public:
        QSize sizeHint() const override;

    protected:
        void paintEvent(QPaintEvent* event) override;

    private:
        CodeEditorPlainTextView* codeEditor;
    };

public:
    CodeEditorPlainTextView(QWidget* parent = nullptr, int zoom = 0, const QString* syntaxFile = nullptr);
    ~CodeEditorPlainTextView();

public:
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void initConnections();

protected:
    void resizeEvent(QResizeEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void keyPressEvent(QKeyEvent* e) override;
    void focusInEvent(QFocusEvent* e) override;

signals:
    void wheelZoomIn(int);
    void wheelZoomOut(int);

private slots:
    void updateLineNumberAreaWidth(int);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect&, int);
    void insertCompletion(const QString&);
	
private slots:
    void zoomIn(int);
    void zoomOut(int);
    void zoomRestore();

private:
    QString textUnderCursor() const;

private:
    GeneralPurposeSyntaxHighlighter*    syntaxHighlighter;     

private:
    QAbstractListModel*                 completerListModel;    
    QCompleter*                         syntaxCompleter;       
    QWidget*                            lineNumberArea;       

private:
    int                                 defaultZoom;         
};


#endif // CODEEDITORPLAINTEXTVIEW_H
