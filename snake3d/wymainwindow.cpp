/*
 * wymainwindow.cpp
 *
 *  Created on: 2014-6-5
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
*/

#include "wymainwindow.h"

WYMainWindow* g_mainwindow;

WYMainWindow::WYMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	g_mainwindow = this;
	ui.setupUi(this);
	m_scene = new WYSceneWindow(ui.centralWidget);
}

WYMainWindow::~WYMainWindow()
{
	delete m_scene;
}


void WYMainWindow::resizeEvent(QResizeEvent *e)
{
	ui.centralWidget->setGeometry(0, 0, width(), height());
	m_scene->setGeometry(ui.centralWidget->geometry());
}