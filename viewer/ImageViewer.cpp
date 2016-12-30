#include "ImageViewer.h"
#include <QTimer>

namespace Kaguya
{

ImageViewer::ImageViewer(QWidget* parent)
    : QMainWindow(parent)
    , img_panel(new ImageViewerPanel(this))
{
    ui.setupUi(this);
    ui.img_panel->addWidget(img_panel.get());
    img_panel->setFocusPolicy(Qt::StrongFocus);

    ui.display_t->addItem("Beauty");
    ui.display_t->addItem("P");
    ui.display_t->addItem("N");
    ui.display_t->addItem("dPdu");
    ui.display_t->addItem("dPdv");

    connect(ui.display_t,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::activated),
            this, &ImageViewer::switchTexture);
}

void ImageViewer::setpixmap(const Kaguya::renderBuffer* pixmap)
{
    rbuf = pixmap;
    switchTexture();
}

void ImageViewer::switchTexture()
{
    switch (ui.display_t->currentIndex())
    {
    case 0:
        img_panel->texType = DISPLAY_TYPE::BEAUTY;
        img_panel->textures = static_cast<const void*>(rbuf->beauty.data());
        break;
    case 1:
        img_panel->texType = DISPLAY_TYPE::P;
        img_panel->textures = static_cast<const void*>(rbuf->p.data());
        break;
    case 2:
        img_panel->texType = DISPLAY_TYPE::N;
        img_panel->textures = static_cast<const void*>(rbuf->n.data());
        break;
    case 3:
        img_panel->texType = DISPLAY_TYPE::DPDU;
        img_panel->textures = static_cast<const void*>(rbuf->dpdu.data());
        break;
    case 4:
        img_panel->texType = DISPLAY_TYPE::DPDV;
        img_panel->textures = static_cast<const void*>(rbuf->dpdv.data());
        break;
    default:
        break;
    }
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
    , texType(DISPLAY_TYPE::BEAUTY)
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
}

void ImageViewerPanel::updateTexture()
{
    makeCurrent();
    if (textures != nullptr)
    {
        switch (texType)
        {
        case DISPLAY_TYPE::BEAUTY:
            glTextureStorage2D(tex, 1, GL_RGBA32F, imgsize[0], imgsize[1]);
            glTextureSubImage2D(tex, 0, 0, 0, imgsize[0], imgsize[1], GL_RGBA, GL_FLOAT, textures);
            break;
        case DISPLAY_TYPE::P:
        case DISPLAY_TYPE::N:
        case DISPLAY_TYPE::DPDU:
        case DISPLAY_TYPE::DPDV:
            glTextureStorage2D(tex, 1, GL_RGB32F, imgsize[0], imgsize[1]);
            glTextureSubImage2D(tex, 0, 0, 0, imgsize[0], imgsize[1], GL_RGB, GL_FLOAT, textures);
            break;
        default:
            break;
        }

    }
    else
    {
        glTextureStorage2D(tex, 1, GL_RGBA32F, 0, 0);
        glTextureSubImage2D(tex, 0, 0, 0, 0, 0, GL_RGBA, GL_FLOAT, 0);
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
        glCreateTextures(GL_TEXTURE_2D, 1, &tex);
        glTextureParameteri(tex, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(tex, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureStorage2D(tex, 1, GL_RGB32F, imgsize[0], imgsize[1]);
        if (texLen > 0)
        {
            glTextureSubImage2D(tex, 0, 0, 0, imgsize[0], imgsize[1], GL_RGB, GL_FLOAT, textures);
        }
        texHandle = glGetTextureHandleARB(tex);
        glMakeTextureHandleResidentARB(texHandle);


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
    if (textures)
    {
        glUniformHandleui64ARB((*shaderP)["tex"], texHandle);
    }

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    doneCurrent();
}

void ImageViewerPanel::resizeGL(int w, int h)
{

}

}
