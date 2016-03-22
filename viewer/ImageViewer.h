#pragma once
#include "common.h"
#include "Core/Kaguya.h"
#include <QMainWindow>
#include <QImage>
#include "ui_ImageViewer.h"

class ImageViewer : public QMainWindow
{
public:
	ImageViewer(QWidget* parent = nullptr);
	~ImageViewer();

	void setpixmap(const vector<uint8_t> &pixmap);
private:
	Ui::img_viewer ui;
};

