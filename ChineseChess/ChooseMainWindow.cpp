#include "ChooseMainWindow.h"
#include "global.h"
#include <QMessageBox>
#include <QDebug>
ChooseMainWindow::ChooseMainWindow(int nChooseGame, QWidget *parent /*= 0*/):QWidget(parent),m_nChooseGame(nChooseGame)
{
	bool isServer=false;
	bool bServer = false;
	switch (nChooseGame)
	{
	case 0:
		m_p1 = new ChessBoard();
		m_p1->setWindowTitle("����Լ���ս");
		m_p1->show();
		break;
	case  1:
		m_p2 = new MachineGame();
		m_p2->setWindowTitle("��Һ�AI��ս");
		m_p2->show();
		break;
	case  2:
		static	QMessageBox::StandardButton ret = QMessageBox::question(NULL, "��ʾ", "�Ƿ���Ϊ����������[ѡ��췽]");
		
;		if (ret==QMessageBox::Yes)
		{
			isServer = true;
		}
		m_p3 = new NetworkGame(bServer);
		m_p3->setWindowTitle("��Һ���Ҷ�ս" );
		m_p3->show();
		break;
	default:
		break;
	}
}

ChooseMainWindow::~ChooseMainWindow()
{
	switch (m_nChooseGame)
	{
	case 0:
		if (m_p1)
		{
			delete m_p1;
		}
		break;
	case 1:
		if (m_p2)
		{
			delete m_p2;
		}
		break;
	case 2:
		if (m_p3)
		{
			delete m_p3;
		}
		break;
	default:
		break;
	}
}
