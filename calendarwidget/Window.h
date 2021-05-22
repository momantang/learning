#pragma once

#include <QtWidgets/QWidget>
#include "ui_Window.h"

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(QWidget *parent = Q_NULLPTR);

private:
    Ui::WindowClass ui;
};
