#include "AddressWidget.h"
#include "QTableView"
#include <QHeaderView>
#include "AddDialog.h"
#include <QMessageBox>
AddressWidget::AddressWidget(QWidget *parent /*= 0*/):QTabWidget(parent)
{
	table = new TableModel(this);
	newAddressTab = new NewAddressTab(this);
	connect(newAddressTab, &NewAddressTab::sendDetails, this, &AddressWidget::addEntry);
	this->addTab(newAddressTab, "Address Book");
		setupTabs();
}

void AddressWidget::readFromFile(const QString &filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::information(this, "unable to open file", file.errorString());
		return;
	}
	QList<Contact> list;
	QDataStream in(&file);
	in >> list;
	if (list.isEmpty())
	{
		QMessageBox::information(this, "No Contacts in file", "The file yout attemping to open contains no contacts.");

	}
	else {
		for (const auto &contact:qAsConst(list))
		{
			addEntry(contact.name, contact.address);
		}
	}
}

void AddressWidget::writeT0File(const QString &filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly))
	{
		QMessageBox::information(this, "unable to open file", file.errorString());
		return;
	}
	QDataStream out(&file);
	out << table->getContacts();
}

void AddressWidget::showAddEntryDialog()
{
	AddDialog aDialog;
	if (aDialog.exec())
	{
		QString name = aDialog.nameText->text();
		QString address = aDialog.addressText->toPlainText();
		addEntry(name, address);
	}
}

void AddressWidget::addEntry(QString name, QString address)
{
	if (!table->getContacts().contains({name,address}))
	{
		table->insertRows(0, 1, QModelIndex());
		QModelIndex index = table->index(0, 0, QModelIndex());
		table->setData(index, name, Qt::EditRole);
		index = table->index(0, 1, QModelIndex());
		table->setData(index, address, Qt::EditRole);
		removeTab(indexOf(newAddressTab));
	}
	else {
		QMessageBox::information(this, "Duplicate", tr("the name \"%1\" already exists.").arg(name));
	}
}

void AddressWidget::editEntry()
{
	QTableView *temp = static_cast<QTableView*>(currentWidget());
	QSortFilterProxyModel* proxy = static_cast<QSortFilterProxyModel*>(temp->model());
	QItemSelectionModel *selectionModel = temp->selectionModel();

	QModelIndexList indexes = selectionModel->selectedRows();
	QString name;
	QString address;
	int row = -1;
	foreach (QModelIndex index,indexes)
	{
		row = proxy->mapToSource(index).row();
		QModelIndex nameIndex = table->index(row, 0, QModelIndex());
		QVariant varName = table->data(nameIndex, Qt::DisplayRole);
		name = varName.toString();


		QModelIndex addressIndex = table->index(row, 1, QModelIndex());
		QVariant varAddress = table->data(addressIndex, Qt::DisplayRole);
		address = varAddress.toString();
	}
	AddDialog aDialog;
	aDialog.setWindowTitle("Edit a Contact");
	aDialog.nameText->setText(name);
	aDialog.nameText->setReadOnly(true);
	aDialog.addressText->setText(address);

	if (aDialog.exec())
	{
		QString newAddress = aDialog.addressText->toPlainText();
		if (newAddress!=address)
		{
			QModelIndex index = table->index(row, 1, QModelIndex());
			table->setData(index, newAddress, Qt::EditRole);
		}
	}
}

void AddressWidget::remveEntry()
{
	QTableView *temp = static_cast<QTableView*>(currentWidget());
	QSortFilterProxyModel* proxy = static_cast<QSortFilterProxyModel*>(temp->model());
	QItemSelectionModel* selectionModel = temp->selectionModel();

	QModelIndexList indexes = selectionModel->selectedRows();
	foreach(QModelIndex index, indexes) {
		int row = proxy->mapToSource(index).row();
		table->removeRows(row, 1, QModelIndex());
	}
	if (table->rowCount(QModelIndex())==0)
	{
		insertTab(0, newAddressTab, "Address Book");
	}
}

void AddressWidget::setupTabs()
{
	QStringList groups;
	groups << "ABC" << "DEF" << "GHI" << "JKL" << "MNO" << "PQR" << "STU" << "VW" << "XYZ";
	for (int i=0;i<groups.size();++i)
	{
		QString str = groups.at(i);
		QString reg = QString("^[%1].*").arg(str);

		proxyModel = new QSortFilterProxyModel(this);
		proxyModel->setSourceModel(table);
		proxyModel->setFilterRegExp(QRegExp(reg, Qt::CaseInsensitive));
		proxyModel->setFilterKeyColumn(0);

		QTableView *tableView=new QTableView;
		tableView->setModel(proxyModel);

		tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
		tableView->horizontalHeader()->setStretchLastSection(true);
		tableView->verticalHeader()->hide();
		tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
		tableView->setSelectionMode(QAbstractItemView::SingleSelection);

		connect(tableView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &AddressWidget::selectionChanged);
		connect( this, &QTabWidget::currentChanged, this, [this](int tabIndex) {
			auto* tableView = qobject_cast<QTableView*> (widget(tabIndex));
			if (tableView)
			{
				emit selectionChanged(tableView->selectionModel()->selection());
			}
		});
		addTab(tableView, str);
	}
}
