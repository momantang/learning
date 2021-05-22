#pragma once
#include <QTabWidget>
#include <QSortFilterProxyModel>
#include "TableModel.h"
#include "newAddressTab.h"
class AddressWidget:public QTabWidget
{
	Q_OBJECT
public:
	AddressWidget(QWidget *parent = 0);
	void readFromFile(const QString &filename);
	void writeT0File(const QString &filename);
public slots:
	void showAddEntryDialog();
	void addEntry(QString name, QString address);
	void editEntry();
	void remveEntry();
signals:
	void selectionChanged(const QItemSelection &selected);
private:
	void setupTabs();
	TableModel* table;
	NewAddressTab* newAddressTab;
	QSortFilterProxyModel* proxyModel;
};