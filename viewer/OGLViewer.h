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
#include "Geometry/DifferentialGeometry.h"

#include "Camera/PerspectiveCamera.h"
#include "Tracer/renderBuffer.h"

// OpenGL Window in Qt
namespace Kaguya
{

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
    void bindMesh();
    void bindReslotionGate();
    void saveFrameBuffer();

    void renderpixels();
protected:
    std::shared_ptr<PerspectiveCamera> view_cam;
    renderBuffer* pixmap;
private:
    int lastMousePos[2];
    int selectMode;
private: // OpenGL variables
    int display_mode = 0;

    std::unique_ptr<Scene> mScene;
    std::vector<Primitive*> objlist;

    GLuint model_vert_vbo, model_ibo, model_vao;

    std::vector<GLfloat> filmgate, resgate;
    GLuint resgate_vbo, resgate_vao;

    std::unique_ptr<GLSLProgram> model_shader;
    std::unique_ptr<GLSLProgram> gate_shader;// OpenGL model_shader program

    friend class MainWindow;
    friend class ImageViewer;
};

}
