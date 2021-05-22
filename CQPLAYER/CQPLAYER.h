#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_CQPLAYER.h"

class CQPLAYER : public QMainWindow
{
    Q_OBJECT

public:
    CQPLAYER(QWidget *parent = Q_NULLPTR);

private:
    Ui::CQPLAYERClass ui;
};
