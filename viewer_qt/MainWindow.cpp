#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, mViewer(std::make_unique<Kaguya::OGLViewer>(this))
	//, mViewer(std::make_unique<NewGLView>(this))
	, mUi(std::make_unique<Ui::MainWindowClass>())
{

	mUi->setupUi(this);
	mUi->ogl_layout->addWidget(mViewer.get());

	//mViewer->setFocusPolicy(Qt::StrongFocus);

	/*connect(ui.render_button, &QPushButton::clicked, m_oglviewer, &OGLViewer::renderpixels);*/
	connect(mUi->render_button, &QPushButton::clicked, this, &MainWindow::connectimg);
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
}

void MainWindow::closeEvent(QCloseEvent* /*e*/)
{
	for (QWidget* widget : QApplication::topLevelWidgets())
	{
		widget->close();
	}
}
