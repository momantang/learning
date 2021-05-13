#pragma once
#include <QDialog>
#include <QWidget>
#include "VideoProcessor.h"
#include "ui_MagnifyDialog.h"
class MagnifyDialog:public QDialog
{
	Q_OBJECT
public:
	explicit MagnifyDialog(QWidget *parent = 0, VideoProcessor *processor = 0);
	
	//explicit MagnifyDialog(QWidget *parent = 0);
	~MagnifyDialog();
private slots:
	void on_alphaSlider_valueChanged(int value);

	void on_lambdaSlider_valueChanged(int value);

	void on_flSlider_valueChanged(int value);

	void on_fhSlider_valueChanged(int value);

	void on_chromSlider_valueChanged(int value);
private:
	Ui::Dialog ui;
	VideoProcessor * processor;
	QString alphaStr, lambdaStr, flStr, fhStr, chromStr;
public:
	
};

