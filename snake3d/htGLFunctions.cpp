/*
 * htGLFunctions.cpp
 *
 *  Created on: 2013-12-5
 *      Author: Wang Yang
 */

#include "htGLFunctions.h"

static htEnableFunction s_enableOpenGLFunc = NULL;
static htDisableFunction s_disableOpenGLFunc = NULL;
static void* s_htGLParamEnable = NULL;
static void* s_htGLParamDisable = NULL;

static htBufferLoadFun s_loadDataWithSourceNameCommon = NULL;
static htBufferUnloadFun s_unloadBufferDataCommon = NULL;
static void* s_htLoadParam = NULL;
static void* s_htUnloadParam = NULL;
static bool s_stopGlobalEnable = false;

bool htEnableOpenGL()
{
	if(s_enableOpenGLFunc != NULL)
		return s_enableOpenGLFunc(s_htGLParamEnable);
	LOG_CODE
	(
		static bool s_bFirst = true;
		if(s_bFirst)
		{
			s_bFirst = false;
			LOG_ERROR("Warning: Enable Functions are not set.");
		}
	);
	return false;
}

bool htDisableOpenGL()
{
	if(s_disableOpenGLFunc != NULL)
		return s_disableOpenGLFunc(s_htGLParamDisable);
	return false;
}

void htSetGLEnableFunction(htEnableFunction func, void* param)
{
	s_enableOpenGLFunc = func;
	s_htGLParamEnable = param;
}

void htSetGLDisableFunction(htDisableFunction func, void* param)
{
	s_disableOpenGLFunc = func;
	s_htGLParamDisable = param;
}

void* htGetCurrentEnableFunctionParam()
{
	return s_htGLParamEnable;
}

void* htGetCurrentDisableFunctionParam()
{
	return s_htGLParamDisable;
}

void htStopGlobalEnableFunction()
{
	s_stopGlobalEnable = true;
}

void htRestoreGlobalEnableFunction()
{
	s_stopGlobalEnable = false;
}

void htSetCommonLoadFunction(htBufferLoadFun fun, void* arg)
{
	s_loadDataWithSourceNameCommon = fun;
	s_htLoadParam = arg;

}
void htSetCommonUnloadFunction(htBufferUnloadFun fun, void* arg)
{
	s_unloadBufferDataCommon = fun;
	s_htUnloadParam = arg;
}

bool htLoadResourceCommon(const char* sourceName, void** bufferData, GLint* w, GLint* h, HTBufferFormat* fmt)
{
	if(s_loadDataWithSourceNameCommon != NULL)
		return s_loadDataWithSourceNameCommon(sourceName, bufferData, w, h, fmt, s_htLoadParam);
	return false;
}

bool htUnloadResourceCommon(void* bufferData)
{
	if(s_unloadBufferDataCommon != NULL)
		return s_unloadBufferDataCommon(bufferData, s_htUnloadParam);
	return false;
}

htBufferLoadFun htGetCommonLoadFunc()
{
	return s_loadDataWithSourceNameCommon;
}

htBufferUnloadFun htGetCommonUnloadFunc()
{
	return s_unloadBufferDataCommon;
}

void* htGetCommonLoadArg()
{
	return s_htLoadParam;
}

void* htGetCommonUnloadArg()
{
	return s_htUnloadParam;
}

void htGetDataAndChannelByFormat(HTBufferFormat fmt, GLenum* dataFmt, GLenum* channelFmt, GLint* channel)
{
	GLenum df, cf;
	GLint c;
	switch(fmt)
	{
	case HT_FORMAT_RGB_INT8:
		df = GL_UNSIGNED_BYTE;
		cf = GL_RGB;
		c = 3;
		break;
	case HT_FORMAT_RGBA_INT8:
		df = GL_UNSIGNED_BYTE;
		cf = GL_RGBA;
		c = 4;
		break;
	case HT_FORMAT_RGB_INT16:
		df = GL_UNSIGNED_SHORT;
		cf = GL_RGB;
		c = 3;
		break;
	case HT_FORMAT_RGBA_INT16:
		df = GL_UNSIGNED_SHORT;
		cf = GL_RGBA;
		c = 4;
		break;
	case HT_FORMAT_RGB_FLOAT32:
		df = GL_FLOAT;
		cf = GL_RGB;
		c = 3;
		break;
	case HT_FORMAT_RGBA_FLOAT32:
		df = GL_FLOAT;
		cf = GL_RGB;
		c = 4;
		break;

#ifdef GL_BGR
	case HT_FORMAT_BGR_INT8:
		df = GL_UNSIGNED_BYTE;
		cf = GL_BGR;
		c = 3;
		break;
	case HT_FORMAT_BGR_INT16:
		df = GL_UNSIGNED_SHORT;
		cf = GL_BGR;
		c = 3;
		break;
	case HT_FORMAT_BGR_FLOAT32:
		df = GL_FLOAT;
		cf = GL_BGR;
		c = 3;
		break;
#endif
#ifdef GL_BGRA
	case HT_FORMAT_BGRA_INT8:
		df = GL_UNSIGNED_BYTE;
		cf = GL_BGRA;
		c = 4;
		break;	
	case HT_FORMAT_BGRA_INT16:
		df = GL_UNSIGNED_SHORT;
		cf = GL_BGRA;
		c = 4;
		break;	
	case HT_FORMAT_BGRA_FLOAT32:
		df = GL_FLOAT;
		cf = GL_BGRA;
		c = 4;
		break;
#endif
	default:
		df = GL_FALSE;
		cf = GL_FALSE;
		c = 0;
	}

	if(dataFmt != NULL) *dataFmt = df;
	if(channelFmt != NULL) *channelFmt = cf;
	if(channel != NULL) *channel = c;
}

GLuint htGenTextureWithBuffer(const void* bufferData, GLint w, GLint h, GLenum channelFmt, GLenum dataFmt)
{
	htEnableOpenGL();
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, channelFmt, dataFmt, bufferData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	return tex;
}

char* htGetScaleBuffer(const void* buffer, int& w, int& h, int channel, int maxSize)
{
	if(maxSize > HT_MAX(w, h) || buffer == NULL) return NULL;
	char *tmpbuffer = NULL;
	const char* data = (const char*)buffer;

	double scale;
	int width = w, height = h;
	if(w > h)
	{
		h = h * maxSize / w;
		w = maxSize;
		scale = double(width) / w;
	}
	else
	{
		w = w * maxSize / h;
		h = maxSize;
		scale = double(height) / h;
	}
	int len = w * h;
	tmpbuffer = new char[len * channel];
	//	memset(tmpbuffer, 0, len * channel);
	if(channel == 4)
	{
		for(int i = 0; i != h; ++i)
		{
			for(int j = 0; j != w; ++j)
			{
				const int L = (j + i * w) * channel;
				const int R = (static_cast<int>(j * scale) + static_cast<int>(i * scale) * width) * channel;
				tmpbuffer[L] = data[R];
				tmpbuffer[L+1] = data[R+1];
				tmpbuffer[L+2] = data[R+2];
				tmpbuffer[L+3] = data[R+3];
			}
		}
	}
	else
	{
		for(int i = 0; i != h; ++i)
		{
			for(int j = 0; j != w; ++j)
			{
				const int L = (j + i * w) * channel;
				const int R = (static_cast<int>(j * scale) + static_cast<int>(i * scale) * width) * channel;
				tmpbuffer[L] = data[R];
				tmpbuffer[L+1] = data[R+1];
				tmpbuffer[L+2] = data[R+2];
			}
		}
	}
	return tmpbuffer;
}