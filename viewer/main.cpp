#include <QApplication>
#include <QFile>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication qtApp(argc, argv);
	QFile File(":stylesheets/dark.qss");
	if (File.open(QFile::ReadOnly))
	{
		QString StyleSheet = QLatin1String(File.readAll());
		qtApp.setStyleSheet(StyleSheet);
	}
	MainWindow w;
	w.show();

	return qtApp.exec();
}
