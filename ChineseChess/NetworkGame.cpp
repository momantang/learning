#include "NetworkGame.h"
#include "global.h"
#include <QHostAddress>

NetworkGame::NetworkGame(bool isServer)
{
	m_tcpServer = NULL;
	m_tcpSocket = NULL;
	if (isServer)
	{
		m_bIsTcpServer = true;
		m_tcpServer = new QTcpServer(this);
		m_tcpServer->listen(QHostAddress::Any, 9999);

		connect(m_tcpServer, &QTcpServer::newConnection, this, &NetworkGame::slotNewConnection);
	}
	else {
		m_bIsTcpServer = false;
		m_tcpSocket = new QTcpSocket(this);
		m_tcpSocket->connectToHost(QHostAddress("127.0.0.1"), 9999);
		connect(m_tcpSocket, &QTcpSocket::readyRead, this, &NetworkGame::slotRecv);
	}
}

NetworkGame::~NetworkGame()
{

}

void NetworkGame::clickPieces(int checkID, int&row, int&col)
{
	if (m_bIsTcpServer)
	{
		if (m_nSlectedID==-1&&m_nCheckedID!=-1)
		{
			if (m_bIsTcpServer!=m_chessPieces[checkID].m_bRed)
			{
				return;
			}
		}
	}
	else {
		if (m_nSlectedID==-1&&m_nCheckedID!=-1)
		{
			if (m_bIsTcpServer!=m_chessPieces[checkID].m_bRed)
			{
				return;
			}
		}
	}
	whoWin();
	ChessBoard::clickPieces(checkID, row, col);
	char array[3];
	array[0] = checkID;
	array[1] = row;
	array[2] = col;
	m_tcpSocket->write(array, 3);
}

void NetworkGame::slotNewConnection()
{
	if (m_tcpSocket)
	{
		return;
	}
	m_tcpSocket = m_tcpServer->nextPendingConnection();
	connect(m_tcpSocket, &QTcpSocket::readyRead, this, &NetworkGame::slotRecv);
}

void NetworkGame::slotRecv()
{
	QByteArray arry = m_tcpSocket->readAll();
	int nCheckedID = arry[0];
	int nrow = arry[1];
	int ncol = arry[2];
	ChessBoard::clickPieces(nCheckedID, nrow, ncol);
}
