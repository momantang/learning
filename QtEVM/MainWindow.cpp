#include "MainWindow.h"
#include <QDebug>
#include "QImage"
#include <QTimeZone>
#include <QPushButton>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
	rateLabel = new QLabel;
	inputTip = "Please open a video";
	ui.videoLabel->setText(inputTip);


	rateLabel->setText("");
	ui.statusBar->addPermanentWidget(rateLabel);

	progressDialog = 0;
	magnifyDialg = 0;

	updateStatus(false);

	video = new VideoProcessor;

	connect(video, &VideoProcessor::showFrame, this, &MainWindow::showFrame);
	connect(video, &VideoProcessor::updateBtn, this, &MainWindow::updateBtn);

	connect(video, &VideoProcessor::sleep, this, &MainWindow::sleep);
	connect(video, &VideoProcessor::updateProgressBar, this, &MainWindow::updateProgressBar);
	//connect(video, &VideoProcessor::updateBtn, this, &MainWindow::updateBtn);

	//connect(video, &VideoProcessor::updateBtn, this, &MainWindow::updateBtn);

}

MainWindow::~MainWindow()
{

}

bool MainWindow::maybeSave()
{
	qDebug() << "maybe Save";
	if (video->isModified())
	{
		QMessageBox box;
		box.setWindowTitle("Warning");
		box.setIcon(QMessageBox::Warning);
		box.setText(QString("The current file %1 has been changed. save?").arg(curFile));

		QPushButton *yesBtn = box.addButton("Yes", QMessageBox::YesRole);
		box.addButton("No", QMessageBox::NoRole);
		QPushButton *cancelBtn = box.addButton("Cancel", QMessageBox::RejectRole);

		box.exec();
		if (box.clickedButton()==yesBtn)
		{
			return saveAs();
		}
		if (box.clickedButton()==cancelBtn)
		{
			return false;
		}
		
	}
	return true;
}

bool MainWindow::save()
{
	return false;
}

bool MainWindow::saveAs()
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save as ", curFile);
	if (fileName.isEmpty())
	{
		return false;
	}
	return saveFile(fileName);
}

bool MainWindow::saveFile(const QString & filename)
{
	QFileInfo fi(filename);
	if (fi.suffix()=="")
	{
		video->setOutput(fi.filePath().toStdString(),".avi");
	}
	else {
		video->setOutput(fi.filePath().toStdString());
	}
	QApplication::setOverrideCursor(Qt::WaitCursor);
	video->writeOutput();
	QApplication::restoreOverrideCursor();
	curFile = fi.canonicalFilePath();
	setWindowTitle(curFile);
	return true;
}

bool MainWindow::LoadFile(const QString& filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly))
	{
		QMessageBox::warning(this, "VideoPlayer", QString("Unable to load file %1:\n%2").arg(filename).arg(file.errorString()));
		return false;
	}

	//强制光标图标
	QApplication::setOverrideCursor(Qt::WaitCursor);
	if (!video->setInput(filename.toStdString()))
	{
		QMessageBox::warning(this, "videoPlayer", QString("unable to load file %1 \n%2.").arg(filename).arg(file.errorString()));
		return false;
	}
	QApplication::restoreOverrideCursor();

	rateLabel->setText(QString("Frame rate: %1 ").arg(video->getFrameRate()));
	updateTimeLabel();

	curFile = QFileInfo(filename).canonicalFilePath();
	setWindowTitle(curFile);
	return true;
}

void MainWindow::play()
{
	qDebug() << "play()";
	video->setDelay(1000 / video->getFrameRate());
	video->playIt();
}

void MainWindow::about()
{
	QMessageBox::about(this, "About QtEVM", "<h2>QtEVM 1.0</h2>"
		"<p>Copyright &copy; 2014 Joseph Pan"
		"(<a href=\"mailto:cs.wzpan@gmail.com\">"
		"<span style=\" text-decoration: underline; color:#0000ff;\">"
		"cs.wzpan@gmail.com</span></a>).</p>"
		"<p>Yet anther C++ implementation of EVM"
		"(<a href=\"http://people.csail.mit.edu/mrub/vidmag/\">"
		"<span style=\" text-decoration: underline; color:#0000ff;\">"
		"Eulerian Video Magnification</span></a>)"
		", based on OpenCV and Qt.</p>");
}

void MainWindow::aboutQt()
{
	qApp->aboutQt();
}

void MainWindow::on_actionOpen_triggered()
{
	qDebug() << "on_actionOpen_triggered";
	QString fileName = QFileDialog::getOpenFileName(this, "Open Video", ".", "Video File(*.avi *.mov *.mpeg *.mp4 *.mkv)");
	if (!fileName.isEmpty())
	{
		if (LoadFile(fileName))
		{
			updateStatus(true);
			updateBtn();
		}
	}

}

void MainWindow::on_actionClose_triggered()
{
	qDebug() << "on_actionClose_triggered";
	if (maybeSave())
	{
		updateStatus(false);
		ui.videoLabel->setText(inputTip);
		video->close();
		clean();
		ui.timeLabel->setText("");
		rateLabel->setText("");
	}
}

void MainWindow::on_actionQuit_triggered()
{
	qDebug() << "on_actionQuit_triggered";
	on_actionClose_triggered();
	qApp->quit();
}

void MainWindow::on_actionSave_as_triggered()
{
	qDebug() << "video -> pauseIt";
	saveAs();
}

void MainWindow::on_actionAbout_triggered()
{
	qDebug() << "on_actionAbout_triggered";
	about();
}

