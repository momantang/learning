#pragma once
#include <QMainWindow>
#include "QItemSelectionModel"
#include "AddressWidget.h"
#include <QMenu>
#include <QAction>
class MainWindow:public QMainWindow
{
	Q_OBJECT;
public:
	MainWindow(QWidget* parent = 0);
private slots:
	void updateActions(const QItemSelection &selection);
	void openFile();
	void saveFile();
private:
	void createMenus();
	AddressWidget* addressWiget;
	QMenu * fileMenu;
	QMenu * toolMenu;
	QAction * openAct;
	QAction * saveAct;
	QAction * exitAct;
	QAction * addAct;
	QAction * editAct;
	QAction * removeAct;
	
};

