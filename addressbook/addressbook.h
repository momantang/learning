#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_addressbook.h"

class addressbook : public QMainWindow
{
    Q_OBJECT

public:
    addressbook(QWidget *parent = Q_NULLPTR);

private:
    Ui::addressbookClass ui;
};
