#include "SQLiteExplorer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SQLiteExplorer w;
    w.show();
    return a.exec();
}
