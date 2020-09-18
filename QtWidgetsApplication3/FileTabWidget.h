#pragma once
#ifndef FILE_TAB_WIDGET
#define FILE_TAB_WIDGET
#include <QtWidgets>
#include "qtabwidget.h"
#include "CodeEditor.h"
#include "FileTabBar.h"

class FileTabWidget : public QTabWidget
{
	Q_OBJECT

public:
	FileTabWidget(QWidget *parent = nullptr);
	QString getNewFileName();
	bool hasFileOpen(QFile* file);
	void removeNewFile(CodeEditor* widget);

protected:
	void tabInserted(int index) override;

private slots:
	void onTabSave(GUID guid, bool isNew);
	void onNewTabRequest();
	void onTabClose(int index);
	void onDocumentModified(bool changed);

private:
	std::vector<QWidget*> newFileWidgets;
};

#endif
