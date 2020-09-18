#include "FileTabWidget.h"



FileTabWidget::FileTabWidget(QWidget *parent) : QTabWidget(parent)
{
	this->setTabBar(new FileTabBar());

	connect(this->tabBar(), SIGNAL(tabCloseRequested(int)), this, SLOT(onTabClose(int)));
}

void FileTabWidget::tabInserted(int index)
{
	QTabWidget::tabInserted(index);
	CodeEditor* tab((CodeEditor*)this->widget(index));
	
	GUID newGuid;
	CoCreateGuid(&newGuid);

	if (!tab->hasFile())
	{
		tab->setNewFileName(this->tabText(index));
		newFileWidgets.push_back(tab);
	}

	connect(tab, SIGNAL(editorSaved(GUID, bool)), this, SLOT(onTabSave(GUID, bool)));
	connect(tab, SIGNAL(newEditorRequest()), this, SLOT(onNewTabRequest()));
	connect(tab, SIGNAL(modificationChanged(bool)), this, SLOT(onDocumentModified(bool)));

	
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

void FileTabWidget::onTabClose(int index)
{
	CodeEditor* editor = (CodeEditor*)this->widget(index);

	
	this->removeTab(index);
	this->removeNewFile(editor);
}

void FileTabWidget::onDocumentModified(bool changed)
{
	// At the moment you can only have one tab open at a time 
	// this is a temporary thing as of now
	int selectedIdx = this->currentIndex();
	CodeEditor* selectedTab = (CodeEditor*)this->widget(selectedIdx);

	if (selectedTab->hasFile())
	{
		QString title = this->tabText(selectedIdx);

		if (changed)
			title = title += " *";
		else
			title = title.left(title.length() - 2);

		this->setTabText(selectedIdx, title);

	}
	
	
}