/*
 * scenewindow.cpp
 *
 *  Created on: 2014-6-5
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
*/


#include "scenewindow.h"

SceneWindow* g_sceneWindow = NULL;

const char* const s_vshScene = SHADER_STRING
(
attribute vec3 vPosition;

void main()
{
	gl_Position = vec4(vPosition, 1.0);
}
);

const char* const s_fshSceneNormal = SHADER_STRING_PRECISION_M
(

void main()
{
	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
);

const char* const s_fshSceneMesh = SHADER_STRING_PRECISION_M
(

void main()
{
	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
);

SceneWindow::SceneWindow(QWidget* parent) : QGLWidget(parent), m_programDrawNormal(NULL), m_programDrawMesh(NULL), m_ground(NULL), m_bIsMouseDown(false), m_lastX(0), m_lastY(0), m_farAway(100.0f), m_headUp(0.0f)
{
	if(g_sceneWindow != NULL)
	{
		LOG_ERROR("Invalid Usage!\n");
	}
	g_sceneWindow = this;
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);
	setAutoBufferSwap(false);

	m_v2Position = HTAlgorithm::Vec2f(0.0f, 0.0f);
	m_v2Direction = HTAlgorithm::Vec2f(0.0f, m_farAway);

	m_m4Projection.loadIdentity();
	updateModelView();
	grabKeyboard();
}

SceneWindow::~SceneWindow()
{
	delete m_programDrawNormal;
	delete m_programDrawMesh;
	delete m_ground;
}

void SceneWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	HTAlgorithm::Mat4 qmat = m_m4Projection * m_m4ModelView;

	m_ground->drawGround(qmat);
	m_ground->drawGroundWithMesh(qmat);

	swapBuffers();
}

void SceneWindow::initializeGL()
{
	makeCurrent();

#if !defined(QT_OPENGL_ES_2)
    g_glFunctions = context()->functions();
#endif

	m_programDrawNormal = new WYQOpenGLShaderProgram;

	if(!(m_programDrawNormal->addShaderFromSourceCode(QOpenGLShader::Vertex, s_vshScene) &&
		m_programDrawNormal->addShaderFromSourceCode(QOpenGLShader::Fragment, s_fshSceneNormal) &&
		m_programDrawNormal->link()))
	{
		LOG_ERROR("Program link failed!\n");
	}

	m_programDrawMesh = new WYQOpenGLShaderProgram;

	if(!(m_programDrawMesh->addShaderFromSourceCode(QOpenGLShader::Vertex, s_vshScene) &&
		m_programDrawMesh->addShaderFromSourceCode(QOpenGLShader::Fragment, s_fshSceneMesh) &&
		m_programDrawMesh->link()))
	{
		LOG_ERROR("Program link failed!\n");
	}

	glClearColor(0.2f, 0.2f, 0.1f, 1.0f);

	m_ground = new Ground;
	m_ground->initWithStage(g_stage1, g_stage1Width, g_stage1Height);
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), SLOT(updateGL()));
	timer->start(20);
}

void SceneWindow::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	initPerspective(w, h);
}

void SceneWindow::mousePressEvent(QMouseEvent *e)
{
	m_bIsMouseDown = true;
	m_lastX = e->x();
	m_lastY = e->y();
}

void SceneWindow::mouseDoubleClickEvent(QMouseEvent *e)
{

}

void SceneWindow::mouseMoveEvent(QMouseEvent *e)
{
	if(!m_bIsMouseDown)
		return;
	
	using namespace HTAlgorithm;

	//m_modelView.rotateZ((m_lastX - e->x()) / 180.0f);

	m_v2Direction = Mat2::makeRotation((e->x() - m_lastX) / 180.0f) * m_v2Direction;

	m_headUp += (e->y() - m_lastY);// * 100.0f;

	if(m_headUp < -m_farAway / 2.0f)
		m_headUp = -m_farAway / 2.0f;
	else if(m_headUp > m_farAway)
		m_headUp = m_farAway;

	m_lastX = e->x();
	m_lastY = e->y();

	updateModelView();
}

void SceneWindow::mouseReleaseEvent(QMouseEvent *e)
{

}

void SceneWindow::keyPressEvent(QKeyEvent *e)
{
	using namespace HTAlgorithm;

	float motion = 0.1f;

	switch (e->key())
	{
	case Qt::Key_Left: case Qt::Key_A:
		goLeft(motion);
		break;
	case Qt::Key_Right: case Qt::Key_D:
		goRight(motion);
		break;
	case Qt::Key_Up: case Qt::Key_W:
		goForward(motion);
		break;
	case Qt::Key_Down: case Qt::Key_S:
		goBack(motion);
		break;
	default:
		return;
	}

	updateModelView();
}

void SceneWindow::keyReleaseEvent(QKeyEvent *)
{

}

GLuint SceneWindow::genTextureWithBuffer(const void* bufferData, GLint w, GLint h, GLenum channelFmt, GLenum dataFmt)
{
	GLuint tex;
    glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, channelFmt, dataFmt, bufferData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return tex;
}

void SceneWindow::initOrtho(int w, int h)
{
	m_m4Projection = HTAlgorithm::Mat4::makeOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, -1.0);
}

void SceneWindow::initPerspective(int w, int h)
{	
	float aspectRatio = w / float(h);
	float z = HT_MIN(width(), height());
	m_m4Projection = HTAlgorithm::Mat4::makePerspective(M_PI / 3.0f, aspectRatio, .1f, 10000.0f);
}

void SceneWindow::updateModelView()
{
	using namespace HTAlgorithm;
	const Vec2f v2Dir = m_v2Position + m_v2Direction;
	const float len = v2Dir.length();
	const Vec2f v2DirBack = v2Dir * (- m_headUp / len);
	
	m_m4ModelView = HTAlgorithm::Mat4::makeLookAt(m_v2Position[0], m_v2Position[1], 0.1f, v2Dir[0], v2Dir[1], m_headUp, v2DirBack[0], v2DirBack[1], len);
	
}

void SceneWindow::goForward(float dis)
{
	HTAlgorithm::Vec2f tmp(m_v2Direction);
	m_v2Position += tmp.normalize() * dis;
}

void SceneWindow::goBack(float dis)
{
	HTAlgorithm::Vec2f tmp(m_v2Direction);
	m_v2Position -= tmp.normalize() * dis;
}

void SceneWindow::goLeft(float dis)
{
	HTAlgorithm::Vec2f tmp(m_v2Direction);
	tmp.normalize();
	m_v2Position += HTAlgorithm::Vec2f(-tmp[1], tmp[0]) * dis;
}

void SceneWindow::goRight(float dis)
{
	HTAlgorithm::Vec2f tmp(m_v2Direction);
	tmp.normalize();
	m_v2Position += HTAlgorithm::Vec2f(tmp[1], -tmp[0]) * dis;
}
