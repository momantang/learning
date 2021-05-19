#pragma once

#include <QMainWindow>
#include <QFrame>
#include "ChessPieces.h"
#include <QPainter>
#include <QPoint>
#include <QMouseEvent>
#include <QTime>
#include <QTimer>
#include <QMessageBox>
#include <QtGlobal>

namespace Ui { class ChessBoard; };

class ChessBoard : public QMainWindow
{
	Q_OBJECT

public:
	ChessBoard(QWidget *parent = Q_NULLPTR);
	~ChessBoard();
	bool isDead(int id);
	int getStoneId(int row, int col);
	//�����ڵĹ��ܸ����������ж������Ƿ���һ��ֱ�����棬�ҷ���ֱ��֮������Ӹ���
	int getStoneCountAtLine(int row1, int col1, int row2, int col2);
	void whoWin();
	//�Ƿ�ѡ�и�����
	bool isChecked(QPoint pt, int &row, int &col);
	QPoint getRealPoint(QPoint pt);

	QPoint center(int row, int col);//�������ƪ������ת���ɽ�������
	QPoint center(int id);
	virtual void paintEvent(QPaintEvent *event);
	void drawChessPieces(QPainter& painter, int id);

	virtual void mousePressEvent(QMouseEvent *event);
	virtual void clickPieces(int checkedId, int &row, int &col);

	bool canMove(int moveId, int killId, int row, int col);
	bool canMoveJIANG(int moveId, int killId, int row, int col);
	bool canMoveSHI(int moveId, int killId, int row, int col);
	bool canMoveXIANG(int moveId, int killId, int row, int col);
	bool canMoveMA(int moveId, int killId, int row, int col);
	bool canMoveCHE(int moveId, int killId, int row, int col);
	bool canMovePAO(int moveId, int killId, int row, int col);
	bool canMoveBING(int moveId, int killId, int row, int col);

	void debugChess();

	void init();
	ChessPieces m_chessPieces[32];
	int m_nR;
	int m_nOffset;
	int m_nD;
	int m_nSlectedID;
	int m_nCheckedID;
	bool m_bIsRed;
	bool m_bIsTcpServer;
	bool m_bISOver;
private slots:
	void updateTime();
	void on_pushButton_start_clicked();
	void on_pushButton_reset_clicked();
	void on_pushButton_about_clicked();
	void on_pushButton_restart_clicked();

private:

	int getD(int moveId,int row,int col);
	Ui::ChessBoard *ui;
	QTimer *m_timer;
	QTime *m_timeRecord;
	bool m_bIsStart;

};