void MainWindow::on_actionAbout_Qt_triggered()
{
	qDebug() << "on_actionAbout_Qt_triggered";
	aboutQt();
}

void MainWindow::on_actionPlay_triggered()
{
	qDebug() << "on_actionPlay_triggered";
	play();
}

void MainWindow::on_actionStop_S_triggered()
{
	qDebug() << "on_actionStop_S_triggered";
	video->stopIt();
}

void MainWindow::on_actionLast_Frame_L_triggered()
{
	qDebug() << "on_actionLast_Frame_L_triggered";
	video->prevFrame();
}

void MainWindow::on_actionLast_Frame_N_triggered()
{
	qDebug() << "on_actionLast_Frame_N_triggered";
	video->nextFrame();
}

void MainWindow::on_actionPause_triggered()
{
	qDebug() << "on_actionPause_triggered";
	video->pauseIt();
}

void MainWindow::on_action_Morpho_triggered()
{
	qDebug() << "on_action_Morpho_triggered";
	if (!magnifyDialg)
	{
		magnifyDialg = new MagnifyDialog(this,video);
	}
	magnifyDialg->show();
}

void MainWindow::on_actionClean_Temp_Files_triggered()
{
	qDebug() << "video -> pauseIt";
}

void MainWindow::on_action_Motion_triggered()
{
	qDebug() << "video -> pauseIt";
}

void MainWindow::on_action_Color_triggered()
{
	qDebug() << "video -> pauseIt";
}

void MainWindow::on_btnNext_clicked()
{
	qDebug() << "on_btnNext_clicked";
	video->nextFrame();
}

void MainWindow::on_btnLast_clicked()
{
	qDebug() << "n_btnLast_clicked";
	video->prevFrame();
}

void MainWindow::on_btnPlay_clicked()
{
	qDebug() << "on_btnPlay_clicked";
	play();
}

void MainWindow::on_btnPause_clicked()
{
	qDebug() << "on_btnPause_clicked";
	video->pauseIt();
}

void MainWindow::on_btnStop_clicked()
{
	qDebug() << "on_btnStop_clicked";
	video->stopIt();
}

void MainWindow::on_progressSlider_sliderMoved(int position)
{
	qDebug() << "on_progressSlider_sliderMoved  :" <<position;
	long pos = position * video->getLength() / ui.progressSlider->maximum();
	video->jumpTo(pos);
	updateTimeLabel();
}

void MainWindow::updateBtn()

{
	qDebug() << "updateBtn";
	bool isStop = video->isStop();
	ui.actionPause->setVisible(!isStop);
	ui.btnPause->setVisible(!isStop);
	ui.btnPlay->setVisible(isStop);
	ui.actionPlay->setVisible(isStop);
}

void MainWindow::showFrame(cv::Mat frame)
{
	//qDebug() << "showFrame"<<frame.data;
	Mat tmp;
	cvtColor(frame, tmp, COLOR_BGR2RGB);
	QImage img = QImage(tmp.data, tmp.cols, tmp.rows, QImage::Format_RGB888);
	ui.videoLabel->setPixmap(QPixmap::fromImage(img));
	ui.videoLabel->repaint();
}

void MainWindow::sleep(int msecs)
{
	//qDebug() << "sleep "<< msecs;
	helper->sleep(msecs);
}

void MainWindow::updateProgressBar()
{
	ui.progressSlider->setValue(video->getNumberOfPlayedFrames()*ui.progressSlider->maximum() / video->getLength()*1.0);
	updateTimeLabel();
}

void MainWindow::closeEvent(QCloseEvent * event)
{
	qDebug() << "close event " << event;
	if (maybeSave())
	{
		qDebug() << "close event accept";
		event->accept();
		clean();
	}
	else {
		qDebug() << "close event ignore";
		event->ignore();
	}
}

void MainWindow::updateStatus(bool vi)
{
	for (int i=0;i<ui.menuPlay->actions().count();i++)
	{
		//ui.menuPlay.actions().at(i).setEnabled(vi);
		auto action = ui.menuPlay->actions().at(i);
		action->setEnabled(vi);
	}
	for each (auto action in ui.menuMagnification->actions())
	{
		action->setEnabled(vi);
	}
	ui.actionClose->setEnabled(vi);
	ui.actionSave_as->setEnabled(vi);
	ui.progressSlider->setEnabled(vi);
	ui.btnLast->setEnabled(vi);
	ui.btnNext->setEnabled(vi);
	ui.btnPlay->setEnabled(vi);
	ui.btnPause->setEnabled(vi);
	ui.btnStop->setEnabled(vi);
}

void MainWindow::updateTimeLabel()
{
	QTimeZone zone(Qt::UTC);
	QString curPos = QDateTime::fromMSecsSinceEpoch(video->getPositionMS(),zone).toString("hh:mm::ss zzz");
	//QString curPos = QDateTime::fromMSecsSinceEpoch(0).toString("hh:mm::ss");
	QString length = QDateTime::fromMSecsSinceEpoch(video->getLengthMS(),zone).toString("hh::mm::ss zzz");
	ui.timeLabel->setText(QString("<span style='color:#ff0000;'>%1</span>/%2").arg(curPos,length));
}

void MainWindow::clean()
{
	QString path = ".";
	QDir dir(path);
	std::string dirFile;
	while (true)
	{
		video->getTempFile(dirFile);
		if (dirFile=="")
		{
			break;
		}else{
			dir.remove(QString::fromStdString(dirFile));
		}
	}
}
