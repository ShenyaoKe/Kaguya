#include "OGLViewer.h"
#include "Geometry/Sphere.h"

OGLViewer::OGLViewer(QWidget *parent)
	: QOpenGLWidget(parent)
	, selectMode(OBJECT_SELECT)
	, view_cam(new perspCamera(
		Point3f(10, 6, 10), Point3f(0, 0, 0), Vector3f(0, 1, 0),
		width() / static_cast<double>(height())))
	, pixmap(new renderBuffer(default_resX, default_resY))
	, resgate{	0, 0,
				640, 0,
				640, 480,
				0, 480 }
{
	// Set surface format for current widget
	QSurfaceFormat format;
	format.setDepthBufferSize(32);
	format.setStencilBufferSize(8);
	//format.setSamples(16);
	format.setVersion(4, 5);
	//format.setProfile(QSurfaceFormat::CoreProfile);
	this->setFormat(format);

	Float assetsssss;
	// Read obj file
	//box_mesh = new Mesh("scene/obj/cube_large.obj");
	model_mesh = make_unique<TriangleMesh>("scene/obj/monkey.obj");
	
	model_mesh->refine(objlist);
	tree = make_unique<KdTreeAccel>(objlist);

	// Export View, Projection, Rasterization Matrices
	view_cam->exportVBO(cam_mat, cam_mat + 16, cam_mat + 32);
}

OGLViewer::~OGLViewer()
{
}
/************************************************************************/
/* OpenGL Rendering Modules                                             */
/************************************************************************/
void OGLViewer::initializeGL()
{
	// OpenGL extention initialization
	glewInit();

	// Print OpenGL vertion
	cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
	cout << "OpenGL version supported " << glGetString(GL_VERSION) << endl;

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
//#ifdef _DEBUG
	model_shader.reset(new GLSLProgram("resources/shaders/model_vs.glsl", "resources/shaders/model_fs.glsl", "resources/shaders/model_gs.glsl"));
	gate_shader.reset(new GLSLProgram("resources/shaders/gate_vs.glsl", "resources/shaders/gate_fs.glsl"));
/*
#else
	model_shader.reset(new GLSLProgram(":shaders/model_vs.glsl", ":shaders/model_fs.glsl", ":shaders/model_gs.glsl"));
	gate_shader.reset(new GLSLProgram(":shaders/gate_vs.glsl", ":shaders/gate_fs.glsl"));
#endif*/

	// Export vbo for shaders
	//box_mesh->exportVBO(&box_verts, &box_uvs, &box_norms);
	model_mesh->exportIndexedVBO(&model_verts, nullptr, nullptr, &model_ids);

	//bindBox();
	bindMesh();
	bindReslotionGate();

	// Get uniform variable location
	model_mat_loc = model_shader->getUniformLocation("model_matrix");
	view_mat_loc = model_shader->getUniformLocation("view_matrix");
	proj_mat_loc = model_shader->getUniformLocation("proj_matrix");; // WorldToCamera matrix

	gate_shader->add_uniformv("transform");
}


