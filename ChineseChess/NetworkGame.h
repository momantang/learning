#pragma once
#include <QWidget>
#include "ChessBoard.h"
#include <QTcpServer>
#include <QTcpSocket>
class NetworkGame:public ChessBoard
{
	Q_OBJECT
public:
	NetworkGame(bool isServer);
	~NetworkGame();
	QTcpServer* m_tcpServer;
	QTcpSocket* m_tcpSocket;

	virtual void clickPieces(int checkID, int&row, int&col);
public slots:
	void slotNewConnection();
	void slotRecv();
};

