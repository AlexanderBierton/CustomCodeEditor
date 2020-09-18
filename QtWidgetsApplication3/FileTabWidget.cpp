#include "FileTabWidget.h"



FileTabWidget::FileTabWidget(QWidget *parent) : QTabWidget(parent)
{
	
}

void FileTabWidget::tabInserted(int index)
{
	QTabWidget::tabInserted(index);
	CodeEditor* tab((CodeEditor*)this->widget(index));
	
	GUID newGuid;
	CoCreateGuid(&newGuid);
	//tab->setGUID(newGuid);

	if (!tab->hasFile())
	{
		tab->setNewFileName(this->tabText(index));
		newFileWidgets.push_back(tab);
	}

	connect(tab, SIGNAL(editorSaved(GUID, bool)), this, SLOT(onTabSave(GUID, bool)));
	connect(tab, SIGNAL(newEditorRequest()), this, SLOT(onNewTabRequest()));
}

QString FileTabWidget::getNewFileName()
{
	QString fileName = "New File ";
	
	if (newFileWidgets.size() > 0)
	{
		std::string lastNewFileName = ((CodeEditor*)this->newFileWidgets.back())->getNewFileName().toStdString();
		int num = lastNewFileName[lastNewFileName.length() - 1] + 1;
		fileName += num;
	}
	else
		fileName += '1';
	
	return fileName;
}

bool FileTabWidget::hasFileOpen(QFile* file)
{
	bool res = false;
	int currentIndex = 0;
	int numOfTabs = this->count();
	QFileInfo fileInfo(*file);
	QString filePath = fileInfo.absoluteFilePath();

	while (currentIndex < numOfTabs)
	{
		CodeEditor* tab((CodeEditor*)this->widget(currentIndex));
		QString absolutePath = tab->getFilePath();

		if (filePath == absolutePath)
		{
			res = true;
			break;
		}
		++currentIndex;
	}

	return res;
}

void FileTabWidget::removeNewFile(CodeEditor* widget)
{
	int idx = -1;
	for (int i = 0; i < newFileWidgets.size(); i++)
	{
		if ((CodeEditor*)newFileWidgets[i] == widget)
		{
			idx = i;
			break;
		}
	}

	if (idx > -1)
		newFileWidgets.erase(newFileWidgets.begin() + (idx));
}

void FileTabWidget::onTabSave(GUID guid, bool isNew)
{
	if (isNew)
	{
		int num = this->count();
		for (int i = 0; i < this->count(); i++)
		{
			CodeEditor* tab = (CodeEditor*)this->widget(i);
			if (tab->getGUID() == guid)
			{
				QFileInfo fileInfo(tab->getFilePath());

				this->setTabText(i, fileInfo.fileName());
			}
		}
	}
}

void FileTabWidget::onNewTabRequest()
{
	int newTab = this->addTab(new CodeEditor(), this->getNewFileName());
	this->setCurrentIndex(newTab);
}