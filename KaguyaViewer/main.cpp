#include "MainWindow.h"

#include <QtWidgets/QApplication>
#include <QFile>

int main(int argc, char *argv[])
{

	QApplication qtApp(argc, argv);
	QFile File("stylesheet.qss");
	File.open(QFile::ReadOnly);

	QString StyleSheet = QLatin1String(File.readAll());
	qtApp.setStyleSheet(StyleSheet);
	MainWindow w;
	w.show();
	/*QSurfaceFormat format;
	format.setDepthBufferSize(32);
	format.setStencilBufferSize(8);
	format.setVersion(4, 5);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);*/

	/*OGLViewer oglWindow;
	oglWindow.show();*/
	return qtApp.exec();
}
