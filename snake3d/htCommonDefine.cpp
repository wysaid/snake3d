#include "htCommonDefine.h"

void htPrintGLString(const char* name, GLenum em)
{
	LOG_INFO("GL_INFO %s = %s\n", name, glGetString(em));
}

void _htCheckGLError(const char* name, const char* file, int line)
{
	for (GLint error = glGetError(); error; error = glGetError())
	{		
		const char* pMsg;
		switch (error) 
		{
		case GL_INVALID_ENUM: pMsg = "invalid enum"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: pMsg = "invalid framebuffer operation"; break;
		case GL_INVALID_OPERATION: pMsg = "invalid operation"; break;
		case GL_INVALID_VALUE: pMsg = "invalid value"; break;
		case GL_OUT_OF_MEMORY: pMsg = "out of memory"; break;
		default: pMsg = "unknown error";
		}
		LOG_ERROR("After %s() glGetError %s(0x%x) at %s:%d\n", name, pMsg, error, file, line);
	}
}
