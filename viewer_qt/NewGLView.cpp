#include "NewGLView.h"

using namespace Kaguya;

NewGLView::NewGLView(QWidget* parent)
	: QOpenGLWidget(parent)
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
}

NewGLView::~NewGLView()
{
}

/************************************************************************/
/* OpenGL Rendering Modules                                             */
/************************************************************************/
void NewGLView::initializeGL()
{
	// OpenGL extension initialization
	bool funcsInitOK = initializeOpenGLFunctions();
	std::cout << "OpenGL Functions Initialized OK: " << funcsInitOK << std::endl;

	// Get the surface format from the current context, and print the
	// major and minor version actually received, along with the profile.
	QSurfaceFormat sf = QOpenGLContext::currentContext()->format();
	std::cout << "OpenGL Version: " << sf.majorVersion() << "."
		 << sf.minorVersion() << std::endl;
	std::cout << "OpenGL Profile: ";
	switch(sf.profile())
	{
	case QSurfaceFormat::NoProfile:
		std::cout << "None";
		break;
	case QSurfaceFormat::CoreProfile:
		std::cout << "Core";
		break;
	case QSurfaceFormat::CompatibilityProfile:
		std::cout << "Compatibility";
		break;
	default:
		std::cout << "invalid option";
		break;
	}
	std::cout << std::endl;
	glFrontFace(GL_CCW); // set counter-clock-wise vertex order to mean the front

	bindReslotionGate();
}

void NewGLView::paintGL()
{
	// Make current window
	makeCurrent();
	// Clear background and color buffer
	glClearColor(1.0, 0.6, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	mResGate->drawGL(*this, mViewCamera->RasterToScreen.getMat().mtx);

	doneCurrent();
}

void NewGLView::bindReslotionGate()
{
	mResGate = std::make_unique<ResolutionGate>(640, 400);
	mResGate->initGL(*this);

	mViewCamera = std::make_unique<Kaguya::PerspectiveCamera>();
	auto &film = mViewCamera->getFilm();
	film.setResolution(640, 480);
	mViewCamera->updateRasterToScreen();
	mViewCamera->updateRasterToCam();


	std::cout << "Screen point: "
			  << mViewCamera->RasterToScreen(Point3f(0, 400, 0)) << " to "
			  << mViewCamera->RasterToScreen(Point3f(640, 400, 0)) << std::endl;
}
