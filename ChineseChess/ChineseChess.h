#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ChineseChess.h"

class ChineseChess : public QMainWindow
{
    Q_OBJECT

public:
    ChineseChess(QWidget *parent = Q_NULLPTR);

private:
    Ui::ChineseChessClass ui;
};
