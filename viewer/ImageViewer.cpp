#include "ImageViewer.h"
#include <QTimer>


ImageViewer::ImageViewer(QWidget* parent)
	: QMainWindow(parent)
	, img_panel(new ImageViewerPanel(this))
{
	ui.setupUi(this);
	ui.img_panel->addWidget(img_panel.get());
	img_panel->setFocusPolicy(Qt::StrongFocus);
}


ImageViewer::~ImageViewer()
{
}
/*

ImageViewer* ImageViewer::getInstance()
{
	if (instance == nullptr)
	{
		instance = new ImageViewer();
		return instance;
	}
}*/

void ImageViewer::setpixmap(const renderBuffer* pixmap)
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
	, drawType(0)
{
	QSurfaceFormat format;
	format.setDepthBufferSize(32);
	format.setStencilBufferSize(8);
	//format.setSamples(16);
	format.setVersion(4, 5);
	//format.setProfile(QSurfaceFormat::CoreProfile);
	this->setFormat(format);
	this->create();
}

ImageViewerPanel::~ImageViewerPanel()
{

}

void ImageViewerPanel::setImageResolution(uint32_t w, uint32_t h)
{
	this->setFixedSize(w, h);
	imgsize[0] = w;
	imgsize[1] = h;
	/*frame[2] = frame[4] = static_cast<float>(w);
	frame[5] = frame[7] = static_cast<float>(h);*/
}

void ImageViewerPanel::updateTexture()
{
	makeCurrent();
	if (!textures->empty())
	{
		for (int i = 0; i < textures->size; i++)
		{
			glTextureStorage2D(tex[i], 1, GL_RGB8, imgsize[0], imgsize[1]);
			glTextureSubImage2D(tex[i], 0, 0, 0, imgsize[0], imgsize[1], GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		}
		
	}
	doneCurrent();
}

void ImageViewerPanel::initializeGL()
{
	//glewInit();

	glGetIntegerv(GL_MAJOR_VERSION, &ogl_ver_major);
	glGetIntegerv(GL_MINOR_VERSION, &ogl_ver_minor);

	shaderP = make_unique<GLSLProgram>(
		"resources/shaders/img_vs.glsl",
		"resources/shaders/img_fs.glsl");

	if (ogl_ver_major == 4 && ogl_ver_minor >= 5)
	{
		// DSA
		// Create VAO
		glCreateBuffers(1, &vbo);
		glNamedBufferData(vbo, sizeof(frame), frame, GL_STATIC_DRAW);
		
		// IBO
		GLuint indices[] = { 0,1,2,2,3,0 };
		glCreateBuffers(1, &ibo);
		glNamedBufferData(ibo, sizeof(indices), indices, GL_STATIC_DRAW);

		// VAO
		glCreateVertexArrays(1, &vao);
		glEnableVertexArrayAttrib(vao, 0);

		// Setup the formats
		glVertexArrayAttribFormat(vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(float) * 2);
		glVertexArrayAttribBinding(vao, 0, 0);

		glVertexArrayElementBuffer(vao, ibo);

		// Setup textures
		int texSize = 4;
		glCreateTextures(GL_TEXTURE_2D, 8, tex);
		for (int i = 0; i < 8 ; i++)
		{
			glTextureParameteri(tex[i], GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(tex[i], GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTextureParameteri(tex[i], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTextureParameteri(tex[i], GL_TEXTURE_MAG_FILTER, GL_NEAREST);

			glTextureStorage2D(tex[i], 1, GL_RGB8, imgsize[0], imgsize[1]);
			if (!textures->empty())
			{
				glTextureSubImage2D(tex[i], 0, 0, 0, imgsize[0], imgsize[1], GL_RGB, GL_UNSIGNED_BYTE, &textures);
			}
			texHandle[i] = glGetTextureHandleARB(tex[i]);
			glMakeTextureHandleResidentARB(texHandle[i]);
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

	glClearColor(0.2, 0.2, 0.2, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(vao);
	shaderP->use_program();
	glUniformHandleui64ARB((*shaderP)["tex"], texHandle[drawType]);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	doneCurrent();
}

void ImageViewerPanel::resizeGL(int w, int h)
{

}
