#include "ChessBoard.h"
#include "ui_ChessBoard.h"
#include <QTimer>
#include <QDebug>
#include "global.h"




ChessBoard::ChessBoard(QWidget *parent)
	: QMainWindow(parent)
{
	qDebug() << "ChessBorad::cb";
	ui = new Ui::ChessBoard();
	ui->setupUi(this);
	ui->label->setText("");

	init();

	m_timer = new QTimer;
	m_timeRecord = new QTime(0, 0, 0);
	m_bIsStart = false;
	connect(m_timer, &QTimer::timeout, this, &ChessBoard::updateTime);


}

ChessBoard::~ChessBoard()
{
	delete ui;
}

bool ChessBoard::isDead(int id)
{
	if (id==-1)
	{
		return true;
	}
	return m_chessPieces[id].m_bDead;
}

int ChessBoard::getStoneId(int row, int col)
{
	for (int i = 0; i < 32; ++i)
	{
		if (m_chessPieces[i].m_nRow == row && m_chessPieces[i].m_nCol == col && !isDead(i))
			return i;
	}
	return -1;
}

int ChessBoard::getStoneCountAtLine(int row1, int col1, int row2, int col2)
{
	int ret =0;
	if (row1!=row2&&col1!=col2)
	{
		return -1;
	}
	if (row1==row2&&col1==col2)
	{
		return -1;
	}
	if (row1==row2)
	{
		int min = col1<col2?col1:col2;
		int max = col1<col2?col2:col1;
		for (int c=min+1;c<max;c++)
		{
			if (getStoneId(row1,c)!=-1)
			{
				ret++;
			}
		}
	}
	else {
		int min = row1 < row2 ? row1 : row2;
		int max = row1 < row2 ? row2 : row1;
		for (int r=min+1;r<max;r++)
		{
			if (getStoneId(r,col1)!=-1)
			{
				ret++;
			}
		}
	}
	return ret;
}

void ChessBoard::whoWin()
{
	if (m_chessPieces[4].m_bDead&&!m_chessPieces[20].m_bDead)
	{
		m_bISOver = true;
		if (m_bIsStart)
		{
			m_timer->stop();
			m_bIsStart = false;
		}
		ui->pushButton_start ->setEnabled(false);
		QMessageBox box(QMessageBox::Information, "提示", "本剧结束，红方胜利");
		box.exec();
	}
	if (m_chessPieces[20].m_bDead && !m_chessPieces[4].m_bDead)
	{
		m_bISOver = true;
		if (m_bIsStart)
		{
			m_timer->stop();
			m_bIsStart = false;
		}
		ui->pushButton_start ->setEnabled(false);
		QMessageBox box(QMessageBox::Information, "提示", "本剧结束，黑方胜利");
		box.exec();
	}

}

bool ChessBoard::isChecked(QPoint pt, int &row, int &col)
{
	for (row=0;row<=9;row++)
	{
		for (col=0;col<=8;col++)
		{
			QPoint p = center(row, col);
			int x = p.x() - pt.x();
			int y = p.y() - pt.y();
			if (x*x+y*y<m_nR*m_nR)
			{
				return true;
			}
		}
	}
	return false;
}

QPoint ChessBoard::getRealPoint(QPoint pt)
{
	int side = qMin(int((ui->centralWidget->width() - ui->widget->width()) / 0.9), ui->label->height());
	QPoint ret;
	ret.setX(pt.x() / double(side)*960.0);
	ret.setY(pt.y() / double(side)*960.0);
	return ret;
}

QPoint ChessBoard::center(int row, int col)
{
	QPoint rePoint;
	rePoint.ry() = row * m_nD + m_nOffset;
	rePoint.rx() = col * m_nD + m_nOffset;
	return rePoint;
}

QPoint ChessBoard::center(int id)
{
	return center(m_chessPieces[id].m_nRow,m_chessPieces[id].m_nCol);
}

