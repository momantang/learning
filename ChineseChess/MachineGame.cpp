#include "MachineGame.h"
#include "global.h"
#include <QDebug>
MachineGame::MachineGame()
{
}

MachineGame::~MachineGame()
{

}

void MachineGame::chooseOrMovePieces(int tempID, int &row, int &col)
{
	qDebug() << "chooseOrMovePieces";
	//qDebug() << "tempID: " << tempID << " row :" << row << " col " << col;
	if (this->m_nSlectedID == -1)
	{
		qDebug() << "选择棋子 " << this->m_nSlectedID;
		if (this->m_nCheckedID != -1)
		{
			if (this->m_chessPieces[this->m_nCheckedID].m_bRed)
			{
				this->m_nSlectedID = tempID;
			}
			else {
				this->m_nSlectedID = -1;
				return;
			}
		}
	}
	else {
		qDebug() << "移动棋子" << this->m_nSlectedID;
		if (this->canMove(this->m_nSlectedID, this->m_nCheckedID, row, col))
		{
			this->m_chessPieces[this->m_nSlectedID].m_nRow = row;
			this->m_chessPieces[this->m_nSlectedID].m_nCol = col;
			if (this->m_nCheckedID != -1)
			{
				this->m_chessPieces[this->m_nCheckedID].m_bDead = true;
			}
			this->m_nSlectedID = -1;
			this->m_bIsRed = !m_bIsRed;
		}
	}
	whoWin();
	this->update();
}

void MachineGame::saveStep(int moveID, int checkedID, int row, int col, QVector<ChessStep*> &steps)
{
	ChessStep* step = new ChessStep;
	step->m_nRowFrom = m_chessPieces[moveID].m_nRow;
	step->m_nColFrom = m_chessPieces[moveID].m_nCol;
	step->m_nRowTo = row;
	step->m_nColTo = col;
	step->m_nMoveID = moveID;
	step->m_nKillID = checkedID;

	steps.push_back(step);
}

void MachineGame::getAllPossibleMoveStep(QVector<ChessStep*>&steps)
{
	for (int id = 0; id < 16; id++)
	{
		if (this->m_chessPieces[id].m_bDead)
		{
			continue;
		}
		for (int row = 0; row < 10; row++)
		{
			for (int col = 0; col < 9; col++)
			{
				int i = 16;
				for (; i < 32; i++)
				{
					if (this->m_chessPieces[i].m_nRow == row && this->m_chessPieces[i].m_nCol == col && (!this->m_chessPieces[i].m_bDead))
					{
						break;
					}
				}
				if (i != 32)
				{
					if (this->canMove(id, i, row, col))
					{
						this->saveStep(id, i, row, col, steps);
					}
				}
			}
		}

	}
}

void MachineGame::getAllPossibleMoveStepAndNoKill(QVector<ChessStep*>&steps)
{
	for (int id = 0; id < 16; id++)   //存在的黑棋， 能否走到这些盘棋盘里面的格子
	{
		if (m_chessPieces[id].m_bDead)
			continue;

		for (int row = 0; row < 10; row++)
		{
			for (int col = 0; col < 9; col++)
			{

				int i = 0;
				for (; i <= 31; i++)
				{
					if (m_chessPieces[i].m_nRow == row && m_chessPieces[i].m_nCol == col && m_chessPieces[i].m_bDead == false)
						break;
				}

				if (id < 16 && i == 32)
				{
					if (canMove(id, -1, row, col))
						saveStep(id, -1, row, col, steps);
				}
			}
		}
	}
}

void MachineGame::mousePressEvent(QMouseEvent *event)
{
	//qDebug() << "MachineGame::mousePressEvent(QMouseEvent *event)";
	if (event->button() != Qt::LeftButton)
	{
		return;
	}
	int row, col;
	if (!isChecked(this->getRealPoint(event->pos()), row, col))
	{
		return;
	}
	m_nCheckedID = -1;
	int i = 0;
	for (; i < 32; i++)
	{
		//qDebug() << "r:" << m_chessPieces[i].m_nRow << "  c  " << m_chessPieces[i].m_nCol;
		if (m_chessPieces[i].m_nRow == row && m_chessPieces[i].m_nCol == col && m_chessPieces[i].m_bDead == false)
		{
			break;
		}
	}
	if (0 <= i && i < 32)
	{
		m_nCheckedID = i;
	}
	clickPieces(m_nCheckedID, row, col);
	if (m_bIsRed)
	{
		//qDebug() << "if (m_bIsRed) " << this->m_bIsRed;
		chooseOrMovePieces(i, row, col);
		
		if (!m_bIsRed) {
		//	qDebug() << "if (!m_bIsRed) " << this->m_bIsRed;
			machineChooseAndMovePieces();
		}
	}
}

