#pragma once
#include <qaction.h>
#include "QtWidgetsApplication3.h"

class NewFileAction : QAction
{
	Q_OBJECT

public:
	NewFileAction(QWidget *parent = nullptr) : QAction(parent)
	{
		
	}
};