void ChessBoard::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	int side = qMin(int((ui->centralWidget->width() - ui->widget->width()) / 0.9),ui->label->height());
	qDebug() << "side " << side;
	painter.scale(side / 960.0, side / 960.0);
	m_nOffset = 60;
	int m_nOffSet = m_nOffset;
	m_nD = 90;
	m_nR = m_nD / 2;

	//绘画棋盘
	//绘制10条横线
	for (int i = 0; i <= 9; i++)
	{
		painter.drawLine(QPoint(m_nOffset, m_nOffset + i * m_nD), QPoint(m_nOffset + 8 * m_nD, m_nOffset + i * m_nD));
	}
	//绘制9条竖线
	for (int i=0;i<9;i++)
	{
		if (i==0||i==8)
		{
			painter.drawLine(QPoint(m_nOffset+i*m_nD,m_nOffset), QPoint(m_nOffset+i*m_nD,m_nOffset+9*m_nD));
		}
		else {
			painter.drawLine(QPoint(m_nOffset + i * m_nD, m_nOffset), QPoint(m_nOffset + i * m_nD, m_nOffset + 4 * m_nD));
			painter.drawLine(QPoint(m_nOffset + i * m_nD, m_nOffset + 5 * m_nD), QPoint(m_nOffset + i * m_nD, m_nOffset + 9 * m_nD));
		}
	}
	//绘画4条斜线
	painter.drawLine(QPoint(m_nOffSet + 3 * m_nD, m_nOffSet), QPoint(m_nOffSet + 5 * m_nD, m_nOffSet + 2 * m_nD));
	painter.drawLine(QPoint(m_nOffSet + 3 * m_nD, m_nOffSet + 2 * m_nD), QPoint(m_nOffSet + 5 * m_nD, m_nOffSet));
	painter.drawLine(QPoint(m_nOffSet + 3 * m_nD, m_nOffSet + 7 * m_nD), QPoint(m_nOffSet + 5 * m_nD, m_nOffSet + 9 * m_nD));
	painter.drawLine(QPoint(m_nOffSet + 3 * m_nD, m_nOffSet + 9 * m_nD), QPoint(m_nOffSet + 5 * m_nD, m_nOffSet + 7 * m_nD));

	QRect rect1(m_nOffSet + m_nD, m_nOffSet + 4 * m_nD, m_nD, m_nD);
	QRect rect2(m_nOffSet + 2 * m_nD, m_nOffSet + 4 * m_nD, m_nD, m_nD);
	QRect rect3(m_nOffSet + 5 * m_nD, m_nOffSet + 4 * m_nD, m_nD, m_nD);
	QRect rect4(m_nOffSet + 6 * m_nD, m_nOffSet + 4 * m_nD, m_nD, m_nD);

	painter.setFont(QFont("隶书", m_nR, 800));
	painter.drawText(rect1, "楚", QTextOption(Qt::AlignCenter));
	painter.drawText(rect2, "河", QTextOption(Qt::AlignCenter));
	painter.drawText(rect3, "汉", QTextOption(Qt::AlignCenter));
	painter.drawText(rect4, "界", QTextOption(Qt::AlignCenter));
	
	for (int i=0;i<32;i++)
	{
		drawChessPieces(painter, i);
	}
	
}

void ChessBoard::drawChessPieces(QPainter& painter, int id)
{
	if (m_chessPieces[id].m_bDead)
	{
		return;
	}
	QPoint temp = center(id);
	QRect rect(temp.x() - m_nR, temp.y() - m_nR, m_nD, m_nD);
	if (m_nSlectedID==id)
	{
		painter.setBrush(QBrush(QColor(64,64,196,250)));
	}
	else {
		painter.setBrush(QBrush(QColor(64, 64, 196, 100)));
	}
	painter.setPen(QColor(0, 0, 0));
	painter.drawEllipse(center(id), m_nR, m_nR);
	painter.setFont(QFont("华文行楷", m_nR, 700));
	if (id<16)
	{
		painter.setPen(QColor(0,0,0));
	}
	else {
		painter.setPen(QColor(255, 0, 0));
	}
	painter.drawText(rect, m_chessPieces[id].getnName()+m_chessPieces[id].m_bRed, QTextOption(Qt::AlignCenter));
}

