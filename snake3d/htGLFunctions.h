/*
 * htGLFunctions.h
 *
 *  Created on: 2013-12-5
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 */

#ifndef HTGLFUNCTIONS_H_
#define HTGLFUNCTIONS_H_

#include "htCommonDefine.h"

/*
  为节省texture资源，对OpenGL 所有texture的使用约束如下:
  0号和1号纹理在各种初始化中可能会被多次用到，如果需要绑定固定的纹理，
  请在使用纹理时，从 TEXTURE_START 开始。
  不排除这种需要会增加，所以，
  请使用下面的宏进行加法运算, 以代替手写的 GL_TEXTURE*

*/

#define TEXTURE_START_INDEX 2
#define TEXTURE_START GL_TEXTURE2

#define TEXTURE_INPUT_IMAGE_INDEX 1
#define TEXTURE_INPUT_IMAGE GL_TEXTURE1

#define TEXTURE_OUTPUT_IMAGE_INDEX 0
#define TEXTURE_OUTPUT_IMAGE GL_TEXTURE0

//Mark if the texture is premultiplied with the alpha channel.
#ifndef TEXTURE_PREMULTIPLIED
#define TEXTURE_PREMULTIPLIED 1
#endif

#ifdef ANDROID_NDK

#endif

#ifdef _HT_SHADER_VERSION_

#define HT_GLES_ATTACH_STRING_L "#version " HT_GET_MACRO_STRING(_HT_SHADER_VERSION_) "\n#ifdef GL_ES\nprecision lowp float;\n#endif\n"
#define HT_GLES_ATTACH_STRING_M "#version " HT_GET_MACRO_STRING(_HT_SHADER_VERSION_) "\n#ifdef GL_ES\nprecision mediump float;\n#endif\n"
#define HT_GLES_ATTACH_STRING_H "#version " HT_GET_MACRO_STRING(_HT_SHADER_VERSION_) "\n#ifdef GL_ES\nprecision highp float;\n#endif\n"

#else

#define HT_GLES_ATTACH_STRING_L "#ifdef GL_ES\nprecision lowp float;\n#endif\n"
#define HT_GLES_ATTACH_STRING_M "#ifdef GL_ES\nprecision mediump float;\n#endif\n"
#define HT_GLES_ATTACH_STRING_H "#ifdef GL_ES\nprecision highp float;\n#endif\n"
#endif

//Do not add any precision conf within SHADER_STRING_PRECISION_* macro!
#if defined(_MSC_VER) && _MSC_VER < 1600
//If the m$ compiler is under 10.0, there mustn't be any ',' outside the "()" in the shader string !! 
//For exmaple: vec2(0.0, 0.0) --> YES. 
//             float a, b;  --> No!!!, you must do like this: float a; float b;
#define SHADER_STRING_PRECISION_L(string) HT_GLES_ATTACH_STRING_L  #string
#define SHADER_STRING_PRECISION_M(string) HT_GLES_ATTACH_STRING_M  #string
#define SHADER_STRING_PRECISION_H(string) HT_GLES_ATTACH_STRING_H  #string
#ifndef SHADER_STRING
#define SHADER_STRING(string) #string
#endif
#else
#define SHADER_STRING_PRECISION_L(...) HT_GLES_ATTACH_STRING_L  #__VA_ARGS__
#define SHADER_STRING_PRECISION_M(...) HT_GLES_ATTACH_STRING_M  #__VA_ARGS__
#define SHADER_STRING_PRECISION_H(...) HT_GLES_ATTACH_STRING_H  #__VA_ARGS__
#ifndef SHADER_STRING
#define SHADER_STRING(...) #__VA_ARGS__
#endif
#endif

GLuint htGenTextureWithBuffer(const void* bufferData, GLint w, GLint h, GLenum channelFmt, GLenum dataFmt);

char* htGetScaleBuffer(const void* buffer, int& w, int& h, int channel, int maxSize);

//////////////////////////////////////////////////////////////////////////
#include "htShaderFunctions.h"


#endif /* HTGLFUNCTIONS_H_ */
