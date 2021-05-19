#include "ChineseChess.h"
#include <QtWidgets/QApplication>
#include "SelectGameMode.h"
#include "ChooseMainWindow.h"
#include <QCoreApplication>
#include <QDialog>
#include "global.h"
int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);
	SelectGameMode dlg;
	dlg.setWindowTitle("选择游戏方式");
	dlg.setFixedSize(200, 120);
	if (dlg.exec()!=QDialog::Accepted)
	{
	  return 0;
	}
	ChooseMainWindow wnd(dlg.m_nselect);
    return a.exec();
}
