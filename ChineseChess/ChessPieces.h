#pragma once
#include <QString>

class ChessPieces
{
public:
	ChessPieces();
	~ChessPieces();

	void init(int id);
	QString getnName();//棋子对应的汉字
	enum  m_emTYPE
	{
		JIANG,
		SHI,
		XIANG,
		MA,
		CHE,
		PAO,
		BING
	};
	int m_nRow;
	int m_nCol;
	int m_nID;
	bool m_bDead;
	bool m_bRed;
	m_emTYPE m_emType;
};

