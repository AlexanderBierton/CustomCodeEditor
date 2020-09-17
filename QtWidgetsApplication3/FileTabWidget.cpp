#include "FileTabWidget.h"



FileTabWidget::FileTabWidget(QWidget *parent) : QTabWidget(parent)
{

}

void FileTabWidget::tabInserted(int index)
{
	QTabWidget::tabInserted(index);

	CodeEditor* tab((CodeEditor*)this->widget(index));

	if (!tab->hasFile())
	{
		newFileWidgets.push_back(tab);
	}
}

QString FileTabWidget::getNewFileName()
{
	QString fileName = "New File ";

	fileName += QString::number(((int)this->newFileWidgets.size()) + 1);
	
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