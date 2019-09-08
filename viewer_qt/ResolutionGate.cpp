//
// Created by Shenyao Ke on 2019-02-18.
//

#include "ResolutionGate.h"

#include <Math/Vector.h>

using namespace Kaguya;

ResolutionGate::ResolutionGate(uint32_t width, uint32_t height)
	: mWidth(width), mHeight(height)
	, mVBO(QOpenGLBuffer::VertexBuffer)
{

}

void ResolutionGate::initGL(QtGLFunctions &functions)
{
	const std::array<Point2f, 4> vertPosition{
		Point2f(0, 0), Point2f(mWidth, 0),
		Point2f(mWidth, mHeight), Point2f(0, mHeight) };

	mVAO.create();
	mVAO.bind();

	mVBO.create();
	mVBO.bind();
	mVBO.setUsagePattern(QOpenGLBuffer::StaticDraw);
	// Create VBO
	mVBO.allocate(vertPosition.data(), sizeof(float) * vertPosition.size());

	// Setup the formats
	{
		functions.glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Point2f), 0);
		functions.glEnableVertexAttribArray(0);
	}
	mVAO.release();

	// Init shaders
	{
		mShader.create();
		const QString shaderLocation = "../../resources/shaders/";
		mShader.addShaderFromSourceFile(QOpenGLShader::Vertex, shaderLocation + "gate_vs.glsl");
		mShader.addShaderFromSourceFile(QOpenGLShader::Fragment, shaderLocation + "gate_fs.glsl");
		mShader.addShaderFromSourceFile(QOpenGLShader::Geometry, shaderLocation + "gate_gs.glsl");
		mShader.link();
	}
}

void ResolutionGate::drawGL(QtGLFunctions &functions, const float rasterToScreen[4][4])
{
	mVAO.bind();
	mShader.bind();

	mShader.setUniformValue("transform", rasterToScreen);
	//functions.glUniformMatrix4fv(mShader.attributeLocation("transform"), 1, GL_FALSE, rasterToScreen);
	functions.glDrawArrays(GL_LINE_LOOP, 0, 4);

	mShader.release();
	mVAO.release();
}
