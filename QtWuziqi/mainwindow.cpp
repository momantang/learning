#include <QPainter>
#include <QTimer>
#include <QSound>

#include <QMouseEvent>
#include <QMessageBox>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include "math.h"

#include "mainwindow.h"

#define  CHESS_ON_SOUND ""
#define  WIN_SOUND ""
#define  LOSE_SOUND ""


const int kBordMargin = 20;
const int kRadius = 15;
const int kMarkSize = 6;
const int kBlockSize = 40;
const int kPosDelta = 20;

const int kAIDelay = 70;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	qDebug() << "init MainWindow";
	setFixedSize(kBordMargin * 2 + kBlockSize * kBoardSizeNum, kBordMargin * 2 + kBlockSize * kBoardSizeNum);
	setMouseTracking(true);//开启鼠标hover功能

	QMenu* gameMenu = menuBar()->addMenu("Game");
	QAction *acionPVP = new QAction("Person vs Person", this);
	connect(acionPVP, &QAction::triggered, this, &MainWindow::initPVPGame);
	QAction *acionPVE = new QAction("Person vs AI", this);
	connect(acionPVE, &QAction::triggered, this, &MainWindow::initPVEGame);
	gameMenu->addAction(acionPVP);
	gameMenu->addAction(acionPVE);
	initGame();
	setWindowTitle("wuziqi  copy by mmt");

    //ui.setupUi(this);
}

