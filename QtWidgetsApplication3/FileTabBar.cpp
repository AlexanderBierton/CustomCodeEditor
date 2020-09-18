#include "FileTabBar.h"

FileTabBar::FileTabBar(QWidget *parent)
	: QTabBar(parent)
{
}

FileTabBar::~FileTabBar()
{
}

void FileTabBar::mousePressEvent(QMouseEvent *event)
{
	switch (event->button())
	{
	case Qt::MiddleButton:
		if (this->tabAt(event->pos()) > -1)
			this->tabCloseRequested(this->tabAt(event->pos()));
		break;
	default:
		QTabBar::mousePressEvent(event);
	}
}