#pragma once
#include <QVector>
#include "ChessBoard.h"
#include "ChessStep.h"
class MachineGame :public ChessBoard
{
public:
	 MachineGame();
	~MachineGame();
	void chooseOrMovePieces(int tempID, int &row, int &col);//辅助函数：选棋或者移动棋子
	void saveStep(int moveID, int checkedID, int row, int col, QVector<ChessStep*> &steps);
	void getAllPossibleMoveStep(QVector<ChessStep*>&steps);
	void getAllPossibleMoveStepAndNoKill(QVector<ChessStep*>&steps);

	virtual void mousePressEvent(QMouseEvent *event);
	void clickPieces(int checkID, int &row, int &col);

	void fakeMove(ChessStep* step);
	void unFackeMove(ChessStep* step);
	int calcScore();
	ChessStep* getBestMove();
	void machineChooseAndMovePieces();
};

