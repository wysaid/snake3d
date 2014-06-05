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

class SceneWindow : public QGLWidget
{
public:
	SceneWindow(QWidget* parent = NULL);
	~SceneWindow();

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
	ProgramObject* m_program;

};

#endif