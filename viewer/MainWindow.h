#pragma once
#ifndef QTOGL_WINDOW_H
#define QTOGL_WINDOW_H

#include <QMainWindow>
//#include <QtGui/QWindow>
#include "OGLViewer.h"
#include "ImageViewer.h"
#include <QDialog>
#include "ui_MainWindow.h"
#include "ui_About.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	//~MainWindow();

public slots:
	void on_actionAbout_triggered();
	void connectimg();
	//void aboutwindow();
protected:
	void closeEvent(QCloseEvent *e);
private:

	OGLViewer* m_oglviewer;
	ImageViewer* m_imgviewer;
	QDialog* about;
	Ui::MainWindowClass ui;
	//Ui::about_dialog about;
	//Ui::img_viewer imgViewer;
};

#endif // QTOGL_WINDOW_H
