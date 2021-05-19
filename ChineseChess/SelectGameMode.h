#pragma once
#include <QDialog>
#include <QPushButton>
class SelectGameMode:public QDialog
{
	Q_OBJECT
public:
	SelectGameMode(QWidget *parent = 0);
	~SelectGameMode();
	QPushButton * m_buttons[4];
	int m_nselect;
public slots:
	void slotClicked();
};

