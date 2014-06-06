/*
 * scenewindow.cpp
 *
 *  Created on: 2014-6-5
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
*/


#include "scenewindow.h"

SceneWindow* g_sceneWindow = NULL;
QGLFunctions* g_glFunctions = NULL;

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


SceneWindow::SceneWindow(QWidget* parent) : QGLWidget(parent), m_programDrawNormal(NULL), m_programDrawMesh(NULL), m_ground(NULL)
{
	if(g_sceneWindow != NULL)
	{
		LOG_ERROR("Invalid Usage!\n");
	}
	g_sceneWindow = this;
	setAttribute(Qt::WA_PaintOnScreen);
	setAttribute(Qt::WA_NoSystemBackground);
	setAutoBufferSwap(false);
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
	
	QMatrix4x4 qmat = m_projection * m_modelView;

	m_ground->drawGround(qmat);
	m_ground->drawGroundWithMesh(qmat);

	swapBuffers();
	m_modelView.rotate(0.5f, 0.0f, 0.0f, 1.0f);
}

void SceneWindow::initializeGL()
{
	makeCurrent();
	g_glFunctions = context()->functions();

	m_programDrawNormal = new QOpenGLShaderProgram;

	if(!(m_programDrawNormal->addShaderFromSourceCode(QOpenGLShader::Vertex, s_vshScene) &&
		m_programDrawNormal->addShaderFromSourceCode(QOpenGLShader::Fragment, s_fshSceneNormal) &&
		m_programDrawNormal->link()))
	{
		LOG_ERROR("Program link failed!\n");
	}

	m_programDrawMesh = new QOpenGLShaderProgram;

	if(!(m_programDrawMesh->addShaderFromSourceCode(QOpenGLShader::Vertex, s_vshScene) &&
		m_programDrawMesh->addShaderFromSourceCode(QOpenGLShader::Fragment, s_fshSceneMesh) &&
		m_programDrawMesh->link()))
	{
		LOG_ERROR("Program link failed!\n");
	}

	glClearColor(0.2f, 0.2f, 0.1f, 1.0f);

//	initOrtho();
	initPerspective();

	m_ground = new Ground;
	m_ground->initWithStage(g_stage1, g_stage1Width, g_stage1Height);
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), SLOT(updateGL()));
	timer->start(30);
}

void SceneWindow::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

void SceneWindow::mousePressEvent(QMouseEvent *e)
{

}

void SceneWindow::mouseDoubleClickEvent(QMouseEvent *e)
{

}

void SceneWindow::mouseMoveEvent(QMouseEvent *e)
{

}

void SceneWindow::mouseReleaseEvent(QMouseEvent *e)
{

}

void SceneWindow::keyPressEvent(QKeyEvent *e)
{

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

void SceneWindow::initOrtho()
{
	m_modelView.setToIdentity();
	m_projection.ortho(-1.0, 1.0, -1.0, 1.0, 1.0, -1.0);
	m_modelView.lookAt(QVector3D(0.0f, 0.0f, 0.1f), QVector3D(0.0f, -100.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f));
}

void SceneWindow::initPerspective()
{
	float aspectRatio = width() / float(height());
	float z = HT_MIN(width(), height());
	
	m_projection.setToIdentity();
	m_modelView.setToIdentity();

	m_projection.perspective(60.0f, aspectRatio, .1f, 10000.0f);
	//m_projection.ortho(-1.0, 1.0, -1.0, 1.0, 1.0, -1.0);
	m_modelView.lookAt(QVector3D(0.0f, 0.0f, 0.1f), QVector3D(0.0f, -100.0f, 0.0f), QVector3D(0.0f, 0.0f, 1.0f));

}