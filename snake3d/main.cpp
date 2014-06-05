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
	w.show();
	htPrintGLString("Vendor: ", GL_VENDOR);
	htPrintGLString("Renderer: ", GL_RENDERER);
	htPrintGLString("Version: ", GL_VERSION);
	return a.exec();
}