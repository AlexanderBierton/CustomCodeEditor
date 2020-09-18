#include "QtWidgetsApplication3.h"
#include "CodeEditor.h"

QtWidgetsApplication3::QtWidgetsApplication3(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	connect(ui.actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(addFile()));
	connect(ui.fileTabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

	ui.fileTabs->insertTab(0,new CodeEditor, ui.fileTabs->getNewFileName());
}


void QtWidgetsApplication3::newFile()
{
	int newTab = ui.fileTabs->insertTab(ui.fileTabs->currentIndex() + 1, new CodeEditor(ui.fileTabs), ui.fileTabs->getNewFileName());
	ui.fileTabs->setCurrentIndex(newTab);
}

void QtWidgetsApplication3::addFile()
{
	QList<QUrl> urls = QFileDialog::getOpenFileUrls(this, tr("Select file(s)"), QUrl("C:\\"));
	int lastInsertedIndex = ui.fileTabs->currentIndex();

	for each (QUrl url in urls)
	{
		QFile* newFile = new QFile(url.toLocalFile());
		if (!ui.fileTabs->hasFileOpen(newFile))
			lastInsertedIndex = ui.fileTabs->insertTab(ui.fileTabs->currentIndex() + 1, new CodeEditor(newFile, ui.fileTabs), url.fileName());
	}

	ui.fileTabs->setCurrentIndex(lastInsertedIndex);
}

void QtWidgetsApplication3::closeTab(int index)
{
	qDebug() << index;

	CodeEditor* selTab = (CodeEditor*)ui.fileTabs->widget(index);
	ui.fileTabs->removeNewFile(selTab);
	ui.fileTabs->removeTab(index);
	
	if (ui.fileTabs->count() == 0)
		newFile();
}