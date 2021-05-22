#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

QT_BEGIN_NAMESPACE
class QAction;
class QMenu;
class QPlainTextEdit;
class QSessionManager;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);
	void loadFile(const QString& filename);
protected:
	void closeEvent(QCloseEvent *event) override;
private slots:
	void newFile();
	void open();
	bool save();
	bool saveAs();
	void about();
	void documentWasModified();
#ifndef QT_NO_SESSIONMANAGER
	void commitData(QSessionManager &);
#endif
private:
	void createActions();
	void createStatusBar();
	void readSettings();
	void writeSettings();
	bool maybeSave();
	bool saveFile(const QString& filename);
	void setCurrentFile(const QString& filename);
	QString strippedName(const QString& fullFilename);

	QPlainTextEdit *textEdit;
	QString curFile;
};
