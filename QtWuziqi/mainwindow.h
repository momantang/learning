#pragma once

#include <QtWidgets/QMainWindow>
#include "GameModel.h"
//#include "ui_mainwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();
protected:
	void paintEvent(QPaintEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
private:
	GameModel * m_game_model;
	GameType m_game_type;
	int clickRosRow, clickPosCol;
	void initGame();
	void checkGame(int y, int x);
private slots:
	void chessOneByPerson();
	void chessOnByAI();
	void initPVPGame();
	void initPVEGame();
   // Ui::MainWindowClass ui;
};
