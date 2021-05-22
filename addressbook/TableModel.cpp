#include "TableModel.h"

TableModel::TableModel(QObject*parent /*= 0*/):QAbstractTableModel(parent)
{

}

TableModel::TableModel(QList<Contact> contacts, QObject*parent /*= 0*/):QAbstractTableModel(parent),contacts(contacts)
{

}


int TableModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	Q_UNUSED(parent);
	return contacts.size();
}

int TableModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	Q_UNUSED(parent);
	return 2;
}



QVariant TableModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
	if (!index.isValid())
	{
		return QVariant();
	}
	if (index.row()>contacts.size()||index.row()<0)
	{
		return QVariant();
	}
	if (role==Qt::DisplayRole)
	{
		const auto &contact = contacts.at(index.row());
		if (index.column()==0)
		{
			return contact.name;
		}else if (index.column()==1)
		{
			return contact.address;
		}
	}
	return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if (role!=Qt::DisplayRole)
	{
		return QVariant();
	}
	if (orientation==Qt::Horizontal)
	{
		switch (section)
		{
		case 0:
			return tr("Name");
		case 1:
			return tr("Address");
		default:
			return QVariant();
		}
	}
	return QVariant();
}


Qt::ItemFlags TableModel::flags(const QModelIndex &index) const
 {
	if (!index.isValid())
	{
		return Qt::ItemIsEnabled;
	}
	return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
 }

 bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role /*= Qt::EditRole */) 
 {
	 if (index.isValid()&&role==Qt::EditRole)
	 {
		 int row = index.row();
		 auto contact = contacts.value(row);
		 if (index.column() == 0)
		 {
			 contact.name = value.toString();
		 }
		 else if (index.column() == 1)
		 {
			 contact.address = value.toString();
		 }
		 else {
			 return false;
		 }
		 contacts.replace(row, contact);
		 emit dataChanged(index, index, { row });
		 return true;
		 
	 }
	 return false;
 }

 bool TableModel::insertRows(int row, int count, const QModelIndex &parent /*= QModelIndex()*/) 
 {
	 Q_UNUSED(parent);
	 beginInsertRows(QModelIndex(), row, row + count - 1);
	 for (int r=0;r<count;r++)
	 {
		 contacts.insert(row, { QString(),QString() });
	 }
	 endInsertRows();
	 return true;
 }

 bool TableModel::removeRows(int row, int count, const QModelIndex &parent /*= QModelIndex()*/) 
 {
	 Q_UNUSED(parent);
	 beginRemoveRows(QModelIndex(), row, row + count - 1);
	 for (int r=0;r<count;r++)
	 {
		 contacts.removeAt(row);
	 }
	 endRemoveRows();
	 return true;
 }

QList<Contact> TableModel::getContacts() const
 {
	return contacts;
 }
