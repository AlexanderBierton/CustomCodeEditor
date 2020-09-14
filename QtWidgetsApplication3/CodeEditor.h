#pragma once

#include <QPlainTextEdit>

class CodeEditor : public QPlainTextEdit
{
	Q_OBJECT

public:
	CodeEditor(QWidget *parent = nullptr);
	CodeEditor(QFile *workingFile, QWidget *parent = nullptr);
	void lineNumberAreaPaintEvent(QPaintEvent *event);
	int lineNumberAreaWidth();
	bool hasFile();

protected:
	void resizeEvent(QResizeEvent *event) override;

private slots:
	void updateLineNumberAreaWidth(int newBlockCount);
	void highlightCurrentLine();
	void updateLineNumberArea(const QRect &rect, int dy);

private:
	QWidget *lineNumberArea;
	QFile *file = nullptr;
};