void ChessBoard::mousePressEvent(QMouseEvent *event)
{
	//debugChess();
	qDebug() << "mousePressEvent (" << event->x()<< " " << event->y()<<")";
	if (event->button()!=Qt::LeftButton||event->type()!=QEvent::Type::MouseButtonPress)
	{
		return ;
	}
	QPoint pt = event->pos();
	pt = getRealPoint(pt);
	int row, col;
	if (!isChecked(pt,row,col))
	{
		return;
	}
	if (m_bISOver)
	{
		QMessageBox box(QMessageBox::Information, "提示", "本剧已结束");
		box.exec();
		return;
	}
	int i;
	m_nCheckedID = -1;
	for (i=0;i<32;i++)
	{
		if ((m_chessPieces[i].m_nRow==row)&&(m_chessPieces[i].m_nCol==col)&&(m_chessPieces[i].m_bDead==false))
		{
			break;
		}
	}
	if (i>=0&&i<32)
	{
		m_nCheckedID = i;
	}
	else {
		m_nCheckedID = -1;
	}
	clickPieces(m_nCheckedID, row, col);
	update();
	whoWin();
}

void ChessBoard::clickPieces(int checkedId, int &row, int &col)
{
	qDebug() << "m_nCheckedID "<<m_nCheckedID<< "m_nSlectedID  " <<m_nSlectedID;
	m_nCheckedID = checkedId;
	if (m_nSlectedID==-1)
	{
		if (m_nCheckedID!=-1)
		{
			
			if (m_bIsRed==m_chessPieces[checkedId].m_bRed)
			{
				m_nSlectedID = m_nCheckedID;
			}
		}
	}
	else {
		if (canMove(m_nSlectedID,m_nCheckedID,row,col))
		{
			m_chessPieces[m_nSlectedID].m_nRow = row;
			m_chessPieces[m_nSlectedID].m_nCol = col;
			if (m_nCheckedID!=-1)
			{
				m_chessPieces[m_nCheckedID].m_bDead = true;
			}
			m_nSlectedID = -1;
			m_bIsRed = !m_bIsRed;
		}
	}
}

bool ChessBoard::canMove(int moveId, int killId, int row, int col)
{
	qDebug() << moveId;
	qDebug() << killId;
	auto &p = m_chessPieces[moveId];
	auto &k = m_chessPieces[killId];
	qDebug() << p.m_nRow <<" "<< p.m_nCol << " kill  " << k.m_nRow << " " << k.m_nCol;
	if (p.m_bRed==k.m_bRed)
	{
		if (killId==-1)
		{
			switch (p.m_emType)
			{
			case  ChessPieces::JIANG:
					return canMoveJIANG(moveId, killId, row, col);
			case  ChessPieces::XIANG:
				return canMoveXIANG(moveId, killId, row, col);
			case  ChessPieces::SHI:
				return canMoveSHI(moveId, killId, row, col);
			case  ChessPieces::MA:
				return canMoveMA(moveId, killId, row, col);
			case  ChessPieces::PAO:
				return canMovePAO(moveId, killId, row, col);
			case  ChessPieces::CHE:
				return canMoveCHE(moveId, killId, row, col);
			case  ChessPieces::BING:
				return canMoveBING(moveId, killId, row, col);
			}
		}
		m_nSlectedID = killId;
		return false;
	}
	else {
		switch (p.m_emType)
		{
		case  ChessPieces::JIANG:
			return canMoveJIANG(moveId, killId, row, col);
		case  ChessPieces::XIANG:
			return canMoveXIANG(moveId, killId, row, col);
		case  ChessPieces::SHI:
			return canMoveSHI(moveId, killId, row, col);
		case  ChessPieces::MA:
			return canMoveMA(moveId, killId, row, col);
		case  ChessPieces::PAO:
			return canMovePAO(moveId, killId, row, col);
		case  ChessPieces::CHE:
			return canMoveCHE(moveId, killId, row, col);
		case  ChessPieces::BING:
			return canMoveBING(moveId, killId, row, col);
		default:
			break;
		}
	}
	return true;
}

bool ChessBoard::canMoveJIANG(int moveId, int killId, int row, int col)
{
	if (m_chessPieces[moveId].m_bRed)
	{
		if (row<7||col<3||col>5)
		{
			return false;
		}
	}
	else {
		if (row>2||col<3||col>5)
		{
			return false;
		}
	}
	int dr = m_chessPieces[moveId].m_nRow - row;
	int dc = m_chessPieces[moveId].m_nCol - col;
	int d = abs(dr) * 10 + abs(dc);
	if (d==1||d==10)
	{
		return true;
	}
	return false;
}

