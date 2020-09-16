#include "QtWidgetsApplication3.h"
#include "CodeEditor.h"

QtWidgetsApplication3::QtWidgetsApplication3(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	connect(ui.actionNew, SIGNAL(triggered()), this, SLOT(newFile()));
	connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(addFile()));

	ui.fileTabs->addTab(new CodeEditor, ui.fileTabs->getNewFileName());
}


void QtWidgetsApplication3::newFile()
{
	int newTab = ui.fileTabs->insertTab(ui.fileTabs->currentIndex() + 1, new CodeEditor, ui.fileTabs->getNewFileName());
	ui.fileTabs->setCurrentIndex(newTab);
}

void QtWidgetsApplication3::addFile()
{
	QList<QUrl> urls = QFileDialog::getOpenFileUrls(this, tr("Select file(s)"), QUrl("C:\\"));
	int lastInsertedIndex = 0;

	for each (QUrl url in urls)
	{
		QFile* newFile = new QFile(url.toLocalFile());
		lastInsertedIndex = ui.fileTabs->insertTab(ui.fileTabs->currentIndex() + 1, new CodeEditor(newFile), url.fileName());
	}

	ui.fileTabs->setCurrentIndex(lastInsertedIndex);
}