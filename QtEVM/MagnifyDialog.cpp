#include "MagnifyDialog.h"

MagnifyDialog::MagnifyDialog(QWidget *parent /*= 0*/,VideoProcessor *processor):QDialog(parent)
{
	ui.setupUi(this);
	this->processor = processor;
	alphaStr = ui.label->text();
	lambdaStr = ui.label_2->text();
	flStr = ui.label_3->text();
	fhStr = ui.label_4->text();
	chromStr = ui.label_5->text();

	std::stringstream ss;
	ss.str("");
	ss << alphaStr.toStdString() << processor->alpha;
	ui.label->setText(QString::fromStdString(ss.str()));

	ss.str("");
	ss << lambdaStr.toStdString() << processor->lambda;
	ui.label_2->setText(QString::fromStdString(ss.str()));

	ss.str("");
	ss << flStr.toStdString() << processor->fl;
	ui.label_3->setText(QString::fromStdString(ss.str()));

	ss.str("");
	ss << fhStr.toStdString() << processor->fh;
	ui.label_4->setText(QString::fromStdString(ss.str()));

	ss.str("");
	ss << chromStr.toStdString() << processor->chromattenuation;
	ui.label_5->setText(QString::fromStdString(ss.str()));

}

MagnifyDialog::~MagnifyDialog()
{
	delete &ui;
}

void MagnifyDialog::on_alphaSlider_valueChanged(int value)
{
	processor->alpha = value;
	std::stringstream ss;
	ss << alphaStr.toStdString() << processor->alpha;
	ui.label->setText(QString::fromStdString(ss.str()));
}

void MagnifyDialog::on_lambdaSlider_valueChanged(int value)
{
	processor->lambda = value;
	std::stringstream ss;
	ss << lambdaStr.toStdString() << processor->lambda;
	ui.label_2->setText(QString::fromStdString(ss.str()));
}

void MagnifyDialog::on_flSlider_valueChanged(int value)
{
	processor->fl = value;
	std::stringstream ss;
	ss << flStr.toStdString() << processor->fl;
	ui.label_3->setText(QString::fromStdString(ss.str()));
}

void MagnifyDialog::on_fhSlider_valueChanged(int value)
{
	processor->fh = value;
	std::stringstream ss;
	ss << fhStr.toStdString() << processor->fh;
	ui.label_4->setText(QString::fromStdString(ss.str()));
}

void MagnifyDialog::on_chromSlider_valueChanged(int value)
{
	processor->chromattenuation = value;
	std::stringstream ss;
	ss << chromStr.toStdString() << processor->chromattenuation;
	ui.label_5->setText(QString::fromStdString(ss.str()));
}
