#pragma once
#ifndef __OGLVIEWER__
#define __OGLVIEWER__

#include "GL/glew.h"
#include "common.h"

#include <QOpenGLWidget>
#include <QKeyEvent>
#include <QString>
#include <QFileDialog>

#include "OpenGL_Utils/GLSLProgram.h"

#include "Math/MathUtil.h"
#include "Image/ImageData.h"
#include "Geometry/Mesh.h"
#include "Geometry/DifferentialGeometry.h"
#include "Accel/KdTreeAccel.h"
#include "Camera/perspCamera.h"

static int model_mat_loc;// Uniform matrix location
static GLfloat model_mat[16];// Uniform matrix buffer
static int view_mat_loc;// Uniform matrix location
static GLfloat view_mat[16];
static int proj_mat_loc;// Porjection matrix location
static GLfloat proj_mat[16];
static GLfloat rast_mat[16];

// OpenGL Window in Qt
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
	void paintEvent(QPaintEvent *e) Q_DECL_OVERRIDE;

	void keyPressEvent(QKeyEvent *e) Q_DECL_OVERRIDE;
	void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
private:
	
	void bindBox();
	void bindMesh();
	void bindReslotionGate();
	void saveFrameBuffer();

	void renderpixels();
protected:
	perspCamera* view_cam;
	vector<uint8_t> pixmap;
private:

	int m_lastMousePos[2];
	int m_selectMode;
private: // OpenGL variables
	int display_mode = 0;

	Mesh *box_mesh;// Display object
	vector<GLfloat> box_verts;// vertices vbo
	vector<GLfloat> box_uvs;// Texture coordinates vbo
	vector<GLfloat> box_norms;// Normal coordinates vbo
	GLuint box_vert_vbo, box_norm_vbo, box_vao;

	Mesh *model_mesh;
	vector<Shape*> objlist;
	KdTreeAccel* tree;
	vector<GLfloat> model_verts;// vertices vbo
	vector<GLfloat> model_uvs;// Texture coordinates vbo
	vector<GLfloat> model_norms;// Normal coordinates vbo
	GLuint model_vert_vbo, model_norm_vbo, model_uv_vbo, model_vao;

	vector<GLfloat> filmgate, resgate;
	GLuint resgate_vbo, resgate_vao;

	GLSLProgram* model_shader;
	GLSLProgram* gate_shader;// OpenGL model_shader program

	friend class MainWindow;
};

#endif // __OGLVIEWER__