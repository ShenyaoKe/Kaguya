#pragma once

#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLBuffer>
#include <QtGui/QOpenGLVertexArrayObject>

#include "QtOpenGLFunctions.h"

class ResolutionGate
{
public:
	ResolutionGate(uint32_t width, uint32_t height);

	void initGL(QtGLFunctions &functions);

	void drawGL(QtGLFunctions &functions, const float rasterToScreen[4][4]);

private:
	uint32_t mWidth;
	uint32_t mHeight;

	QOpenGLBuffer mVBO = QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	QOpenGLVertexArrayObject mVAO;

	QOpenGLShaderProgram mShader;// OpenGL model_shader program

};