MainWindow::~MainWindow()
{
	if (m_game_model)
	{
		delete m_game_model;
		m_game_model = nullptr;
	}
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	//绘制棋盘
	painter.setRenderHint(QPainter::Antialiasing, true);//抗锯齿
	for (int i=1;i<kBoardSizeNum+1;i++)
	{
		painter.drawLine(kBordMargin + kBlockSize * i, kBordMargin+kBlockSize,kBordMargin+kBlockSize*i,size().height()-kBordMargin-kBlockSize);
		painter.drawLine(kBordMargin+kBlockSize, kBordMargin + kBlockSize * i, size().width() - kBordMargin-kBlockSize, kBordMargin + kBlockSize * i);
	}
	QBrush brush;
	brush.setStyle(Qt::SolidPattern);
	if (clickRosRow>0&&clickRosRow<kBoardSizeNum&&clickRosRow>0&&clickPosCol<kBoardSizeNum&&m_game_model->gameMapVec[clickRosRow][clickPosCol]==0)
	{
		if (m_game_model->playerFlag)
		{
			brush.setColor(Qt::white);
		}
		else {
			brush.setColor(Qt::black);
		}
		painter.setBrush(brush);
		painter.drawRect(kBordMargin + kBlockSize * clickPosCol - kMarkSize / 2, kBordMargin + kBlockSize * clickRosRow - kMarkSize / 2, kMarkSize, kMarkSize);
	}
	for (int i=0;i<kBoardSizeNum;i++)
	{
		for (int j=0;j<kBoardSizeNum;j++)
		{
			if (m_game_model->gameMapVec[i][j]==1)
			{
				brush.setColor(Qt::white);
				painter.setBrush(brush);
				painter.drawEllipse(kBordMargin + kBlockSize * j - kRadius, kBordMargin + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
			}
			else if (m_game_model->gameMapVec[i][j] == -1)
			{
				brush.setColor(Qt::black);
				painter.setBrush(brush);
				painter.drawEllipse(kBordMargin + kBlockSize * j - kRadius, kBordMargin + kBlockSize * i - kRadius, kRadius * 2, kRadius * 2);
			}
		}
	}
	//判断输赢

	if (clickRosRow>0&&clickRosRow<kBoardSizeNum&&clickPosCol>0&clickPosCol<kBoardSizeNum&&(m_game_model->gameMapVec[clickRosRow][clickPosCol]==1|| m_game_model->gameMapVec[clickRosRow][clickPosCol]==-1))
	{
		if (m_game_model->isWin(clickRosRow, clickPosCol) && m_game_model->gameStatus == PALYING)
		{
			qDebug() << "win";
			m_game_model->gameStatus = WIN;
			QString str = "win";
			switch (m_game_model->gameMapVec[clickRosRow][clickPosCol])
			{
			case 1:
				str = "white palter ";
				break;
			case -1:
				str = "black winter";
				break;
			default:
				break;
			}
			QMessageBox::StandardButton btn = QMessageBox::information(this, "congratulation", str);
			if (btn == QMessageBox::Ok) {
				m_game_model->startGame(m_game_type);
				m_game_model->gameStatus = PALYING;
			}
		}
	}
	if (m_game_model->isDeadGame())
	{
		QMessageBox::StandardButton btnValue = QMessageBox::information(this, "oops", "dead game!");
		if (btnValue == QMessageBox::Ok)
		{
			m_game_model->startGame(m_game_type);
			m_game_model->gameStatus = PALYING;
		}

	}
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
	
	int x = event->x();
	int y = event->y();
	qDebug() << "x: " << x << "  y:  " << y;
	if (x >= kBordMargin + kBlockSize / 2 && x < (size().width() - kBordMargin) && y >= kBordMargin + kBlockSize / 2 && y < size().height() - kBordMargin)
	{
		int col = x / kBlockSize;
		int row = y / kBlockSize;
		int leftTopPosX = kBordMargin + kBlockSize * col;
		int leftTopPosY = kBordMargin + kBlockSize * row;


		clickRosRow = -1;
		clickPosCol = -1;
		int len = 0;
		len = sqrt((x - leftTopPosX)*(x - leftTopPosX) + (y - leftTopPosY)*(y - leftTopPosY));
		if (len < kPosDelta)
		{
			clickRosRow = row;
			clickPosCol = col;
		}
		len = sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < kPosDelta)
		{
			clickRosRow = row;
			clickPosCol = col + 1;
		}
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize));
		if (len < kPosDelta)
		{
			clickRosRow = row + 1;
			clickPosCol = col;
		}
		len = sqrt((x - leftTopPosX - kBlockSize) * (x - leftTopPosX - kBlockSize) + (y - leftTopPosY - kBlockSize) * (y - leftTopPosY - kBlockSize));
		if (len < kPosDelta)
		{
			clickRosRow = row + 1;
			clickPosCol = col + 1;
		}
	}
	qDebug() << "click row :" << clickRosRow << "  col:  " << clickPosCol;
	update();
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
	//人辖区，不能抢机器的棋
	if (!(m_game_type==BOT&&!m_game_model->playerFlag))
	{
		chessOneByPerson();
		if (m_game_model->gameType==BOT&&!m_game_model->playerFlag)
		{
			QTimer::singleShot(kAIDelay, this, &MainWindow::chessOnByAI);
		}
	}
}

void MainWindow::initGame()
{
	qDebug() << "init Game";
	m_game_model = new GameModel;
	initPVPGame();
}

void MainWindow::checkGame(int y, int x)
{

}

void MainWindow::chessOneByPerson()
{
	if (clickPosCol!=-1&&clickPosCol!=-1&&m_game_model->gameMapVec[clickRosRow][clickPosCol]==0)
	{
		m_game_model->actionByPerson(clickRosRow, clickPosCol);
		update();
	}
}

void MainWindow::chessOnByAI()
{
	m_game_model->actionByAi(clickRosRow, clickPosCol);
	update();
}

void MainWindow::initPVPGame()
{
	m_game_type = PERSON;
	m_game_model->gameStatus = PALYING;
	m_game_model->startGame(m_game_type);
	update();
}

void MainWindow::initPVEGame()
{
	m_game_type = BOT;
	m_game_model->gameStatus = PALYING;
	m_game_model->startGame(m_game_type);
	update();
}
