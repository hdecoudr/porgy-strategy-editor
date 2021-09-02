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

#include "codeeditorplaintextview.h"
#include "generalpurposesyntaxhighlighter.h"
#include "generalpurposesyntaxhighlightingrules.h"
#include "xmlsyntaxparser.h"

#include <QClipboard>
#include <QTextBlock>
#include <QPainter>
#include <QCompleter>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QStringListModel>
#include <QMessageBox>
#include <QFileInfo>

/****************************************************************************************
 ****************************************************************************************
 *
 * Constructor.
 *
 ****************************************************************************************/
CodeEditorPlainTextView::CodeEditorPlainTextView(QWidget* parent, int zoom, const QString* syntaxFile) : QPlainTextEdit(parent), syntaxHighlighter(nullptr), syntaxCompleter(nullptr)
{
    lineNumberArea = new LineNumberArea(this);

    if(syntaxFile)
    {
        QFileInfo f(*syntaxFile);

        if(f.exists())
        {
            syntaxHighlighter = new GeneralPurposeSyntaxHighlighter(syntaxFile, document());
            QString err = syntaxHighlighter->getParseError();

            if(err.isEmpty())
            {
                QStringList keywords = syntaxHighlighter->getKeywords();

                syntaxCompleter = new QCompleter(this);
                completerListModel = new QStringListModel(keywords, syntaxCompleter);

                syntaxCompleter->setModel(completerListModel);
                syntaxCompleter->setModelSorting(QCompleter::CaseSensitivelySortedModel);

                syntaxCompleter->setCompletionMode(QCompleter::PopupCompletion);
                syntaxCompleter->setWrapAround(false);
                syntaxCompleter->setCaseSensitivity(Qt::CaseInsensitive);
            }
            else
            {
                QMessageBox::warning(this, "I/O Error", "Failed to parse" + *syntaxFile + "! Auto-completion and Syntax highlighting are disabled!");
            }
        }
        else
        {
            QMessageBox::warning(this, "I/O Error", "No syntax file detected! Auto-completion and Syntax highlighting are disabled!");
        }
    }

    setLineWrapMode(QPlainTextEdit::NoWrap);

    initConnections();

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    if(zoom > 0)
    {
        zoomIn(zoom);
    }
    else
    {
        zoomOut(-zoom);
    }

    defaultZoom = font().pointSize() + zoom;

    QFontMetrics fMetrics(font());
    setTabStopWidth(4 * fMetrics.width(' '));
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Destructor.
 *
 ****************************************************************************************/
CodeEditorPlainTextView::~CodeEditorPlainTextView(){}

/****************************************************************************************
 ****************************************************************************************
 *
 * Line numbering area rendering.
 *
 ****************************************************************************************/
void CodeEditorPlainTextView::lineNumberAreaPaintEvent(QPaintEvent* event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), Qt::lightGray);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + static_cast<int>(blockBoundingRect(block).height());

    while(block.isValid() && top <= event->rect().bottom())
    {
        if(block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

/****************************************************************************************
 ****************************************************************************************
 *
 * New width of the line numbering area.
 *
 ****************************************************************************************/
int CodeEditorPlainTextView::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());

    while (max >= 10)
    {
        max /= 10;
        ++digits;
    }

    int space = 3 + this->fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Resize if main container is resized.
 *
 ****************************************************************************************/
void CodeEditorPlainTextView::resizeEvent(QResizeEvent* event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Zoom-in/Zoom-out and scroll with the mouse wheel.
 *
 ****************************************************************************************/
void CodeEditorPlainTextView::wheelEvent(QWheelEvent* event)
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
        QPlainTextEdit::wheelEvent(event);
    }
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Connections.
 *
 ****************************************************************************************/
void CodeEditorPlainTextView::initConnections()
{
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateLineNumberArea(QRect, int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    if(syntaxCompleter)
    {
        connect(syntaxCompleter, SIGNAL(activated(QString)), this, SLOT(insertCompletion(QString)));
    }
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Autocompletion.
 *
 ****************************************************************************************/
void CodeEditorPlainTextView::keyPressEvent(QKeyEvent* e)
{
    if (syntaxCompleter && syntaxCompleter->popup()->isVisible())
    {
        switch (e->key())
        {
            case Qt::Key_Enter:
            case Qt::Key_Return:
            case Qt::Key_Escape:
            case Qt::Key_Tab:
            case Qt::Key_Backtab:
            {
                e->ignore();
                return;
            }

            default:
            {
                break;
            }
        }
    }

    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_Space);

    if(!syntaxCompleter || !isShortcut)
    {
        QPlainTextEdit::keyPressEvent(e);
    }

    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);

    if (!syntaxCompleter || (ctrlOrShift && e->text().isEmpty()))
    {
        return;
    }

    static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-=");

    bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;

    QString completionPrefix = textUnderCursor();

    if(!isShortcut && (hasModifier || e->text().isEmpty() || completionPrefix.length() < 2 || eow.contains(e->text().right(1))))
    {
        syntaxCompleter->popup()->hide();
        return;
    }

    if(completionPrefix != syntaxCompleter->completionPrefix())
    {
        syntaxCompleter->setCompletionPrefix(completionPrefix);
        syntaxCompleter->popup()->setCurrentIndex(syntaxCompleter->completionModel()->index(0, 0));
    }

    QRect cRect = cursorRect();
    cRect.setWidth(syntaxCompleter->popup()->sizeHintForColumn(0) + syntaxCompleter->popup()->verticalScrollBar()->sizeHint().width());
    syntaxCompleter->complete(cRect);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * jknnkn.
 *
 ****************************************************************************************/
void CodeEditorPlainTextView::focusInEvent(QFocusEvent* e)
{
    if(syntaxCompleter)
    {
        syntaxCompleter->setWidget(this);
    }

    QPlainTextEdit::focusInEvent(e);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Update the QPlainTextEdit viewport margins in order to make room for the
 * line numbering area.
 *
 ****************************************************************************************/
void CodeEditorPlainTextView::updateLineNumberAreaWidth(int)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Highlight the edited line.
 *
 ****************************************************************************************/
void CodeEditorPlainTextView::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(Qt::gray).lighter(140);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    this->setExtraSelections(extraSelections);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Resize the line numbering area.
 *
 ****************************************************************************************/
void CodeEditorPlainTextView::updateLineNumberArea(const QRect& rect, int n)
{
    if(n)
    {
        lineNumberArea->scroll(0, n);
    }
    else
    {
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    }

    if(rect.contains(viewport()->rect()))
    {
        updateLineNumberAreaWidth(0);
    }
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Zoom-in.
 *
 ****************************************************************************************/
void CodeEditorPlainTextView::zoomIn(int range)
{
    QFont f = font();
    const int newSize = f.pointSize() + range;

    if (newSize <= 0)
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
void CodeEditorPlainTextView::zoomOut(int range)
{
    zoomIn(-range);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * zoom-restore.
 *
 ****************************************************************************************/
void CodeEditorPlainTextView::zoomRestore()
{
    QFont f = font();
    f.setPointSize(defaultZoom);
    setFont(f);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Complete.
 *
 ****************************************************************************************/
void CodeEditorPlainTextView::insertCompletion(const QString& text)
{
    if (syntaxCompleter->widget() != this)
    {
        return;
    }

    QTextCursor tCursor = textCursor();
    int extra = text.length() - syntaxCompleter->completionPrefix().length();

    tCursor.movePosition(QTextCursor::Left);
    tCursor.movePosition(QTextCursor::EndOfWord);
    tCursor.insertText(text.right(extra));
    setTextCursor(tCursor);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Get text under cursor.
 *
 ****************************************************************************************/
QString CodeEditorPlainTextView::textUnderCursor() const
{
    QTextCursor tCursor = textCursor();
    tCursor.select(QTextCursor::WordUnderCursor);

    return tCursor.selectedText();
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Constructor.
 *
 ****************************************************************************************/
CodeEditorPlainTextView::LineNumberArea::LineNumberArea(CodeEditorPlainTextView* parent) : QWidget(parent), codeEditor(parent) {}

/****************************************************************************************
 ****************************************************************************************
 *
 * Destructor.
 *
 ****************************************************************************************/
CodeEditorPlainTextView::LineNumberArea::~LineNumberArea() {}

/****************************************************************************************
 ****************************************************************************************
 *
 * Preferred size.
 *
 ****************************************************************************************/
QSize CodeEditorPlainTextView::LineNumberArea::sizeHint() const
{
    return QSize(codeEditor->lineNumberAreaWidth(), 0);
}

/****************************************************************************************
 ****************************************************************************************
 *
 * Paint line number area..
 *
 ****************************************************************************************/
void CodeEditorPlainTextView::LineNumberArea::paintEvent(QPaintEvent* event)
{
    codeEditor->lineNumberAreaPaintEvent(event);
}
