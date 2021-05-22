#include "AddDialog.h"
#include <QGridLayout>
AddDialog::AddDialog(QWidget* parent /*= 0*/):QDialog(parent)
{
	nameLabel = new QLabel("name");
	addressLabel = new QLabel("address");

	okBtn = new QPushButton("OK");
	cancelBtn = new QPushButton("Cancel");

	nameText = new QLineEdit;
	addressText = new QTextEdit;

	QGridLayout* layout = new QGridLayout;
	layout->setColumnStretch(1, 2);
	layout->addWidget(nameLabel, 0, 0);
	layout->addWidget(nameText, 0, 1);

	layout->addWidget(addressLabel, 1, 0,Qt::AlignLeft|Qt::AlignTop);
	layout->addWidget(addressText, 1, 1,Qt::AlignLeft);

	QHBoxLayout* btnLayout = new QHBoxLayout;
	btnLayout->addWidget(okBtn);
	btnLayout->addWidget(cancelBtn);

	layout->addLayout(btnLayout, 2, 1, Qt::AlignLeft);

	QVBoxLayout *mainLayout = new QVBoxLayout;
	mainLayout->addLayout(layout);
	setLayout(mainLayout);

	connect(okBtn, &QPushButton::clicked, this, &AddDialog::accept);
	connect(cancelBtn, &QPushButton::clicked, this, &AddDialog::reject);
	
	this->setWindowTitle("Add a Contact");
}
