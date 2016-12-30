#pragma once

#include <QMainWindow>
//#include <QtGui/QWindow>
#include "OGLViewer.h"
#include "ImageViewer.h"
#include <QDialog>
#include <QTabWidget>
#include "ui_MainWindow.h"
#include "ui_About.h"

namespace Kaguya
{

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    //~MainWindow();

    void on_actionAbout_triggered();
    void connectimg();
    //void aboutwindow();
protected:
    void closeEvent(QCloseEvent *e);
private:

    std::unique_ptr<OGLViewer>   viewer;
    std::unique_ptr<ImageViewer> imgViewer;
    std::unique_ptr<QDialog>     about;
    Ui::MainWindowClass          ui;
    //Ui::Shelf tool_shelf;
    //Ui::about_dialog about;
    //Ui::img_viewer imgViewer;
};

}
