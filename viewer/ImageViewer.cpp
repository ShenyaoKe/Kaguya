#include "ImageViewer.h"
#include <QTimer>


ImageViewer::ImageViewer(QWidget* parent)
	: QMainWindow(parent)
	, img_panel(new ImageViewerPanel)
{
	ui.setupUi(this);
	ui.img_panel->addWidget(img_panel.get());
}


ImageViewer::~ImageViewer()
{
}

void ImageViewer::setpixmap(const vector<uint8_t>* pixmap)
{
	img_panel->textures = pixmap;
	img_panel->updateTexture();
	img_panel->update();
}
//////////////////////////////////////////////////////////////////////////
ImageViewerPanel::ImageViewerPanel(QWidget *parent)
	: QOpenGLWidget(parent)
	//, frame{ 0,0, 640,0, 640,480, 0,480 }
	, frame{ -1,-1, 1,-1, 1,1, -1,1 }
	, imgsize{ 640, 480 }
	, textures(nullptr)
{
	QSurfaceFormat format;
	format.setDepthBufferSize(32);
	format.setStencilBufferSize(8);
	//format.setSamples(16);
	format.setVersion(4, 5);
	//format.setProfile(QSurfaceFormat::CoreProfile);
	this->setFormat(format);
}

ImageViewerPanel::~ImageViewerPanel()
{

}

void ImageViewerPanel::setImageResolution(uint32_t w, uint32_t h)
{
	this->setFixedSize(w, h);
	//this->setBaseSize(w, h);
	frame[2] = frame[4] = static_cast<float>(w);
	frame[5] = frame[7] = static_cast<float>(h);
}

void ImageViewerPanel::updateTexture()
{
	glTextureStorage2D(tex, 1, GL_RGB8I, imgsize[0], imgsize[1]);
}

void ImageViewerPanel::initializeGL()
{
	glewInit();

	const char* glverstr = (const char*)glGetString(GL_VERSION);
	sscanf(glverstr, "%d.%d", &ogl_ver_major, &ogl_ver_minor);
	if (ogl_ver_major == 4 && ogl_ver_minor >= 5)
	{
		// DSA
		// Create VAO
		glCreateBuffers(1, &vbo);
		glNamedBufferData(vbo, sizeof(frame), frame, GL_STREAM_DRAW);

		// VAO
		glCreateVertexArrays(1, &vao);
		glEnableVertexArrayAttrib(vao, 0);

		// Setup the formats
		glVertexArrayAttribFormat(vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(float) * 2);
		glVertexArrayAttribBinding(vao, 0, 0);

		// Setup textures
		glCreateTextures(GL_TEXTURE_2D, 1, &tex);
		glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		if (!textures->empty())
		{
			glTextureStorage2D(tex, 1, GL_RGB8I, imgsize[0], imgsize[1]);
		}
	}
	else
	{
		// Non-DSA
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		// Bind UV values
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(frame), frame, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(0);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}

void ImageViewerPanel::paintGL()
{
	makeCurrent();
	// Clear background and color buffer
	glClearColor(0.2, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(vao);
	
	glDrawArrays(GL_LINE_LOOP, 0, 4);


	glBindVertexArray(0);
}
