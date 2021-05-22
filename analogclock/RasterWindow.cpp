#include "RasterWindow.h"

RasterWindow::RasterWindow(QWindow *parent /*= Q_NULLPTR*/):QWindow(parent),m_backStore(new QBackingStore(this))
{
	setGeometry(100, 100, 300, 200);
}

void RasterWindow::render(QPainter *painter)
{
	//painter->drawText(QRectF(0, 0, width(), height()), Qt::AlignCenter, QStringLiteral("QWindow"));
}

void RasterWindow::renderLater()
{
	this->requestUpdate();
}

void RasterWindow::renderNow()
{
	if (!isExposed())
	{
		return;
	}
	QRect rect(0, 0, width(), height());
	m_backStore->beginPaint(rect);
	QPaintDevice*device = m_backStore->paintDevice();
	QPainter painter(device);
	painter.fillRect(0, 0, width(), height(), QGradient::OldHat);
	render(&painter);
	painter.end();
	m_backStore->endPaint();
	m_backStore->flush(rect);
}

bool RasterWindow::event(QEvent * event)
{
	if (event->type()==QEvent::UpdateRequest)
	{
		renderNow();
		return true;
	}
	return QWindow::event(event);
}

void RasterWindow::resizeEvent(QResizeEvent *event)
{
	m_backStore->resize(event->size());
}

void RasterWindow::exposeEvent(QExposeEvent * event)
{
	if (isExposed())
	{
		renderNow();
	}
}