void OGLViewer::bindBox()
{
	glDeleteBuffers(1, &box_vert_vbo);
	glDeleteBuffers(1, &box_norm_vbo);
	glDeleteVertexArrays(1, &box_vao);
	// Bind VAO
	glGenVertexArrays(1, &box_vao);
	glBindVertexArray(box_vao);

	//GLuint box_vert_vbo;
	glGenBuffers(1, &box_vert_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, box_vert_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * box_verts.size(), &box_verts[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(0);

	// Bind normal value as color
	//GLuint box_norm_vbo;
	glGenBuffers(1, &box_norm_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, box_norm_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * box_norms.size(), &box_norms[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OGLViewer::bindMesh()
{
	glDeleteBuffers(1, &model_vert_vbo);
	glDeleteBuffers(1, &model_ibo);
	glDeleteVertexArrays(1, &model_vao);

	// VBO
	glCreateBuffers(1, &model_vert_vbo);
	glNamedBufferData(model_vert_vbo, sizeof(GLfloat) * model_verts.size(), &model_verts[0], GL_STATIC_DRAW);
	// IBO
	glCreateBuffers(1, &model_ibo);
	glNamedBufferData(model_ibo, sizeof(GLuint) * model_ids.size(), &model_ids[0], GL_STATIC_DRAW);

	// Bind VAO
	glCreateVertexArrays(1, &model_vao);
	glEnableVertexArrayAttrib(model_vao, 0);

	glVertexArrayAttribFormat(model_vao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayVertexBuffer(model_vao, 0, model_vert_vbo, 0, sizeof(GLfloat) * 3);
	glVertexArrayAttribBinding(model_vao, 0, 0);
	glVertexArrayElementBuffer(model_vao, model_ibo);

	// Bind normal value as color

	// Bind UV values
	
	/*glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/
}

void OGLViewer::bindReslotionGate()
{
	// DSA
	// Create VAO
	glCreateBuffers(1, &resgate_vbo);
	glNamedBufferData(resgate_vbo, sizeof(GLfloat) * resgate.size(), &resgate[0], GL_STATIC_DRAW);

	// VAO
	glCreateVertexArrays(1, &resgate_vao);
	glEnableVertexArrayAttrib(resgate_vao, 0);

	// Setup the formats
	glVertexArrayAttribFormat(resgate_vao, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayVertexBuffer(resgate_vao, 0, resgate_vbo, 0, sizeof(float) * 2);
	glVertexArrayAttribBinding(resgate_vao, 0, 0);
}

void OGLViewer::paintGL()
{
	// Make curent window
	makeCurrent();
	glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
	// Clear background and color buffer
	glClearColor(0.6, 0.6, 0.6, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//Draw box
	/*glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(1.0);
	glBindVertexArray(box_vao);
	model_shader->use_program();
	vector<GLfloat> projmatvec(proj_mat, proj_mat + 16);
	glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, view_mat);
	glUniformMatrix4fv(proj_mat_loc, 1, GL_FALSE, &projmatvec[0]);
	glDrawArrays(GL_TRIANGLES, 0, box_verts.size() / 3);*/

	//////////////////////////////////////////////////////////////////////////
	// Model
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK); // cull back face
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBindVertexArray(model_vao);
	model_shader->use_program();

	// Apply uniform matrix
	glUniformMatrix4fv(view_mat_loc, 1, GL_FALSE, cam_mat);// View Matrix
	glUniformMatrix4fv(proj_mat_loc, 1, GL_FALSE, cam_mat + 16);// Projection

	glDrawElements(GL_TRIANGLES, model_ids.size(), GL_UNSIGNED_INT, 0);
	////////////////////////////////////////////
	glBindVertexArray(resgate_vao);
	gate_shader->use_program();
	//glUniformMatrix4fv((*gate_shader)("transform"), 1, GL_FALSE, rast_mat);
	glUniformMatrix4fv((*gate_shader)("transform"), 1, GL_FALSE, cam_mat + 32);
	glLineWidth(2.0);
	glDrawArrays(GL_LINE_LOOP, 0, resgate.size() / 2);
	
	glBindVertexArray(0);
	gate_shader->unuse();

	doneCurrent();
}
// Resize function
void OGLViewer::resizeGL(int w, int h)
{
	// Widget resize operations
	view_cam->resizeViewport(width() / static_cast<double>(height()));
	//view_cam->exportVBO(view_mat, proj_mat, rast_mat);
	view_cam->exportVBO(cam_mat, cam_mat + 16, cam_mat + 32);
}
/************************************************************************/
/* Qt User Operation Functions                                          */
/************************************************************************/

void OGLViewer::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Home)
	{
		//initParas();
	}
	// Save frame buffer
	else if (e->key() == Qt::Key_P && e->modifiers() == Qt::ControlModifier)
	{
		this->saveFrameBuffer();
	}
	// Render
	else if (e->key() == Qt::Key_R && e->modifiers() == Qt::ControlModifier)
	{
		this->renderpixels();
	}
	else
	{
		QOpenGLWidget::keyPressEvent(e);
	}
	update();
}

void OGLViewer::mousePressEvent(QMouseEvent *e)
{
	lastMousePos[0] = e->x();
	lastMousePos[1] = e->y();
	if (e->buttons() == Qt::LeftButton && e->modifiers() == Qt::AltModifier)
	{
		// Do something here
	}
};

void OGLViewer::mouseReleaseEvent(QMouseEvent *e)
{
	lastMousePos[0] = e->x();
	lastMousePos[1] = e->y();
}

void OGLViewer::mouseMoveEvent(QMouseEvent *e)
{
	int dx = e->x() - lastMousePos[0];
	int dy = e->y() - lastMousePos[1];

	//printf("dx: %d, dy: %d\n", dx, dy);

	if (e->buttons() == Qt::LeftButton && e->modifiers() == Qt::AltModifier)
	{
		view_cam->rotate(dy * 0.25, -dx * 0.25, 0.0);
		//view_cam->exportVBO(view_mat, nullptr, nullptr);
		view_cam->exportVBO(cam_mat, nullptr, nullptr);
		update();
	}
	else if (e->buttons() == Qt::RightButton && e->modifiers() == Qt::AltModifier)
	{
		if (dx != e->x() && dy != e->y())
		{
			view_cam->zoom(0.0, 0.0, dx * 0.05);
			//view_cam->exportVBO(view_mat, nullptr, nullptr);
			view_cam->exportVBO(cam_mat, nullptr, nullptr);
			update();
		}
	}
	else if (e->buttons() == Qt::MidButton && e->modifiers() == Qt::AltModifier)
	{
		if (dx != e->x() && dy != e->y())
		{
			view_cam->zoom(-dx * 0.05, dy * 0.05, 0.0);
			//view_cam->exportVBO(view_mat, nullptr, nullptr);
			view_cam->exportVBO(cam_mat, nullptr, nullptr);
			update();
		}
	}
	else
	{
		QOpenGLWidget::mouseMoveEvent(e);
	}

	lastMousePos[0] = e->x();
	lastMousePos[1] = e->y();
}

void OGLViewer::wheelEvent(QWheelEvent *e)
{
	view_cam->zoom(0.0, 0.0, -e->delta() * 0.01);
	//view_cam->exportVBO(view_mat, nullptr, nullptr);
	view_cam->exportVBO(cam_mat, nullptr, nullptr);
	update();
}

/************************************************************************/
/* Application Functions                                                */
/************************************************************************/

void OGLViewer::saveFrameBuffer()
{
	QString filename = QFileDialog::getSaveFileName(
		this, "Save Screenshot file...", "default", tr("PNG(*.png)"));
	this->grab().save(filename);
}

void OGLViewer::renderpixels()
{
	makeCurrent();
	clock_t startT, endT;
	startT = clock();
	int index = 0;
	Ray traceRay;
	cameraSampler camsmp;
	Transform w2o;
	geoSphere sphere(&w2o, &w2o);
	Point3f lightpos(3, 10, 1);
	Float cosVal;
	DifferentialGeometry* queryPoint = new DifferentialGeometry;
	for (int j = 0; j < default_resY; j++)
	{
		for (int i = 0; i < default_resX; i++)
		{
			camsmp.imgX = i;
			camsmp.imgY = j;

			view_cam->generateRay(camsmp, &traceRay);
			Float tHit(INFINITY), rayEp(0);

			cosVal = 0;
			Point2f n(0, 0);
			//if (sphere.intersect(traceRay, queryPoint, &tHit, &rayEp))
			if (tree->intersect(traceRay, queryPoint, &tHit, &rayEp))
			{
				//cout << "hit something\n" << endl;
				cosVal = tHit;
				queryPoint->shape->postIntersect(traceRay, queryPoint);
				//cosVal = (Dot(Normalize(lightpos - queryPoint->pos), queryPoint->norm) + 1) * 0.5;
				pixmap->setBuffer(uint32_t(i), uint32_t(j), *queryPoint, tHit);
				//pixmap->setBuffer()
				//pixmap->empty();
				//pixmap->setBuffer(0);
				//pixmap->doSomething();
			}
		}
	}
	delete queryPoint;

	endT = clock();
	cout << "Rendering Time:\t" << (Float)(endT - startT) / CLOCKS_PER_SEC << "s" << endl;//Timer
	doneCurrent();
}
