/*
 * wymainwindow.h
 *
 *  Created on: 2014-6-5
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
*/

#ifndef WYMAINWINDOW_H
#define WYMAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtOpenGL/QGLWidget>
#include <QPainter>
#include <QPaintEngine>
#include <Qstring>
#include <QtEvents>


#include "ui_wymainwindow.h"
#include "scenewindow.h"

class SceneWindow;

class WYMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	WYMainWindow(QWidget *parent = 0);
	~WYMainWindow();

protected:
	void resizeEvent(QResizeEvent *);

private:
	Ui::WYMainWindowClass ui;

	SceneWindow* m_scene;
};

#endif // WYMAINWINDOW_H
