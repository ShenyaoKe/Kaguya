#pragma once

#include "GL/glew.h"
#include "common.h"

#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QString>
#include <QFileDialog>

#include "OpenGL_Utils/GLSLProgram.h"

#include "Math/MathUtil.h"
#include "Image/ImageData.h"
#include "Core/Scene.h"
#include "Geometry/Intersection.h"

#include "Camera/PerspectiveCamera.h"
#include "Tracer/RenderBuffer.h"

// OpenGL Window in Qt
namespace Kaguya
{

struct RenderBufferObject
{
	std::shared_ptr<GLSLProgram>  shader;

	GLuint vbo, ibo, vao;
	uint32_t indexCount, patchSize;
	// Texture buffer object
	union
	{
		GLuint tbo[2]{};
		struct { GLuint texTBO, normTBO; };
	};
	// Texture handle for texture coordinates and shading normal
	union
	{
		GLuint tex[2]{};
		struct { GLuint texTEX, normTEX; };
	};
	AttributeType texAttriType = AttributeType::UNDEFINED;
	AttributeType normAttriType = AttributeType::UNDEFINED;
	GLenum        primMode;
};

class OGLViewer : public QOpenGLWidget
{
	Q_OBJECT
public:
	enum Select_Mode
	{
		OBJECT_SELECT,
		COMPONENT_SELECT,
		FACE_COMPONENT_SELECT
	};
	OGLViewer(QWidget *parent = nullptr);
	~OGLViewer();

protected:
	void initializeGL() Q_DECL_OVERRIDE;
	void paintGL() Q_DECL_OVERRIDE;
	void resizeGL(int w, int h) Q_DECL_OVERRIDE;

	void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent *e) Q_DECL_OVERRIDE;
private:
	RenderBufferObject createRenderObject(const RenderBufferTrait &trait);

	void bindReslotionGate();
	void saveFrameBuffer();

	void renderpixels();
protected:
	std::shared_ptr<ProjectiveCamera> view_cam;
	RenderBuffer* mRenderBuffer;
private:
	int lastMousePos[2];
	int selectMode;
private: // OpenGL variables
	int display_mode = 0;

	std::unique_ptr<Scene> mScene;

	std::vector<RenderBufferObject>    mRBOs;
	std::vector<GLfloat> filmgate, resgate;
	GLuint resgate_vbo, resgate_vao;

	std::shared_ptr<GLSLProgram> triShader, quadShader, curveShader;
	std::unique_ptr<GLSLProgram> gate_shader;// OpenGL model_shader program

	friend class MainWindow;
	friend class ImageViewer;
};

}
