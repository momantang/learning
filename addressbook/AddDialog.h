#pragma once
#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
class AddDialog:public QDialog
{
	Q_OBJECT
public:
	AddDialog(QWidget* parent = 0);
	QLineEdit* nameText;
	QTextEdit* addressText;
private:
	QLabel* nameLabel;
	QLabel*addressLabel;
	QPushButton* okBtn;
	QPushButton*cancelBtn;
};