bool ChessBoard::canMoveSHI(int moveId, int killId, int row, int col)
{
	if (m_chessPieces[moveId].m_bRed)
	{
		if (row<7||col<3||col>5)
		{
			return false;
		}
	}
	else
	{
		if (row>2||col<3||col>5)
		{
			return false;
		}
	}
	int dr = m_chessPieces[moveId].m_nRow - row;
	int dc = m_chessPieces[moveId].m_nCol - col;
	int d = abs(dr) * 10 + abs(dc);
	if (d==11)
	{
		return true;
	}
	return false;
}

bool ChessBoard::canMoveXIANG(int moveId, int killId, int row, int col)
{
	if ((m_chessPieces[moveId].m_bRed&&row<5)||((!m_chessPieces[moveId].m_bRed)&&row>4))
	{
		return false;
	}
	int dr = m_chessPieces[moveId].m_nRow - row;
	int dc = m_chessPieces[moveId].m_nCol - col;
	int d = abs(dc) * 10 + abs(dr);

	//象眼
	int dr2 = (m_chessPieces[moveId].m_nRow + row)/2;
	int dc2 = (m_chessPieces[moveId].m_nCol + col)/2;
	int i = 0;
	for (i=0;i<=31;i++)
	{
		if (m_chessPieces[i].m_nRow==dr2&&m_chessPieces[i].m_nCol==dc2&&(!m_chessPieces[i].m_bDead))
		{
			break;
		}
	}
	if (0<=i&&i<=31)
	{
		return false;
	}
	if (d==22)
	{
		return true;
	}
	return false;
}

bool ChessBoard::canMoveMA(int moveId, int killId, int row, int col)
{
	int dr = m_chessPieces[moveId].m_nRow - row;
	int dc = m_chessPieces[moveId].m_nCol - col;
	int d = abs(dr) * 10 + abs(dc);
	if (d!=12&&d!=21)
	{
		return false;
	}

	int dr2 = (m_chessPieces[moveId].m_nRow + row) / 2;
	int dc2 = (m_chessPieces[moveId].m_nCol + col) / 2;
	//憋马脚
	if (abs(dr)==2&&abs(dc)==1)
	{
		int i = 0;
		if (row<m_chessPieces[moveId].m_nRow)
		{
			for ( i=0;i<32;i++)
			{
				if (m_chessPieces[i].m_nRow == (m_chessPieces[moveId].m_nRow - 1) && m_chessPieces[i].m_nCol == m_chessPieces[moveId].m_nCol && m_chessPieces[i].m_bDead == false)
					break;
			}
		}
		else {
			for (i = 0; i <= 31; i++)
			{
				if (m_chessPieces[i].m_nRow == (m_chessPieces[moveId].m_nRow + 1) && m_chessPieces[i].m_nCol == m_chessPieces[moveId].m_nCol && m_chessPieces[i].m_bDead == false)
					break;
			}
		}
		if (i>=0&&i<=31)
		{
			return false;
		}
	}

	if (abs(dr) == 1 && abs(dc) == 2)
	{
		int i = 0;
		if (col < m_chessPieces[moveId].m_nCol)
		{

			for (i = 0; i <= 31; i++)
			{
				if (m_chessPieces[i].m_nRow == m_chessPieces[moveId].m_nRow && m_chessPieces[i].m_nCol == (m_chessPieces[moveId].m_nCol - 1) && m_chessPieces[i].m_bDead == false)
					break;
			}
		}
		else
		{
			for (i = 0; i <= 31; i++)
			{
				if (m_chessPieces[i].m_nRow == m_chessPieces[moveId].m_nRow && m_chessPieces[i].m_nCol == (m_chessPieces[moveId].m_nCol + 1) && m_chessPieces[i].m_bDead == false)
					break;
			}
		}

		if (0 <= i && i <= 31)
			return false;

	}
	return true;
}