void MachineGame::clickPieces(int checkID, int &row, int &col)
{
	if (this->m_bIsRed)
	{
		//qDebug() << "if (m_bIsRed) " << this->m_bIsRed;
		chooseOrMovePieces(checkID, row, col);
		if (!this->m_bIsRed)
		{
			//qDebug() << "if (!m_bIsRed) " << this->m_bIsRed;
			machineChooseAndMovePieces();
		}
	}
}

void MachineGame::fakeMove(ChessStep* step)
{
	if (step->m_nKillID != -1)
	{
		this->m_chessPieces[step->m_nKillID].m_bDead = true;
	}
	this->m_chessPieces[step->m_nMoveID].m_nRow = step->m_nRowTo;
	this->m_chessPieces[step->m_nMoveID].m_nCol = step->m_nColTo;
	this->m_bIsRed = !m_bIsRed;
}

void MachineGame::unFackeMove(ChessStep* step)
{
	if (step->m_nKillID != -1)
	{
		this->m_chessPieces[step->m_nKillID].m_bDead = false;
	}
	this->m_chessPieces[step->m_nMoveID].m_nRow = step->m_nRowFrom;
	this->m_chessPieces[step->m_nMoveID].m_nCol = step->m_nColFrom;
	this->m_bIsRed = !m_bIsRed;
}

int MachineGame::calcScore()
{
	int rScore = 0;
	int bScore = 0;
	static int chessScore[] = { 200,20,40,60,100,80,10 };
	for (int i = 0; i < 16; i++)
	{
		if (this->m_chessPieces[i].m_bDead)
		{
			continue;
		}
		bScore += chessScore[this->m_chessPieces[i].m_emType];
	}
	for (int i = 16; i < 32; i++)
	{
		if (this->m_chessPieces[i].m_bDead)
		{
			continue;
		}
		rScore += chessScore[this->m_chessPieces[i].m_emType];
	}
	return bScore - rScore;
}
//获得最好的移动步骤
//第一此玩了一把，发现我居然下不赢自己写的算法。哭了哭了哭了555555........ by 原作者
ChessStep* MachineGame::getBestMove()
{
	//qDebug() << "getBestMove";
	int maxScore = -10000;
	ChessStep* result = NULL;


	//有可击杀的红棋子就走击杀红棋子最优的一步
	QVector<ChessStep*> steps;
	getAllPossibleMoveStep(steps);

	//没有可击杀的红棋子就走最后一步
	QVector<ChessStep*> stepsAndNoKill;
	getAllPossibleMoveStepAndNoKill(stepsAndNoKill);

	for (QVector<ChessStep*>::iterator it = steps.begin(); it != steps.end(); it++)
	{
		ChessStep *step = *it;
		fakeMove(step);
		int score = calcScore();
		unFackeMove(step);
		if (score > maxScore)
		{
			maxScore = score;
			result = step;
		}
	}
	if (result != NULL)
	{
		return result;
	}

	//2
	int count = stepsAndNoKill.count();
	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
	int temp = qrand() % count;
	QVector<ChessStep*>::iterator it = stepsAndNoKill.begin();
	result = it[temp];
	if (result == NULL)
	{
		whoWin();
	}


	return result;

}

void MachineGame::machineChooseAndMovePieces()
{
	//qDebug() << "machineChooseAndMovePieces";
	ChessStep* step = getBestMove();
	if (step->m_nKillID == -1)
	{
		this->m_chessPieces[step->m_nMoveID].m_nRow = step->m_nRowTo;
		this->m_chessPieces[step->m_nMoveID].m_nCol = step->m_nColTo;
	}
	else {
		this->m_chessPieces[step->m_nKillID].m_bDead = true;
		this->m_chessPieces[step->m_nMoveID].m_nRow = this->m_chessPieces[step->m_nKillID].m_nRow;
		this->m_chessPieces[step->m_nMoveID].m_nCol = this->m_chessPieces[step->m_nKillID].m_nCol;
		this->m_nSlectedID = -1;
	}
	this->m_bIsRed = !this->m_bIsRed;

}
