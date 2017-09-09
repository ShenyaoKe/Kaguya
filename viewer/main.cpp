#include <QApplication>
#include <QFile>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
#if QT_VERSION > QT_VERSION_CHECK(5, 5, 0)
	qunsetenv("QT_DEVICE_PIXEL_RATIO");
	qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", QByteArray("1"));
#else
	qputenv("QT_DEVICE_PIXEL_RATIO", QByteArray("0"));
#endif

	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication qtApp(argc, argv);
	QFile File(":stylesheets/dark.qss");
	if (File.open(QFile::ReadOnly))
	{
		QString StyleSheet = QLatin1String(File.readAll());
		qtApp.setStyleSheet(StyleSheet);
	}
	Kaguya::MainWindow w;
	w.show();

	return qtApp.exec();
}
