#include "ChessPieces.h"
#include "global.h"
#include <QDebug>
struct POS
{
	int t_nRow;
	int t_nCol;
	ChessPieces::m_emTYPE t_emType;
};
POS tPOS[16] = {
	{0,0,ChessPieces::CHE},
	{0,1,ChessPieces::MA},
	{0,2,ChessPieces::XIANG},
	{0,3,ChessPieces::SHI},
	{0,4,ChessPieces::JIANG},
	{0,5,ChessPieces::SHI},
	{0,6,ChessPieces::XIANG},
	{0,7,ChessPieces::MA},
	{0,8,ChessPieces::CHE},

	
	{2,1,ChessPieces::PAO},
	{2,7,ChessPieces::PAO},
	{3,0,ChessPieces::BING},
	{3,2,ChessPieces::BING},
	{3,4,ChessPieces::BING},
	{3,6,ChessPieces::BING},
	{3,8,ChessPieces::BING}
	
};
ChessPieces::ChessPieces()
{

}

ChessPieces::~ChessPieces()
{

}

void ChessPieces::init(int id)
{
	qDebug() << "chesspieces init :" << id;
	if (id<16)
	{
		m_nRow = tPOS[id].t_nRow;
		m_nCol = tPOS[id].t_nCol;
		m_emType = tPOS[id].t_emType;
		m_bRed = false;
	}
	else
	{
		m_nRow = 9 - tPOS[id - 16].t_nRow;
		m_nCol = 8 - tPOS[id - 16].t_nCol;
		m_emType = tPOS[id - 16].t_emType;
		m_bRed = true;
	}
	m_bDead = false;
}

QString ChessPieces::getnName()
{
	switch (m_emType)
	{
	case ChessPieces::JIANG:
		return "将";
	case ChessPieces::SHI:
		return "士";;
	case ChessPieces::XIANG:
		return "象";;
	case ChessPieces::MA:
		return "马";;
	case ChessPieces::CHE:
		return "车";;
	case ChessPieces::PAO:
		return "炮";
	case ChessPieces::BING:
		return "兵";
	default:
		break;
	}
	return ""
		;
}
