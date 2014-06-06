/*
 * main.cpp
 *
 *  Created on: 2014-6-5
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
*/

#include "wymainwindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	WYMainWindow w;
	w.setWindowTitle(QString::fromLocal8Bit("Snake3D - by wysaid"));
	w.show();
	LOG_INFO("GL_INFO %s = %s\n", "Vendor: ", glGetString(GL_VENDOR));
	LOG_INFO("GL_INFO %s = %s\n", "Version: ", glGetString(GL_VERSION));
	LOG_INFO("GL_INFO %s = %s\n", "Renderer: ", glGetString(GL_RENDERER));

	return a.exec();
}