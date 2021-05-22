#include "MainWindow.h"
#include "QMainWindow"
#include <QMenuBar>
#include <QFileDialog>
MainWindow::MainWindow(QWidget* parent /*= 0*/)
{
	addressWiget = new AddressWidget;
	setCentralWidget(this->addressWiget);
	this->createMenus();
	this->setFixedSize(800, 600);
	this->setWindowTitle("Address Book");
}

void MainWindow::updateActions(const QItemSelection &selection)
{
	QModelIndexList indexs = selection.indexes();
	if (!indexs.isEmpty())
	{
		removeAct->setEnabled(true);
		editAct->setEnabled(true);
	}
	else {
		removeAct->setEnabled(false);
		editAct->setEnabled(false);
	}
}

void MainWindow::openFile()
{
	QString filename = QFileDialog::getOpenFileName(this);
	if (!filename.isEmpty())
	{
		addressWiget->readFromFile(filename);
	}
}

void MainWindow::saveFile()
{
	QString filename = QFileDialog::getSaveFileName(this);
	if (!filename.isEmpty())
	{
		addressWiget->writeT0File(filename);
	}
}

void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	openAct = new QAction("&Open", this);
	fileMenu->addAction(openAct);

	saveAct = new QAction("&Save", this);
	fileMenu->addAction(saveAct);

	exitAct = new QAction("&Exit", this);
	fileMenu->addAction(exitAct);

	toolMenu = menuBar()->addMenu(tr("&Tool"));
	editAct = new QAction("&Edit", this);
	editAct->setEnabled(false);
	toolMenu->addAction(editAct);

	toolMenu->addSeparator();


	addAct = new QAction("&Add", this);
	toolMenu->addAction(addAct);
	removeAct = new QAction("&Remove", this);
	removeAct->setEnabled(false);
	toolMenu->addAction(removeAct);

	connect(openAct, &QAction::triggered, this, &MainWindow::openFile);
	connect(saveAct, &QAction::triggered, this, &MainWindow::saveFile);
	connect(exitAct, &QAction::triggered, this, &MainWindow::close);
	connect(editAct, &QAction::triggered, addressWiget, &AddressWidget::editEntry);
	connect(addAct, &QAction::triggered, addressWiget, &AddressWidget::showAddEntryDialog);
	connect(removeAct, &QAction::triggered, addressWiget, &AddressWidget::remveEntry);
	connect(addressWiget, &AddressWidget::selectionChanged, this, &MainWindow::updateActions);
}
