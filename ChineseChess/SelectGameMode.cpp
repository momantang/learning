#include "SelectGameMode.h"
#include <QVBoxLayout>
#include "global.h"
QString btnStr[] = { "����Լ���ս","��Һ�AI��ս","��Һ���Ҷ�ս" };
SelectGameMode::SelectGameMode(QWidget *parent /*= 0*/):QDialog(parent)
{
	QVBoxLayout* lay = new QVBoxLayout(this);
	for (int i=0;i<3;i++)
	{
		lay->addWidget(m_buttons[i] = new QPushButton(btnStr[i]));
		connect(m_buttons[i], &QPushButton::clicked, this, &SelectGameMode::slotClicked);
	}
}

SelectGameMode::~SelectGameMode()
{

}

void SelectGameMode::slotClicked()
{
	QObject *s = sender();

	for (int i=0;i<3;i++)
	{
		if (m_buttons[i]==s)
		{
			this->m_nselect = i;
			break;
		}
	}
	accept();
}
