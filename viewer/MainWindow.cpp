#include "MainWindow.h"

namespace Kaguya
{

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, mImageViewer(std::make_unique<ImageViewer>(this))
	, mViewer(std::make_unique<OGLViewer>(this))
{

	mUi.setupUi(this);
	mUi.ogl_layout->addWidget(mViewer.get());
	QTabWidget* shelf = new QTabWidget();
	shelf->addTab(new QWidget, "1");
	shelf->addTab(new QWidget, "2");
	mUi.shelf->addWidget(shelf);

	mViewer->setFocusPolicy(Qt::StrongFocus);

	/*connect(ui.render_button, &QPushButton::clicked, m_oglviewer, &OGLViewer::renderpixels);*/
	connect(mUi.render_button, &QPushButton::clicked, this, &MainWindow::connectimg);
}

MainWindow::~MainWindow()
{
}

void MainWindow::on_actionAbout_triggered()
{
	mAbout = std::make_unique<QDialog>(nullptr, Qt::Widget);
	Ui::about_dialog about_ui;
	about_ui.setupUi(mAbout.get());
	mAbout->setAttribute(Qt::WA_DeleteOnClose);
	mAbout->show();
}

void MainWindow::connectimg()
{
	mViewer->renderPixels();
	mImageViewer->setpixmap(mViewer->mRenderBuffer);
	mImageViewer->mImagePanel->setImageResolution(640, 480);
	mImageViewer->adjustSize();
	mImageViewer->show();
}

void MainWindow::closeEvent(QCloseEvent* /*e*/)
{
	for (QWidget* widget : QApplication::topLevelWidgets())
	{
		widget->close();
	}
}

}
