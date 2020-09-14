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