bool ChessBoard::canMoveCHE(int moveId, int killId, int row, int col)
{
	int ret = getStoneCountAtLine(row, col, m_chessPieces[moveId].m_nRow, m_chessPieces[moveId].m_nCol);
	if (ret==0)
	{
		return true;
	}
	return false;
}

bool ChessBoard::canMovePAO(int moveId, int killId, int row, int col)
{
	int ret = getStoneCountAtLine(row, col, m_chessPieces[moveId].m_nRow, m_chessPieces[moveId].m_nCol);
	if (killId!=-1)
	{
		if (ret==1)
		{
			return true;
		}
	}
	else {
		if (ret==0)
		{
			return true;
		}
	}
	return false;
}

bool ChessBoard::canMoveBING(int moveId, int killId, int row, int col)
{
	int r = m_chessPieces[moveId].m_nRow ;
	int c = m_chessPieces[moveId].m_nCol ;
	int d = abs(r-row) * 10 + abs(c-col);
	if (d!=1&&d!=10)
	{
		return false;
	}
	if (m_chessPieces[moveId].m_bRed)
	{
		if (row > r) return false;
		if (r==5||r==6)
		{
			if (c==col&&r-1==row)
			{
				return true;
			}
		}
		else
		{
			if ((row<=4&&col==c)||(row==r&&abs(col-c)==1))
			{
				return true;
			}
		}
		
	}
	else {
		if (row < r)return false;
		{
			if (r==3||r==4)
			{
				if (col==c&&row==r+1)
				{
					return true;
				}
			}
			else
			{
				if ((col==c&&row>=5)||(row==r&&abs(col-c)==1))
				{
					return true;
				}
			}
		}
	}
	return false;
}

void ChessBoard::debugChess()
{
	qDebug() << "/*************debug*************/";
		for (int i=0;i<32;i++)
		{
			qDebug() <<m_chessPieces[i].m_emType<<" "<< m_chessPieces[i].m_nRow<<" "<<m_chessPieces[i].m_nCol<<" "<<m_chessPieces[i].m_bDead;
		}
}

void ChessBoard::init()
{
	qDebug() << "init()";
	for (int i=0;i<32;i++)
	{
		m_chessPieces[i].init(i);
	}
	m_nSlectedID = -1;
	m_nCheckedID = -1;
	m_bIsTcpServer = true;
	m_bIsRed = true;
	m_bISOver = false;
}

void ChessBoard::updateTime()
{
	*m_timeRecord = m_timeRecord->addSecs(1);
	qDebug() << m_timeRecord->toString("hh:mm::ss");
	ui->lcdNumber->display(m_timeRecord->toString("hh:mm:ss"));
	if (m_bIsStart)
	{
		ui->pushButton_start->setText("暂停");
	}
	else {
		ui->pushButton_start->setText("开始");
	}
}

void ChessBoard::on_pushButton_start_clicked()
{
	qDebug() << "on_pushButton_start_clicked";
	qDebug() << " start ：  " << m_bIsStart;
	if (!m_bIsStart)
	{
		m_timer->start(1000);
		ui->pushButton_start->setText("暂停");
	}
	else {
		m_timer->stop();
		ui->pushButton_start->setText("继续");
	}
	m_bIsStart = !m_bIsStart;
}

void ChessBoard::on_pushButton_reset_clicked()
{
	qDebug() << "on_pushButton_reset_clicked";
	m_timer->stop();
	m_timeRecord->setHMS(0, 0, 0);
	ui->lcdNumber->display(m_timeRecord->toString("hh:mm::ss"));
	m_bIsStart = false;
	ui->pushButton_start->setText("开始");
	ui->pushButton_start->setEnabled(true);
}

void ChessBoard::on_pushButton_about_clicked()
{
	qDebug() << "Qt About";
	qApp->aboutQt();
}

void ChessBoard::on_pushButton_restart_clicked()
{
	qDebug() << "on_pushButton_restart_clicked";
	init();
	on_pushButton_reset_clicked();
	update();
}

int ChessBoard::getD(int moveId, int row, int col)
{
	int dr = m_chessPieces[moveId].m_nRow - row;
	int dc = m_chessPieces[moveId].m_nCol - col;
	return abs(dr) * 10 + abs(dc);
}
