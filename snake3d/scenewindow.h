/*
 * scenewindow.h
 *
 *  Created on: 2014-6-5
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
*/

#ifndef _SCENE_WINDOW_H_
#define _SCENE_WINDOW_H_

#include "wymainwindow.h"

#include "htGLFunctions.h"


class WYMainWindow;

class QGLWidget;
class QGLFunctions;

class SceneWindow : public QGLWidget
{
public:
	SceneWindow(QWidget* parent = NULL);
	~SceneWindow();

	static GLuint genTextureWithBuffer(const void* bufferData, GLint w, GLint h, GLenum channelFmt, GLenum dataFmt);

protected:

	void paintGL();
	void initializeGL();
	void resizeGL(int w, int h);

	void mousePressEvent(QMouseEvent *);
	void mouseDoubleClickEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);

	void keyPressEvent(QKeyEvent *);
	void keyReleaseEvent(QKeyEvent *);

private:
	QOpenGLShaderProgram* m_program;
	QMatrix4x4 m_modelView, m_projection;

};

extern SceneWindow* g_sceneWindow;
extern QGLFunctions* g_glFunctions;



#endif