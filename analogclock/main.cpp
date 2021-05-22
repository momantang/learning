#include "RasterWindow.h"
#include <QtWidgets/QApplication>
class AnalogClockWindow:public RasterWindow
{
public:
	AnalogClockWindow();
protected:
	void timerEvent(QTimerEvent *event) override;
	void render(QPainter* painter) override;
private:
	int m_timerId;
};

AnalogClockWindow::AnalogClockWindow() {
	setTitle("analog clock");
	resize(200, 200);
	m_timerId = startTimer(1000);
}
void AnalogClockWindow::timerEvent(QTimerEvent *event) {
	if (event->timerId()==m_timerId)
	{
		renderLater();
	}
}
void AnalogClockWindow::render(QPainter* painter) {
	RasterWindow::render(painter);
	return;
	static const QPoint hourHand[3] = {
		QPoint(7,8),
		QPoint(-7,8),
		QPoint(0,-40) };
	static const QPoint minuteHand[3] = {
		QPoint(7,8),
		QPoint(-7,8),
		QPoint(0,-70)
	};
	static const QPoint secondHand[3] = {
		QPoint(7,8),
		QPoint(-7,8),
		QPoint(0,-90)
	};
	QColor hourColor(127, 0, 127);
	QColor minuteColor(0, 127, 127, 191);
	QColor secondColor(0, 255, 255, 200);

	painter->setRenderHint(QPainter::Antialiasing);

	painter->translate(width() / 2, height() / 2);
	int side = qMin(width(), height());
	painter->scale(side / 200.0, side / 200.0);


	painter->setPen(Qt::NoPen);
	painter->setBrush(hourColor);

	QTime time = QTime::currentTime();
	painter->save();

	painter->rotate(30.0*((time.hour() + time.minute() / 60.0)));
	painter->drawConvexPolygon(hourHand, 3);
	painter->restore();

	painter->setPen(hourColor);
	for (int i=0;i<12;i++)
	{
		painter->drawLine(88, 0, 96, 0);
		painter->rotate(30.0);
	}

	painter->setPen(Qt::NoPen);
	painter->setBrush(minuteColor);

	painter->save();
	painter->rotate(6.0*(time.minute()+time.second()/60.0));
	painter->drawPolygon(minuteHand, 3);
	painter->restore();
	painter->setPen(minuteColor);
	for (int j=0;j<60;j++)
	{
		if ((j%5)!=0)
		{
			painter->drawLine(92, 0, 96, 0);

		}
		painter->rotate(6.0);
	}
	painter->restore();
	painter->setBrush(secondColor);
	painter->setPen(Qt::NoPen);
	painter->rotate(time.second()*6.0);
	painter->drawPolygon(secondHand, 3);
	

}
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RasterWindow w;
    w.show();
    return a.exec();
}
