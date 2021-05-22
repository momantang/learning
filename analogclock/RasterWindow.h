#pragma once

#include <QWindow>
#include <QtGui>

class RasterWindow : public QWindow
{
    Q_OBJECT

public:
   explicit RasterWindow(QWindow *parent = Q_NULLPTR);
   virtual void render(QPainter *painter);
public slots:
	void renderLater();
	void renderNow();
protected:
	bool event(QEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;
	void exposeEvent(QExposeEvent * event) override;

private:
	QBackingStore *m_backStore;
};
