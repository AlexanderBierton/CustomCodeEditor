#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication3.h"

class QtWidgetsApplication3 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication3(QWidget *parent = Q_NULLPTR);
	

public slots:
	void newFile();
	void addFile();
	void closeTab(int index);

private:
    Ui::QtWidgetsApplication3Class ui;
};
