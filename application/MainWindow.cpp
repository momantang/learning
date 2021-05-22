#include "MainWindow.h"
#include <QtWidgets>
MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),textEdit(new QPlainTextEdit)
{
	setCentralWidget(textEdit);
	createActions();
	createStatusBar();
	readSettings();
	connect(textEdit->document(), &QTextDocument::contentsChange, this, &MainWindow::documentWasModified);
#ifndef QT_NO_SESSIONMANAGER
	QGuiApplication::setFallbackSessionManagementEnabled(false);
	connect(qApp, &QGuiApplication::commitDataRequest, this, &MainWindow::commitData);
#endif
}

void MainWindow::loadFile(const QString& filename)
{
	QFile file(filename);
	if (!file.open(QFile::ReadOnly|QFile::Text))
	{
		QMessageBox::warning(this, "Application", tr("Can not read file %1\n%2").arg(QDir::toNativeSeparators(filename), file.errorString()));
		return;
	}
	QTextStream in(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	textEdit->setPlainText(in.readAll());
	QApplication::restoreOverrideCursor();
	setCurrentFile(filename);
	statusBar()->showMessage("File loaded", 2000);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (maybeSave())
	{
		writeSettings();
		event->accept();
	}
	else
	{
		event->ignore();
	}
}

void MainWindow::newFile()
{
	if (maybeSave())
	{
		textEdit->clear();
		setCurrentFile(QString());
	}
}

void MainWindow::open()
{
	if (maybeSave())
	{
		QString fileName = QFileDialog::getOpenFileName(this);
		if (!fileName.isEmpty())
		{
			loadFile(fileName);
		}
	}
}

bool MainWindow::save()
{
	if (curFile.isEmpty())
	{
		return saveAs();
	}
	else {
		return saveFile(curFile);
	}
}

bool MainWindow::saveAs()
{
	QFileDialog dialog(this);
	dialog.setWindowModality(Qt::WindowModal);
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	if (dialog.exec()!=QDialog::Accepted)
	{
		return false;
	}
	return saveFile(dialog.selectedFiles().first());
}

void MainWindow::about()
{
	QMessageBox::about(this, "About Application", "The <b>Application</b> example demonstrates how to "
		"write modern GUI applications using Qt, with a menu bar, "
		"toolbars, and a status bar.");
}

void MainWindow::documentWasModified()
{
	setWindowModified(textEdit->document()->isModified());
}

void MainWindow::commitData(QSessionManager &mananger)
{
	if (mananger.allowsInteraction())
	{
		if (!maybeSave())
		{
			mananger.cancel();
		}
	}
	else {
		if (textEdit->document()->isModified())
		{
			save();
		}
	}
}

void MainWindow::createActions()
{
	QMenu* fileMenu = menuBar()->addMenu("&File");
	QToolBar* fileToolBar = addToolBar("File");

	//new
	const QIcon newIcon = QIcon::fromTheme("document-new",QIcon(":/images/new.png"));
	QAction *newAct = new QAction(newIcon, "&New", this);
	newAct->setShortcut(QKeySequence::New);
	newAct->setStatusTip("Create a new file");
	connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
	fileMenu->addAction(newAct);
	fileToolBar->addAction(newAct);
	//open
	const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
	QAction *openAct = new QAction(openIcon, "&Open", this);
	openAct->setShortcut(QKeySequence::Open);
	openAct->setStatusTip("Open an existing file");
	connect(openAct, &QAction::triggered, this, &MainWindow::open);
	fileMenu->addAction(openAct);
	fileToolBar->addAction(openAct);
	//save
	const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
	QAction *saveAct = new QAction(saveIcon, "&Save", this);
	saveAct->setShortcut(QKeySequence::Save);
	saveAct->setStatusTip("Save the document to disk");
	connect(saveAct, &QAction::triggered, this, &MainWindow::save);
	fileMenu->addAction(saveAct);
	fileToolBar->addAction(saveAct);
	//Saveas
	const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
	QAction *saveAsAct = fileMenu->addAction(saveAsIcon, "Save &As..", this,&MainWindow::saveAs);
	
	//fileToolBar->addAction(saveAsAct);

	fileMenu->addSeparator();
	//exit
	const QIcon exitIcon = QIcon::fromTheme("document-exit");
	QAction *exitAct = fileMenu->addAction(exitIcon, "&Exit", this,&QWidget::close);
	exitAct->setShortcut(QKeySequence::Quit);
	exitAct->setStatusTip("Exit the application");


	QMenu* editMenu = menuBar()->addMenu("&Edit");
	QToolBar *editToolBar = addToolBar("Edit");
#ifndef QT_NO_CLIPBOARD
	const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
	QAction *cutAct = new QAction(cutIcon, "&Cut", this);
	cutAct->setShortcut(QKeySequence::Cut);
	cutAct->setStatusTip("Cut the current slection's conentents to the clipboard");
	connect(cutAct, &QAction::triggered, textEdit,&QPlainTextEdit::cut);
	editMenu->addAction(cutAct);
	editToolBar->addAction(cutAct);

	const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/images/copy.png"));
	QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
	copyAct->setShortcuts(QKeySequence::Copy);
	copyAct->setStatusTip(tr("Copy the current selection's contents to the "
		"clipboard"));
	connect(copyAct, &QAction::triggered, textEdit, &QPlainTextEdit::copy);
	editMenu->addAction(copyAct);
	editToolBar->addAction(copyAct);

	const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
	QAction *pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
	pasteAct->setShortcuts(QKeySequence::Paste);
	pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
		"selection"));
	connect(pasteAct, &QAction::triggered, textEdit, &QPlainTextEdit::paste);
	editMenu->addAction(pasteAct);
	editToolBar->addAction(pasteAct);


	menuBar()->addSeparator();
