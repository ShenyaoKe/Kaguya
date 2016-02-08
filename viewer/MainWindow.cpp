#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	m_oglviewer = new OGLViewer;

	ui.setupUi(this);
	ui.ogl_layout->addWidget(m_oglviewer);
	//setWindowTitle(tr("OpenGL Qt Template"));

	m_oglviewer->setFocusPolicy(Qt::StrongFocus);
}

void MainWindow::on_actionAbout_triggered()
{
	about = new QDialog(0,0);
	Ui::about_dialog about_ui;
	about_ui.setupUi(about);
	about->setAttribute(Qt::WA_DeleteOnClose);
	about->show();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
	Q_UNUSED(e);

	foreach(QWidget *widget, QApplication::topLevelWidgets())
	{
		widget->close();
	}
}
