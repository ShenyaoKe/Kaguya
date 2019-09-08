#pragma once

#include "common.h"

#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QString>
#include <QFileDialog>
#include <QtGui/QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include "Math/MathUtil.h"
#include "Image/ImageData.h"
#include "Core/Scene.h"
#include "Geometry/Intersection.h"

#include "Camera/PerspectiveCamera.h"
#include "Tracer/RenderBuffer.h"

#include "QtOpenGLFunctions.h"
#include "ResolutionGate.h"

// OpenGL Window in Qt
namespace Kaguya
{

struct RenderBufferObject
{
	std::shared_ptr<QOpenGLShaderProgram>  shader;

	std::unique_ptr<QOpenGLBuffer> vbo = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
	std::unique_ptr<QOpenGLBuffer> ibo = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::IndexBuffer);
	std::unique_ptr<QOpenGLVertexArrayObject> vao = std::make_unique<QOpenGLVertexArrayObject>();
	uint32_t indexCount, patchSize;
	GLenum        primMode;
};

class OGLViewer : public QOpenGLWidget, public QtGLFunctions
{
	Q_OBJECT
public:
	OGLViewer(QWidget *parent = nullptr);
	~OGLViewer();

	void renderPixels();

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;

private:
	void createRenderObject(const RenderBufferTrait &trait);

	void bindReslotionGate();
	void saveFrameBuffer();

protected:
	std::shared_ptr<ProjectiveCamera> view_cam;
	RenderBuffer* mRenderBuffer;
private:
	int lastMousePos[2];

	std::unique_ptr<Scene> mScene;

	std::vector<RenderBufferObject> mRBOs;

	std::unique_ptr<ResolutionGate> mResGate;

	std::shared_ptr<QOpenGLShaderProgram> triShader, quadShader, curveShader;

	friend class MainWindow;
	friend class ImageViewer;
};

}
