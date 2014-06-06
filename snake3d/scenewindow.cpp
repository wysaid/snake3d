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

const char* const s_fshScene = SHADER_STRING_PRECISION_M
(

void main()
{
	gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
);


SceneWindow::SceneWindow(QWidget* parent) : QGLWidget(parent)
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
	delete m_program;
}

void SceneWindow::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

	swapBuffers();
}

void SceneWindow::initializeGL()
{
	g_glFunctions = context()->functions();

	m_program = new QOpenGLShaderProgram;

	if(!(m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, s_vshScene) &&
		m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, s_fshScene) &&
		m_program->link()))
	{
		LOG_ERROR("Program link failed!\n");
	}

	glClearColor(0.2f, 0.2f, 0.1f, 1.0f);
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

void resolveOpenGLFunctions()
{

}
