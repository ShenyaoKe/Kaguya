#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	m_oglviewer = new OGLViewer;
	m_imgviewer = new ImageViewer;

	ui.setupUi(this);
	ui.ogl_layout->addWidget(m_oglviewer);
	QTabWidget* shelf = new QTabWidget();
	shelf->addTab(new QWidget, "1");
	shelf->addTab(new QWidget, "2");
	ui.shelf->addWidget(shelf);
	//setWindowTitle(tr("OpenGL Qt Template"));
	
	m_oglviewer->setFocusPolicy(Qt::StrongFocus);

	/*connect(ui.render_button, &QPushButton::clicked, m_oglviewer, &OGLViewer::renderpixels);*/
	connect(ui.render_button, &QPushButton::clicked, this, &MainWindow::connectimg);

}

void MainWindow::on_actionAbout_triggered()
{
	about = new QDialog(0,0);
	Ui::about_dialog about_ui;
	about_ui.setupUi(about);
	about->setAttribute(Qt::WA_DeleteOnClose);
	about->show();
}

void MainWindow::connectimg()
{
	m_oglviewer->renderpixels();
	m_imgviewer->setpixmap(m_oglviewer->pixmap);
	m_imgviewer->show();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
	Q_UNUSED(e);

	foreach(QWidget *widget, QApplication::topLevelWidgets())
	{
		widget->close();
	}
}
