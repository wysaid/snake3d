/*
 * htPlatforms.h
 *
 *  Created on: 2014-6-8
 *      Author: Wang Yang
 *  Description: be compatible with none-angle qt versions.
 */

#include "wyPlatform_QT.h"

#if !defined(QT_OPENGL_ES_2) && !defined(Q_OS_MAC)

namespace snake3D_OpenGLFunctions
{
	QGLFunctions* g_glFunctions = NULL;
}

#endif
