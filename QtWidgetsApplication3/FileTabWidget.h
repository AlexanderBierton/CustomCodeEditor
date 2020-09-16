#pragma once
#include <QtWidgets>
#include "qtabwidget.h"
#include "CodeEditor.h"

class FileTabWidget : public QTabWidget
{
	Q_OBJECT

public:
	FileTabWidget(QWidget *parent = nullptr);
	QString getNewFileName();
	bool hasFileOpen(QFile* file);

protected:
	void tabInserted(int index) override;

private:
	std::vector<QWidget*> newFileWidgets;
};