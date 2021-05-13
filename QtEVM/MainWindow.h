#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include <QCloseEvent>
#include <QDateTime>
#include <QTime>
#include <QFileDialog>
#include <QPushButton>
#include <QMessageBox>
#include <QProgressDialog>
#include <QLabel>
#include <QString>
#include <queue>
#include <opencv2/opencv.hpp>

#include "MagnifyDialog.h"
#include "VideoProcessor.h"
#include "WindowHelper.h"
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();
	bool maybeSave();
	bool save();
	bool saveAs();
	bool saveFile(const QString& filename);
	bool LoadFile(const QString& filename);
	void play();
	void about();
	void aboutQt();

private slots:
	void on_actionOpen_triggered();
	void on_actionClose_triggered();
	void on_actionQuit_triggered();
	void on_actionSave_as_triggered();
	void on_actionAbout_triggered();
	void on_actionAbout_Qt_triggered();
	void on_actionPlay_triggered();
	void on_actionStop_S_triggered();
	void on_actionLast_Frame_L_triggered();
	void on_actionLast_Frame_N_triggered();
	void on_actionPause_triggered();
	void on_action_Morpho_triggered();
	void on_actionClean_Temp_Files_triggered();
	void on_action_Motion_triggered();
	void on_action_Color_triggered();
	

	void on_btnNext_clicked();
	void on_btnLast_clicked();
	void on_btnPlay_clicked();
	void on_btnPause_clicked();
	void on_btnStop_clicked();

	void on_progressSlider_sliderMoved(int position);

	void updateBtn();
	void showFrame(cv::Mat frame);
	void sleep(int msecs);

	void updateProgressBar();

protected:
	void closeEvent(QCloseEvent *event);
private:
    Ui::MainWindowClass ui;
	QProgressDialog *progressDialog;
	MagnifyDialog *magnifyDialg;
	QString inputTip;
	QString curFile;
	QLabel * rateLabel;
	VideoProcessor * video;
	WindowHelper *helper;
	void updateStatus(bool vi);
	void updateTimeLabel();
	void clean();
};
