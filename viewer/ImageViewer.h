#pragma once
#include "common.h"
#include "GL/glew.h"
#include "OpenGL_Utils/GLSLProgram.h"
#include "Core/Kaguya.h"
#include "Tracer/renderBuffer.h"
#include <QMainWindow>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QImage>
#include "ui_ImageViewer.h"

class ImageViewerPanel;

static GLint ogl_ver_major;
static GLint ogl_ver_minor;

class ImageViewer : public QMainWindow
{
	Q_OBJECT
public:
	ImageViewer(QWidget* parent = nullptr);
	~ImageViewer();
	//ImageViewer* getInstance();
	void setpixmap(const renderBuffer* pixmap);
private:

	//static ImageViewer* instance;
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
	void resizeGL(int w, int h) Q_DECL_OVERRIDE;
private:
	float frame[8];// { 0,0, w,0, w,h, 0,h }
	uint32_t imgsize[2];
	
	unique_ptr<GLSLProgram> shaderP;
	GLuint vao, vbo, ibo;

	GLuint tex;
	GLuint64 texHandle;
	uint32_t texLen;
	const void* textures;
	GLenum texType;

	friend class ImageViewer;
};