#pragma once
#include <QtWidgets>
#include <QTabBar>

class FileTabBar : public QTabBar
{
	Q_OBJECT

public:
	FileTabBar(QWidget *parent = nullptr);
	~FileTabBar();

protected:
	void mousePressEvent(QMouseEvent *event) override;
};
