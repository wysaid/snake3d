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

const char* htGetVersion()
{
	static const char* s_sdkVersion = HT_SDK_VERSION;
	return s_sdkVersion;
}

const char* htGetBlendModeName(HTTextureBlendMode mode)
{
	if(mode < 0 || mode >= HT_BLEND_TYPE_MAX_NUM)
		return NULL;
	static const char* const s_blendModeName[] = 
	{
		"MIX",			// 0
		"OVERLAY",		// 1
		"HARDLIGHT",		// 2
		"SOFTLIGHT",		// 3
		"SCREEN",		// 4
		"LINEARLIGHT",	// 5
		"VIVIDLIGHT",	// 6
		"MULTIPLY",		// 7
		"EXCLUDE",		// 8
		"COLORBURN",		// 9
		"DARKEN",		// 10
		"LIGHTEN",		// 11
		"COLORDODGE",	// 12
		"COLORDODGEADOBE",// 13
		"LINEARDODGE",	// 14
		"LINEARBURN",	// 15
		"PINLIGHT",		// 16
		"HARDMIX",		// 17
		"DIFFERENCE",	// 18
		"ADD",			// 19
		"COLOR",			// 20

		/////////////    Special blend mode below     //////////////

		"ADD_REVERSE",	// 21
		"COLOR_BW",		// 22
	};

	return s_blendModeName[mode];
}