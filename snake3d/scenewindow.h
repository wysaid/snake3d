﻿/*
 * scenewindow.h
 *
 *  Created on: 2014-6-5
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
*/

#ifndef _SCENE_WINDOW_H_
#define _SCENE_WINDOW_H_

#include <QTimer>
#include <vector>
#include <QImage>

#include "wymainwindow.h"
#include "htPlatform_QT.h"
#include "htGLFunctions.h"
#include "htShaderFunctions.h"

#include "stages.h"
#include "ground.h"

using namespace Snake3D;

class WYMainWindow;

class QGLWidget;
class QGLFunctions;
class Ground;

class SceneWindow : public QGLWidget
{
	Q_OBJECT
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

	void initOrtho(int w, int h);
	void initPerspective(int w, int h);

	void updateModelView();

	void goForward(float dis);
	void goBack(float dis);
	void goLeft(float dis);
	void goRight(float dis);


private:
	ProgramObject *m_programDrawNormal, *m_programDrawMesh;
	HTAlgorithm::Mat4 m_m4ModelView, m_m4Projection;
	HTAlgorithm::Vec2f m_v2Direction, m_v2Position;

private:
	Ground* m_ground;
	int m_lastX, m_lastY;
	float m_farAway, m_headUp;
	bool m_bIsMouseDown;

};

extern SceneWindow* g_sceneWindow;


#endif
