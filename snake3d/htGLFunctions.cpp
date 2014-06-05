/*
 * htGLFunctions.cpp
 *
 *  Created on: 2013-12-5
 *      Author: Wang Yang
 */

#include "htGLFunctions.h"

GLuint htGenTextureWithBuffer(const void* bufferData, GLint w, GLint h, GLenum channelFmt, GLenum dataFmt)
{
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