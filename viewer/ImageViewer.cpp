#include "ImageViewer.h"



ImageViewer::ImageViewer(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}


ImageViewer::~ImageViewer()
{
}

void ImageViewer::setpixmap(const vector<uint8_t> &pixmap)
{
	QImage img(&pixmap[0], default_resX, default_resY, QImage::Format::Format_RGB888);
	ui.img_space->setPixmap(QPixmap::fromImage(img.transformed(QTransform::fromScale(1, -1))));
}
