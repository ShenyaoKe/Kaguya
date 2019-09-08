#pragma once

#include <QDialog>
#include <QMainWindow>
#include <QTabWidget>
#include "OGLViewer.h"
//#include "NewGLView.h"
#include "ui_MainWindow.h"
#include "ui_About.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	void on_actionAbout_triggered();
	void connectimg();
	//void aboutwindow();
protected:
	void closeEvent(QCloseEvent *e);
private:
	std::unique_ptr<Kaguya::OGLViewer>   mViewer;
	//std::unique_ptr<NewGLView>   mViewer;
	std::unique_ptr<QDialog>     mAbout;
	std::unique_ptr<Ui::MainWindowClass>          mUi;
};
