#pragma once
#ifndef QTOGL_WINDOW_H
#define QTOGL_WINDOW_H

#include "OGLViewer.h"
#include <QMainWindow>
//#include <QtGui/QWindow>
#include <QDialog>
#include <QtGui/QOpenGLFunctions>
#include "ui_MainWindow.h"
#include "ui_About.h"


class MainWindow : public QMainWindow, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void on_actionAbout_triggered();
	//void aboutwindow();
protected:
	void closeEvent(QCloseEvent *e);
private:
	bool m_update_pending;
	bool m_animating;

	OGLViewer *m_oglviewer;
	QDialog *about;
	Ui::MainWindowClass ui;
};

#endif // QTOGL_WINDOW_H
