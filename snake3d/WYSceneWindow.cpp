/*
 * WYSceneWindow.cpp
 *
 *  Created on: 2014-6-5
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
*/


#include "WYSceneWindow.h"

WYSceneWindow* g_sceneWindow = NULL;

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

WYSceneWindow::WYSceneWindow(QWidget* parent) : QGLWidget(parent), m_ground(NULL), m_bIsMouseDown(false), m_lastX(0), m_lastY(0), m_farAway(100.0f), m_headUp(0.0f), m_fovyRad(M_PI / 3.0f), m_sky(NULL),m_zHeight(1.5f)
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
    makeCurrent();
}

WYSceneWindow::~WYSceneWindow() 
{
    makeCurrent();
	delete m_ground;
}

void WYSceneWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	HTAlgorithm::Mat4 qmat = m_m4Projection * m_m4ModelView;
	
//	m_sky->drawSky(qmat);
//	m_sky->drawSkyWithMesh(qmat);

	glEnable(GL_DEPTH_TEST);
//	m_ground->drawGround(qmat);
	m_snake->drawSnake(qmat);
	glDisable(GL_DEPTH_TEST);
//	m_ground->drawGroundWithMesh(qmat);
	m_snake->drawSnakeWithMesh(qmat);

	htCheckGLError("WYSceneWindow::paintGL");
	swapBuffers();
}

void WYSceneWindow::initializeGL()
{
	makeCurrent();

#if !defined(QT_OPENGL_ES_2) && !defined(Q_OS_MAC)
    g_glFunctions = context()->functions();
#endif

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	m_ground = new WYGround;
	if(!m_ground->initWithStage(g_stage1, g_stage1Width, g_stage1Height, g_stage1GroundTextureName))
	{
		LOG_ERROR("Init Stage Failed!");
	}

	m_sky = new WYSky;
	if(!m_sky->initSky(g_skyTextureName))
	{
		LOG_ERROR("Init Sky Failed\n!");
	}

	m_snake = new WYSnake;
	if(!m_snake->init(0.0f, 0.0f, g_stage1SnakeTextureName, 10.0f))
	{
		LOG_ERROR("Init snake Failed!\n");
	}

	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), SLOT(updateGL()));
	timer->start(20);

	glEnable(GL_DEPTH_TEST);
	
	htCheckGLError("SceneWindow::initializeGL");
}

void WYSceneWindow::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	initPerspective(w, h);
}

void WYSceneWindow::mousePressEvent(QMouseEvent *e)
{
	m_bIsMouseDown = true;
	m_lastX = e->x();
	m_lastY = e->y();
}

void WYSceneWindow::mouseDoubleClickEvent(QMouseEvent *e)
{

}

void WYSceneWindow::mouseMoveEvent(QMouseEvent *e)
{
	if(!m_bIsMouseDown)
		return;
	
	using namespace HTAlgorithm;

	//m_modelView.rotateZ((m_lastX - e->x()) / 180.0f);

	m_v2Direction = Mat2::makeRotation((e->x() - m_lastX) / 180.0f) * m_v2Direction;

	m_headUp += (e->y() - m_lastY);// * 100.0f;

	if(m_headUp < -m_farAway / 2.0f)
		m_headUp = -m_farAway / 2.0f;
	else if(m_headUp > m_farAway*2.0f)
		m_headUp = m_farAway*2.0f;

	m_lastX = e->x();
	m_lastY = e->y();

	updateModelView();
}

void WYSceneWindow::mouseReleaseEvent(QMouseEvent *e)
{

}

void WYSceneWindow::keyPressEvent(QKeyEvent *e)
{
	using namespace HTAlgorithm;

	float motion = 0.2f;

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
	case Qt::Key_J:
		m_zHeight -= motion;
		break;
	case Qt::Key_K:
		m_zHeight += motion;
		break;
	default:
		return;
	}

	updateModelView();
}

void WYSceneWindow::keyReleaseEvent(QKeyEvent *)
{

}

void WYSceneWindow::wheelEvent(QWheelEvent *e)
{
	m_fovyRad += e->delta() / 2400.0f;
	if(m_fovyRad < M_PI / 10.0f) 
		m_fovyRad = M_PI / 10.0f;
	else if(m_fovyRad > M_PI / 3.0)
		m_fovyRad = M_PI / 3.0f;
	initPerspective(width(), height());
}

GLuint WYSceneWindow::genTextureWithBuffer(const void* bufferData, GLint w, GLint h, GLenum channelFmt, GLenum dataFmt)
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

void WYSceneWindow::initOrtho(int w, int h)
{
	m_m4Projection = HTAlgorithm::Mat4::makeOrtho(-1.0, 1.0, -1.0, 1.0, 1.0, -1.0);
}

void WYSceneWindow::initPerspective(int w, int h)
{	
	float aspectRatio = w / float(h);
	float z = HT_MIN(width(), height());
	m_m4Projection = HTAlgorithm::Mat4::makePerspective(m_fovyRad, aspectRatio, .1f, 10000.0f);
}

void WYSceneWindow::updateModelView()
{
	using namespace HTAlgorithm;
	const Vec2f v2Dir = m_v2Position + m_v2Direction;
	const float len = v2Dir.length();
	const Vec2f v2DirBack = v2Dir * (- m_headUp / len);
	
	m_m4ModelView = HTAlgorithm::Mat4::makeLookAt(m_v2Position[0], m_v2Position[1], m_zHeight, v2Dir[0], v2Dir[1], m_headUp, v2DirBack[0], v2DirBack[1], len);
	
}

void WYSceneWindow::goForward(float dis)
{
	HTAlgorithm::Vec2f tmp(m_v2Direction);
	m_v2Position += tmp.normalize() * dis;
}

void WYSceneWindow::goBack(float dis)
{
	HTAlgorithm::Vec2f tmp(m_v2Direction);
	m_v2Position -= tmp.normalize() * dis;
}

void WYSceneWindow::goLeft(float dis)
{
	HTAlgorithm::Vec2f tmp(m_v2Direction);
	tmp.normalize();
	m_v2Position += HTAlgorithm::Vec2f(-tmp[1], tmp[0]) * dis;
}

void WYSceneWindow::goRight(float dis)
{
	HTAlgorithm::Vec2f tmp(m_v2Direction);
	tmp.normalize();
	m_v2Position += HTAlgorithm::Vec2f(tmp[1], -tmp[0]) * dis;
}
