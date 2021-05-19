#pragma once
#include <QObject>
class ChessStep:public QObject
{
	Q_OBJECT
public:
	explicit ChessStep(QObject* parent = 0);
	~ChessStep();
	int m_nMoveID;
	int m_nKillID;
	int m_nRowFrom;
	int m_nColFrom;
	int m_nRowTo;
	int m_nColTo;


};

