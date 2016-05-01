#pragma once
#ifndef QTOGL_WINDOW_H
#define QTOGL_WINDOW_H

#include <QMainWindow>
//#include <QtGui/QWindow>
#include "OGLViewer.h"
#include "ImageViewer.h"
#include <QDialog>
#include <QTabWidget>
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

	unique_ptr<OGLViewer> viewer;
	unique_ptr<ImageViewer> imgViewer;
	unique_ptr<QDialog> about;
	Ui::MainWindowClass ui;
	//Ui::Shelf tool_shelf;
	//Ui::about_dialog about;
	//Ui::img_viewer imgViewer;
};

#endif // QTOGL_WINDOW_H
