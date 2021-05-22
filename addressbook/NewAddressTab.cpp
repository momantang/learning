#include "NewAddressTab.h"

NewAddressTab::NewAddressTab(QWidget *parent /*= 0*/):QWidget(parent)
{
	this->descriptionLabel = new QLabel(tr("There are currently no contacts in your address book. "
		"\nClick Add to add new contacts."));
	this->addButton = new QPushButton("Add");
	this->mainLayout = new QVBoxLayout;
	mainLayout->addWidget(descriptionLabel);
	mainLayout->addWidget(addButton);
	setLayout(mainLayout);
	connect(addButton, &QPushButton::clicked, this, &NewAddressTab::addEntry);
}

void NewAddressTab::addEntry()
{

}
