#pragma once

#include <QPlainTextEdit>
#include <guiddef.h>
class CodeEditor : public QPlainTextEdit
{
	Q_OBJECT

public:
	CodeEditor(QWidget *parent = nullptr);
	CodeEditor(QFile *workingFile, QWidget *parent = nullptr);
	void lineNumberAreaPaintEvent(QPaintEvent *event);
	int lineNumberAreaWidth();
	bool hasFile();
	QString getFilePath();
	void setNewFileName(QString fileName);
	QString getNewFileName();
	void saveFile();
	GUID getGUID();

protected:
	void resizeEvent(QResizeEvent *event) override;
	void keyPressEvent(QKeyEvent *event) override;

private slots:
	void updateLineNumberAreaWidth(int newBlockCount);
	void highlightCurrentLine();
	void updateLineNumberArea(const QRect &rect, int dy);

signals:
	void editorSaved(GUID guid, bool isNew);
	void newEditorRequest();
	
private:
	QWidget *lineNumberArea;
	QFile *file = nullptr;
	QString newFileName;
	GUID guid;
};
