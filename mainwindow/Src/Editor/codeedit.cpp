
#include <QtWidgets>
#include "codeedit.h"

#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlRecord>

CodeEditor* myCodeEditor=nullptr;
CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    setStyleSheet("background-color:rgb(180,180,180);selection-color:rgb(255,255,255);selection-background-color:rgb(123, 123, 255);font-family:Consolas;font-size:15px;");
    setCursorWidth(2);
    setMinimumSize(400, 205);
    setWordWrapMode(QTextOption::NoWrap);
    setFrameStyle(QFrame::Shadow_Mask | QFrame::Sunken|QFrame::Shape_Mask);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();
}



int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    int space = 12+  fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;
    return space;
}



void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}



void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}



void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}



void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly()) {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor(255,208,255,200);

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}



void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), QColor("#653BA2")/*Qt::lightGray*/);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = static_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top()) ;
    int bottom = top +static_cast<int>(blockBoundingRect(block).height()+0);

    while (block.isValid() && top <= event->rect().bottom()){
        if (block.isVisible() && bottom >= event->rect().top()){
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::yellow);

            QFont font("Consolas",11);

            painter.setFont(font);

            painter.drawText(-6, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

void CodeEditor::OpenGcode(QString filepath)
{
    if(filepath==nullptr){
        filepath= QFileDialog::getOpenFileName(this, tr("A3200 Program File"),nullptr,tr("A3200 Program(*.PGM);;Text files (*.txt)"));
        if (filepath.isEmpty())
            return;
    }
    // QString fileName=QCoreApplication::applicationDirPath()+"user";
    QFile file(filepath);
    if (!file.open(QFile::ReadOnly)){
        QString msg = tr("Failed to open %1\n%2")
                .arg(QDir::toNativeSeparators(filepath), file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }

    this->clear();
    QByteArray data= file.readAll();
    this->setPlainText(QString(data));
    file.close();
}

void CodeEditor::SaveGcode(QString filepath)
{
    if(filepath==nullptr){
        filepath= QFileDialog::getSaveFileName(this, tr("A3200 Program File"),nullptr,tr("A3200 Program(*.PGM);;Text files (*.txt)"));
        if (filepath.isEmpty())
            return;
    }


    /***********another func***********************/
    //    QFile file(programFilePath);
    //    if (!file.open(QFile::WriteOnly)) {
    //        QString msg = tr("Failed to open %1\n%2")
    //                .arg(QDir::toNativeSeparators(programFilePath), file.errorString());
    //        QMessageBox::warning(this, tr("Error"), msg);
    //        return;
    //    }
    //    //        QTextStream out(&file);
    //    //        out << swatchArray[0]->editor->toPlainText();
    //    file.write(swatchArray[0]->editor->toPlainText().toUtf8());
    //    file.close();
    //    QMessageBox::about(this,tr("Message!!"),tr("Program file save successed !!"));
    /***********************************************************/


    QFile file(filepath);
    if (!file.open(QFile::WriteOnly|QFile::Text)) {
        QString msg = tr("Failed to open %1\n%2")
                .arg(QDir::toNativeSeparators(filepath), file.errorString());
        QMessageBox::warning(this, tr("Error"), msg);
        return;
    }
    QTextStream out(&file);
    out << this->toPlainText();
    file.close();
}

void CodeEditor::saveDatabase()
{

}

