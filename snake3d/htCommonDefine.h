/*
 * htCommonDefine.h
 *
 *  Created on: 2013-12-6
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
*/

#ifndef HTCOMMONDEFINE_H_
#define HTCOMMONDEFINE_H_

#include "htPlatform_QT.h"

enum HTBufferFormat
{
	HT_FORMAT_RGB_INT8, 
	HT_FORMAT_RGB_INT16,
	HT_FORMAT_RGB_FLOAT32,
	HT_FORMAT_RGBA_INT8,		
	HT_FORMAT_RGBA_INT16,		
	HT_FORMAT_RGBA_FLOAT32,
#ifdef GL_BGR
	HT_FORMAT_BGR_INT8, 
	HT_FORMAT_BGR_INT16,
	HT_FORMAT_BGR_FLOAT32,
#endif
#ifdef GL_BGRA
	HT_FORMAT_BGRA_INT8,		
	HT_FORMAT_BGRA_INT16,		
	HT_FORMAT_BGRA_FLOAT32
#endif
};

enum HTTextureBlendMode
{
	HT_BLEND_MIX,			// 0
	HT_BLEND_OVERLAY,		// 1
	HT_BLEND_HARDLIGHT,		// 2
	HT_BLEND_SOFTLIGHT,		// 3
	HT_BLEND_SCREEN,		// 4
	HT_BLEND_LINEARLIGHT,	// 5
	HT_BLEND_VIVIDLIGHT,	// 6
	HT_BLEND_MULTIPLY,		// 7
	HT_BLEND_EXCLUDE,		// 8
	HT_BLEND_COLORBURN,		// 9
	HT_BLEND_DARKEN,		// 10
	HT_BLEND_LIGHTEN,		// 11
	HT_BLEND_COLORDODGE,	// 12
	HT_BLEND_COLORDODGEADOBE,// 13
	HT_BLEND_LINEARDODGE,	// 14
	HT_BLEND_LINEARBURN,	// 15
	HT_BLEND_PINLIGHT,		// 16
	HT_BLEND_HARDMIX,		// 17
	HT_BLEND_DIFFERENCE,	// 18
	HT_BLEND_ADD,			// 19
	HT_BLEND_COLOR,			// 20

	/////////////    Special blend mode below     //////////////

	HT_BlEND_ADD_REVERSE,	// 21
	HT_BLEND_COLOR_BW,		// 22

	/////////////    Special blend mode above     //////////////

	HT_BLEND_TYPE_MAX_NUM //Its value defines the max num of blend.
};


void htPrintGLString(const char*, GLenum);
void _htCheckGLError(const char* name, const char* file, int line);

#define htCheckGLError(name) _htCheckGLError(name, __FILE__, __LINE__);
#define _HT_GET_MACRO_STRING_HELP(x) #x
#define HT_GET_MACRO_STRING(x) _HT_GET_MACRO_STRING_HELP(x)
#define HT_FLOATCOMP0(x)	(x < 0.001f && x > -0.001f)

#ifndef HT_MIN

template<typename Type>
inline Type HT_MIN(Type a, Type b)
{
	return a < b ? a : b;
}

#endif

#ifndef HT_MAX

template<typename Type>
inline Type HT_MAX(Type a, Type b)
{
	return a > b ? a : b;
}

#endif

#ifndef HT_MID

template<typename Type>
inline Type HT_MID(Type n, Type vMin, Type vMax)
{
	return HT_MIN(HT_MAX(n, vMin), vMax);
}

#endif

#endif /* HTCOMMONDEFINE_H_ */
