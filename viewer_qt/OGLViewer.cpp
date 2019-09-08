#include "OGLViewer.h"

#include "Core/ProgressBar.h"
#include "Geometry/Mesh.h"
#include "IO/ObjLoader.h"
#include "IO/SceneLoader.h"
#include <embree3/rtcore_ray.h>
#include <QImage>

namespace Kaguya
{

OGLViewer::OGLViewer(QWidget* parent)
	: QOpenGLWidget(parent)
	, mRenderBuffer(new RenderBuffer(default_resX, default_resY))
	, mScene(SceneLoader::load("../../scene/unitest_scene.json"))
{
	// Set surface format for current widget
	QSurfaceFormat surfaceFormat = format();
	surfaceFormat.setDepthBufferSize(32);
	surfaceFormat.setStencilBufferSize(8);
	// To enable anti-aliasing, set sample to sampleNumber^2
	//format.setSamples(9);
	surfaceFormat.setVersion(OGL_MAJOR_VER, OGL_MINOR_VER);
	surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
	setFormat(surfaceFormat);
	QSurfaceFormat::setDefaultFormat(surfaceFormat);

	if (mScene)
	{
		// Get projective camera
		view_cam = std::static_pointer_cast<ProjectiveCamera>(mScene->getCamera());
		// Commit scene
		mScene->commitScene();
	}
}

OGLViewer::~OGLViewer()
{
}

/************************************************************************/
/* OpenGL Rendering Modules                                             */
/************************************************************************/
void OGLViewer::initializeGL()
{
	// OpenGL extension initialization
	initializeOpenGLFunctions();

	// Print OpenGL version
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "OpenGL version supported " << glGetString(GL_VERSION) << std::endl;

	// Enable OpenGL features
	glEnable(GL_MULTISAMPLE);
	//glEnable(GL_LINE_SMOOTH);
	//glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glFrontFace(GL_CCW); // set counter-clock-wise vertex order to mean the front

	//////////////////////////////////////////////////////////////////////////

	// Create model_shader files
	const QString shaderLocation = "../../resources/shaders/";
	triShader = std::make_shared<QOpenGLShaderProgram>();
	triShader->addShaderFromSourceFile(QOpenGLShader::Vertex, shaderLocation + "TriangleMesh_vs.glsl");
	triShader->addShaderFromSourceFile(QOpenGLShader::Fragment, shaderLocation + "TriangleMesh_fs.glsl");
	triShader->addShaderFromSourceFile(QOpenGLShader::Geometry, shaderLocation + "TriangleMesh_gs.glsl");
	triShader->link();

	quadShader = std::make_shared<QOpenGLShaderProgram>();
	quadShader->addShaderFromSourceFile(QOpenGLShader::Vertex, shaderLocation + "QuadMesh_vs.glsl");
	quadShader->addShaderFromSourceFile(QOpenGLShader::Fragment, shaderLocation + "QuadMesh_fs.glsl");
	quadShader->addShaderFromSourceFile(QOpenGLShader::Geometry, shaderLocation + "QuadMesh_gs.glsl");
	quadShader->link();

	curveShader = std::make_shared<QOpenGLShaderProgram>();
	curveShader->addShaderFromSourceFile(QOpenGLShader::Vertex, shaderLocation + "Curve_vs.glsl");
	curveShader->addShaderFromSourceFile(QOpenGLShader::Fragment, shaderLocation + "Curve_fs.glsl");
	curveShader->addShaderFromSourceFile(QOpenGLShader::Geometry, shaderLocation + "Curve_gs.glsl");
	curveShader->link();

	if (mScene)
	{
		for (size_t i = 0; i < mScene->getPrimitiveCount(); i++)
		{
			createRenderObject(mScene->getRenderBuffer(i));
		}
	}

	bindReslotionGate();
}

void OGLViewer::createRenderObject(const RenderBufferTrait &trait)
{
	mRBOs.emplace_back();
	RenderBufferObject& ret = mRBOs.back();

	// Create VAO
	ret.vao->create();
	ret.vao->bind();

	// VBO
	ret.vbo->create();
	ret.vbo->bind();
	ret.vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
	ret.vbo->allocate(trait.vertex.data, trait.vertex.size);
	// IBO
	ret.ibo->create();
	ret.ibo->bind();
	ret.ibo->setUsagePattern(QOpenGLBuffer::StaticDraw);
	ret.ibo->allocate(trait.index.data, trait.index.size);
	ret.indexCount = trait.index.count;

	// Attach VBO and IBO to VAO
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, trait.vertex.stride, (void*)trait.vertex.offset);
		glEnableVertexAttribArray(0);
	}
	ret.vao->release();

	switch (trait.renderType)
	{
	case GPURenderType::CURVE:
	{
		ret.primMode = GL_PATCHES;
		break;
	}
	case GPURenderType::TRIANGLE:
	{
		ret.primMode = GL_TRIANGLES;
		ret.shader = triShader;
		ret.patchSize = 3;
		break;
	}
	case GPURenderType::QUAD:
	{
		ret.primMode = GL_LINES_ADJACENCY;
		ret.shader = quadShader;
		ret.patchSize = 4;
		break;
	}
	default:
		break;
	}
}

