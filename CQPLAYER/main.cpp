#include "CQPLAYER.h"
#include <QtWidgets/QApplication>
#include <QDir>
#include <QSettings>
#include <QFileInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CQPLAYER w;
    w.show();
    return a.exec();
}
