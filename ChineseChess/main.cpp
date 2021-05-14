#include "ChineseChess.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ChineseChess w;
    w.show();
    return a.exec();
}
