#include "CodeEditor.h"
#include "LineNumberArea.h"
#include "qpainter.h"
#include "qfileinfo.h"
#include <QtWidgets>

CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent)
{
	lineNumberArea = new LineNumberArea(this);

	connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
	connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
	connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

	QFont font;
	font.setFamily("Roboto Mono Medium");
	font.setStyleHint(QFont::Monospace);
	font.setFixedPitch(true);
	font.setPointSize(10);

	this->setFont(font);
	lineNumberArea->setFont(font);

	const int tabStop = 4;  // 4 characters

	QFontMetrics metrics(font);
	this->setTabStopWidth(tabStop * metrics.width(' '));

	this->setStyleSheet("background-color: #51719b; color: #FFF;");

	CoCreateGuid(&guid);
	updateLineNumberAreaWidth(0);
	highlightCurrentLine();
}

CodeEditor::CodeEditor(QFile *workingFile, QWidget *parent) : QPlainTextEdit(parent)
{
	lineNumberArea = new LineNumberArea(this);
	
	connect(this, &CodeEditor::blockCountChanged, this, &CodeEditor::updateLineNumberAreaWidth);
	connect(this, &CodeEditor::updateRequest, this, &CodeEditor::updateLineNumberArea);
	connect(this, &CodeEditor::cursorPositionChanged, this, &CodeEditor::highlightCurrentLine);

	QFont font;
	font.setFamily("Roboto Mono Medium");
	font.setStyleHint(QFont::Monospace);
	font.setFixedPitch(true);
	font.setPointSize(10);

	this->setFont(font);
	lineNumberArea->setFont(font);

	const int tabStop = 4;  // 4 characters

	QFontMetrics metrics(font);
	this->setTabStopWidth(tabStop * metrics.width(' '));

	this->setStyleSheet("background-color: #51719b; color: #FFF;");

	updateLineNumberAreaWidth(0);
	highlightCurrentLine();

	file = workingFile;
	if (!file->open(QIODevice::ReadOnly))
		return;

	QString plainText;

	while (!file->atEnd()) {
		plainText += file->readLine();
	}

	this->insertPlainText(plainText);

	file->close();
	this->document()->setModified(false);
	this->document()->clearUndoRedoStacks();
}

QString CodeEditor::getFilePath()
{
	if (this->hasFile())
	{
		QFileInfo fileInfo(*file);
		return fileInfo.absoluteFilePath();
	}
	else
		return NULL;
}

int CodeEditor::lineNumberAreaWidth()
{
	int digits = 1;
	int max = qMax(1, blockCount());
	while (max >= 10) {
		max /= 10;
		++digits;
	}

	int space = 3 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

	return space;
}

bool CodeEditor::hasFile()
{
	try
	{
		bool test = file != NULL && file->fileName().size() > 0 && file->exists();
		return test;
	}
	catch(...)
	{
		return false;
	}
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

		QColor lineColor = QColor(81, 113, 155).lighter(160);

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
	painter.fillRect(event->rect(), QColor(106,144,193));

	QTextBlock block = firstVisibleBlock();
	int blockNumber = block.blockNumber();
	int top = qRound(blockBoundingGeometry(block).translated(contentOffset()).top());
	int bottom = top + qRound(blockBoundingRect(block).height());

	while (block.isValid() && top <= event->rect().bottom()) {
		if (block.isVisible() && bottom >= event->rect().top()) {
			QString number = QString::number(blockNumber + 1);
			painter.setPen(Qt::white);
			painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
				Qt::AlignRight, number);
		}

		block = block.next();
		top = bottom;
		bottom = top + qRound(blockBoundingRect(block).height());
		++blockNumber;
	}
}

void CodeEditor::setNewFileName(QString fileName)
{
	if (!fileName.isNull())
		this->newFileName = fileName;
}

QString CodeEditor::getNewFileName()
{
	return this->newFileName;
}

void CodeEditor::keyPressEvent(QKeyEvent *event)
{
	if (event->modifiers() == Qt::ControlModifier)
	{
		switch (event->key())
		{
			case Qt::Key_S:
				saveFile();
				break;
			case Qt::Key_N:
				this->newEditorRequest();
				break;
			default:
				QPlainTextEdit::keyPressEvent(event);
				break;
		}
	}
	else
	{
		QPlainTextEdit::keyPressEvent(event);
	}
}

void CodeEditor::saveFile()
{
	if (!hasFile())
	{
		QUrl saveNewUrl = QFileDialog::getSaveFileUrl(this, "Save new file...", QUrl(), tr("Text Files (*.txt)"));

		QFile* newFile = new QFile(saveNewUrl.toLocalFile());
		
		if (newFile->open(QIODevice::WriteOnly))
		{
			QTextStream stream(newFile);
			stream << this->toPlainText();
			newFile->close();
			file = newFile;
			this->document()->setModified(false);

			this->editorSaved(guid, true);
		}
		else
		{
			QErrorMessage error(this);
			error.showMessage("Unable to save to new file");
		}
	}
	else
	{
		if (file->open(QIODevice::WriteOnly))
		{
			QTextStream stream(file);
			stream << this->toPlainText();
			file->close();
			this->document()->setModified(false);
			this->editorSaved(guid, false);
		}
		else
		{
			QErrorMessage error(this);
			error.showMessage("Unable to save to file at this time");
		}
	}
}

GUID CodeEditor::getGUID()
{
	return this->guid;
}