#endif
	QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
	QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
	aboutAct->setStatusTip(tr("Show the application's About box"));

	

	QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
	aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	
#ifndef QT_NO_CLIPBOARD
	cutAct->setEnabled(false);
	
	copyAct->setEnabled(false);
	connect(textEdit, &QPlainTextEdit::copyAvailable, cutAct, &QAction::setEnabled);
	connect(textEdit, &QPlainTextEdit::copyAvailable, copyAct, &QAction::setEnabled);
#endif // !QT_NO_CLIPBOARD
}

void MainWindow::createStatusBar()
{
	statusBar()->showMessage("Ready");
}

void MainWindow::readSettings()
{
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	const QByteArray geometry = settings.value("geometry", QByteArray()).toByteArray();
	if (geometry.isEmpty())
	{
		const QRect availabelGeometry = QApplication::desktop()->availableGeometry(this);
		resize(availabelGeometry.width() / 3, availabelGeometry.height() / 2);
		move((availabelGeometry.width() - width()) / 2, (availabelGeometry.height() - height()) / 2);
	}
	else {
		restoreGeometry(geometry);
	}
}

void MainWindow::writeSettings()
{
	QSettings settings(QCoreApplication::organizationName(), QCoreApplication::applicationName());
	settings.setValue("geometry", saveGeometry());
}

bool MainWindow::maybeSave()
{
	if (!textEdit->document()->isModified())
	{
		return true;
	}
	const QMessageBox::StandardButton ret = QMessageBox::warning(this, "Application", "The document has been modified.\n Do you want to save your changes",QMessageBox::Save|QMessageBox::Discard|QMessageBox::Cancel);
	switch (ret)
	{
	case QMessageBox::Yes:
		return save();
	case QMessageBox::Cancel:
		return false;
	default:
		break;
	}
	return true;
}

bool MainWindow::saveFile(const QString& filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly|QFile::Text))
	{
		QMessageBox::warning(this, "application", tr("cannot write file %1\n%2").arg(QDir::toNativeSeparators(filename), file.errorString()));
		return false;
	}
	QTextStream out(&file);
	QApplication::setOverrideCursor(Qt::WaitCursor);
	out << textEdit->toPlainText();
	QApplication::restoreOverrideCursor();
	setCurrentFile(filename);
	statusBar()->showMessage("file saved", 2000);

	return true;
}

void MainWindow::setCurrentFile(const QString& filename)
{
	curFile = filename;
	textEdit->document()->setModified(false);
	setWindowModified(false);

	QString shownName = curFile;
	if (curFile.isEmpty())
	{
		shownName = "untitled.txt";
	}
	setWindowFilePath(shownName);
}

QString MainWindow::strippedName(const QString& fullFilename)
{
	return QFileInfo(fullFilename).fileName();
}