void OGLViewer::bindReslotionGate()
{
	const GLfloat filmWidth = view_cam->getFilm().width;
	const GLfloat filmHeight = view_cam->getFilm().height;
	printf("Film of (%f, %f)\n", filmWidth, filmHeight);

	mResGate = std::make_unique<ResolutionGate>(filmWidth, filmHeight);
	mResGate->initGL(*this);
}

void OGLViewer::paintGL()
{
	printf("in paint loop\n");
	// Make current window
	makeCurrent();
	glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
	// Clear background and color buffer
	glClearColor(1, 0., 0., 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#if 1
	//////////////////////////////////////////////////////////////////////////
	// Model
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	for (auto &rbo : mRBOs)
	{
		rbo.vao->bind();
		rbo.shader->bind();
		// Apply uniform matrix
		rbo.shader->setUniformValue("view_matrix", view_cam->CameraToWorld.getInvMat().mtx);// View Matrix
		rbo.shader->setUniformValue("proj_matrix", view_cam->CameraToScreen.getMat().mtx);// Projection
		glDrawElements(rbo.primMode, rbo.indexCount, GL_UNSIGNED_INT, 0);
		rbo.vao->release();
		rbo.shader->release();
	}

#else
	////////////////////////////////////////////

	std::cout << "Screen point: "
			  << view_cam->RasterToScreen(Point3f(0, 0, 0)) << " to "
			  << view_cam->RasterToScreen(Point3f(640, 400, 0)) << std::endl;
	mResGate->drawGL(*this, view_cam->RasterToScreen.getMat().mtx);
#endif
	doneCurrent();
}
// Resize function
void OGLViewer::resizeGL(int w, int h)
{
	// Widget resize operations
	view_cam->resizeViewport(width() / static_cast<Float>(height()));
}

/************************************************************************/
/* Application Functions                                                */
/************************************************************************/

void OGLViewer::saveFrameBuffer()
{
	QString filename = QFileDialog::getSaveFileName(
		this, "Save Screen shot file...", "default", tr("PNG(*.png)"));
	grab().save(filename);
}

void OGLViewer::renderPixels()
{
	makeCurrent();

	clock_t startT, endT;
	startT = clock();
	ConsoleProgressBar progBar;

	int index = 0;
	Ray traceRay;
	CameraSample camsmp;
	Transform w2o;

	QImage retImg(default_resX, default_resY, QImage::Format_ARGB32);

	Point3f lightpos(3, 10, 1);
	Float cosVal;
	Intersection isec;
	mRenderBuffer->cleanBuffer();
	for (int j = 0; j < default_resY; j++)
	{
		for (int i = 0; i < default_resX; i++)
		{
			camsmp.mFilm = Point2f(i, j);

			view_cam->generateRay(camsmp, &traceRay);

			cosVal = 0;
			Point2f n(0, 0);
			if (mScene->intersect(traceRay, &isec))
			{
				/*cosVal = tHit;
				isec.shape->postIntersect(traceRay, &isec);
				cosVal = (dot(normalize(lightpos - isec.P), isec.Ng) + 1) * 0.5;
				pixmap->setBuffer(uint32_t(i), uint32_t(j), isec, tHit);
				size_t index = (j * width() + i) << 2;
				pixmap->beauty[index++] = cosVal;
				pixmap->beauty[index++] = cosVal;
				pixmap->beauty[index++] = cosVal;
				pixmap->beauty[index++] = cosVal;

				pixmap->setBuffer()
				pixmap->empty();
				pixmap->setBuffer(0);
				pixmap->doSomething();
				*/
				mRenderBuffer->setBuffer(i, j, isec, traceRay.tMax);
				traceRay.Ng.normalize();
				int rgb[]{ static_cast<int>((traceRay.Ng.x*0.5f + 0.5f) * 255),
					static_cast<int>((traceRay.Ng.y*0.5f + 0.5f) * 255),
					static_cast<int>((traceRay.Ng.z*0.5f + 0.5f) * 255) };
				retImg.setPixelColor(i, default_resY - j - 1,
									 QColor(rgb[0], rgb[1], rgb[2]));

			}
		}
		progBar.print(j / float(default_resY));
	}
	progBar.complete();

	retImg.save("result.png");

	endT = clock();
	std::cout << "Rendering Time:\t" << (Float)(endT - startT) / CLOCKS_PER_SEC << "s" << std::endl;//Timer
	doneCurrent();
}

}
