#pragma once
#include <QAbstractTableModel>
#include <QList>

struct Contact
{
	QString name;
	QString address;
	bool operator==(const Contact &other) const {
		return this->name == other.name&&this->address == other.address;
	}
};
inline QDataStream  &operator<<(QDataStream &stream, const Contact &contact) {
	return stream << contact.name << contact.address;
}
inline QDataStream &operator>>(QDataStream &stream, Contact &contact) {
	return stream >> contact.name >> contact.address;
}
class TableModel:public QAbstractTableModel
{
	Q_OBJECT
public:

	TableModel(QObject*parent = 0);
	TableModel(QList<Contact> contacts, QObject*parent = 0);

	 int rowCount(const QModelIndex &parent = QModelIndex()) const override;


	 int columnCount(const QModelIndex &parent = QModelIndex()) const override;


	 QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	 QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

	 Qt::ItemFlags flags(const QModelIndex &index)const override;
	 bool setData(const QModelIndex &index, const QVariant &value, int role  = Qt::EditRole )  override;
	 bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
	 bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex())  override;

	 QList<Contact> getContacts() const;
private:
	QList<Contact> contacts;

};

