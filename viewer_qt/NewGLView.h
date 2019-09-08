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

class NewGLView : public QOpenGLWidget, public QtGLFunctions
{
Q_OBJECT
public:
	NewGLView(QWidget *parent = nullptr);
	~NewGLView();

protected:
	void initializeGL() override;
	void paintGL() override;
private:
	void bindReslotionGate();

	void renderPixels() {}

private:
	std::shared_ptr<Kaguya::PerspectiveCamera> mViewCamera;

	std::unique_ptr<ResolutionGate> mResGate;

	friend class MainWindow;
};

