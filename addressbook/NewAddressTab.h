#pragma once
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
class NewAddressTab :public QWidget
{
	Q_OBJECT
public:
	NewAddressTab(QWidget *parent = 0);
public slots:
	void addEntry();
signals:
	void sendDetails(QString name, QString address);
private: 
	QLabel *descriptionLabel;
	QPushButton *addButton;
	QVBoxLayout* mainLayout;
};

