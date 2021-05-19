#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_SQLiteExplorer.h"

class SQLiteExplorer : public QMainWindow
{
    Q_OBJECT

public:
    SQLiteExplorer(QWidget *parent = Q_NULLPTR);

private:
    Ui::SQLiteExplorerClass ui;
};
