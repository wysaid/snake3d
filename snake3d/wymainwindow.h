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
#include <QString>
#include <QtEvents>


#include "ui_wymainwindow.h"
#include "WYSceneWindow.h"

class WYSceneWindow;

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

	WYSceneWindow* m_scene;
};

extern WYMainWindow* g_mainwindow;

#endif // WYMAINWINDOW_H
