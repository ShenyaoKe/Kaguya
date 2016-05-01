#pragma once
#include "common.h"
#include "GL/glew.h"
#include "Core/Kaguya.h"
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QImage>
#include "ui_ImageViewer.h"

class ImageViewerPanel;

static GLuint ogl_ver_major;
static GLuint ogl_ver_minor;

class ImageViewer : public QMainWindow
{
	Q_OBJECT
public:
	ImageViewer(QWidget* parent = nullptr);
	~ImageViewer();

	void setpixmap(const vector<uint8_t>* pixmap);
private:
	Ui::img_viewer ui;
	unique_ptr<ImageViewerPanel> img_panel;

	friend class MainWindow;
};

class ImageViewerPanel : public QOpenGLWidget
{
	Q_OBJECT
public:
	ImageViewerPanel(QWidget *parent = nullptr);
	~ImageViewerPanel();

	void setImageResolution(uint32_t w, uint32_t h);
	void updateTexture();
protected:
	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;
private:
	float frame[8];// { 0,0, w,0, w,h, 0,h }
	float imgsize[2];
	
	const vector<uint8_t>* textures;
	GLuint vao, vbo, tex;

	friend class ImageViewer;
};