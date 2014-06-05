/*
 * scenewindow.cpp
 *
 *  Created on: 2014-6-5
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
*/


#include "scenewindow.h"

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
	m_program = new ProgramObject;

	if(!(m_program->initVertexShaderSourceFromString(s_vshScene) && 
		m_program->initFragmentShaderSourceFromString(s_fshScene) &&
		m_program->link()))
	{
		LOG_ERROR("Init Shader Program Failed!\n");
	}

	m_program->bind();

	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
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