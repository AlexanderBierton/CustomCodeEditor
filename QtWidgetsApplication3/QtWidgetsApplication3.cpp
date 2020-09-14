#include "QtWidgetsApplication3.h"
#include "CodeEditor.h"

QtWidgetsApplication3::QtWidgetsApplication3(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

	

	//ui.fileTabs->addTab(new CodeEditor(new QFile("C:\\Users\\alexa\\Desktop\\Python\\txtfiles\\testfile.txt")), "Test");

	ui.fileTabs->addTab(new CodeEditor, ui.fileTabs->getNewFileName());
}
