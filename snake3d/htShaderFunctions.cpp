/*
 * htShaderFunctions.cpp
 *
 *  Created on: 2013-12-5
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
*/

#include "htCommonDefine.h"
#include "htGLFunctions.h"
#include "htShaderFunctions.h"

#define GETUNIFORM(uniform, programID, name) \
GLint uniform = glGetUniformLocation(programID, name);\
if(uniform < 0) \
{\
LOG_ERROR("uniform name %s does not exist!\n", name);\
return ;\
}\

ShaderObject::ShaderObject() : m_shaderID(0)
{
	m_shaderType = GL_FALSE;
}

ShaderObject::~ShaderObject()
{
	clear();
}

bool ShaderObject::init(GLenum shaderType)
{
	LOG_INFO("Init %s \n", (shaderType == GL_VERTEX_SHADER) ? "VertexShader" : "FragmentShader");
	m_shaderType = shaderType;
	if(m_shaderID != 0)
		glDeleteShader(m_shaderID);

	m_shaderID = glCreateShader(m_shaderType);
	if(m_shaderID == 0)
		return false;
	return true;
}

void ShaderObject::clear()
{
	if(m_shaderID == 0) return;
	glDeleteShader(m_shaderID);
	LOG_INFO("%s Shader release\n", m_shaderType == GL_VERTEX_SHADER ? "Vertex" : "Fragment");
	m_shaderID = 0;
	m_shaderType = GL_FALSE;
}

bool ShaderObject::loadShaderSourceFromFile(const char* filename)
{
	using std::fstream;
	fstream file(filename, fstream::in | fstream::binary);
	if(!file) return false;
	GLuint len = getShaderFileLength(file);
	char *buffer = new char[len+1];
	file.read(buffer, len);
	buffer[len] = '\0';
	formatShaderSource(buffer);
	file.close();
	bool status = loadShaderSourceFromString(buffer);
	delete [] buffer;
	return status;
}

bool ShaderObject::loadShaderSourceFromString(const char* shaderString)
{
//	LOG_INFO("Shader String: \n%s\n", shaderString);

	if(m_shaderID == 0)
		m_shaderID = glCreateShader(m_shaderType);
	if(m_shaderID == 0) 
	{
		LOG_ERROR("glCreateShader Failed!");
		return false;
	}
	glShaderSource(m_shaderID, 1, (const GLchar**)&shaderString, NULL);
	glCompileShader(m_shaderID);
	GLint compiled = 0;
	glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &compiled);
	
	if(compiled == GL_TRUE) return true;

	GLint logLen;
	glGetShaderiv(m_shaderID, GL_INFO_LOG_LENGTH, &logLen);
	if(logLen > 0)
	{
		char *buf = new char[logLen];
		if(buf != NULL)
		{
			glGetShaderInfoLog(m_shaderID, logLen, &logLen, buf);
			LOG_ERROR("Shader %d compile faild: \n%s\n", m_shaderID, buf);
			delete [] buf;
		}
		glDeleteShader(m_shaderID);
		m_shaderID = 0;
	}
	return false;
}

GLuint ShaderObject::getShaderFileLength(std::fstream& file)
{
	using std::fstream;
	if(!file.good()) return 0;
	file.seekg(0, fstream::end);
	GLuint len = (GLuint)file.tellg();
	file.seekg(fstream::beg);
	if(len > 0xffff)
	{
		LOG_ERROR("Shader File is Too Large!\n");
		return 0;
	}
	return len;
}

void ShaderObject::formatShaderSource(const char* src)
{

}

ProgramObject::ProgramObject()
{
	m_programID = glCreateProgram();
	LOG_INFO("CREATE PROGRAM!!!! --> %d\n", m_programID);
}

ProgramObject::~ProgramObject()
{
	GLuint attachedShaders[32];
	int numAttachedShaders;
	glGetAttachedShaders(m_programID, 32, &numAttachedShaders, attachedShaders);
	for(int i = 0; i < numAttachedShaders; ++i)
	{
		glDetachShader(m_programID, attachedShaders[i]);
	}
	htCheckGLError("Detach Shaders in useProgram");
	glDeleteProgram(m_programID);
	LOG_INFO("ProgramObject release\n");
}

bool ProgramObject::initFragmentShaderSourceFromFile(const char* fragFileName)
{
	return m_programID != 0 && m_fragObj.init(GL_FRAGMENT_SHADER) &&
		m_fragObj.loadShaderSourceFromFile(fragFileName);
}

bool ProgramObject::initVertexShaderSourceFromFile(const char* vertFileName)
{
	return m_programID != 0 && m_vertObj.init(GL_VERTEX_SHADER) &&
		m_vertObj.loadShaderSourceFromFile(vertFileName);
}

bool ProgramObject::initFragmentShaderSourceFromString(const char* fragShader)
{
	return m_programID != 0 && m_fragObj.init(GL_FRAGMENT_SHADER) &&
		m_fragObj.loadShaderSourceFromString(fragShader);
}

bool ProgramObject::initVertexShaderSourceFromString(const char* vertShader)
{
	return m_programID != 0 && m_vertObj.init(GL_VERTEX_SHADER) &&
		m_vertObj.loadShaderSourceFromString(vertShader);
}

bool ProgramObject::link()
{
	LOG_INFO("COMPILE PROGRAM!!!!\n");
	GLuint attachedShaders[32];
	int numAttachedShaders, programStatus;
	glGetAttachedShaders(m_programID, 32, &numAttachedShaders, attachedShaders);
	for(int i = 0; i < numAttachedShaders; ++i)
	{
		glDetachShader(m_programID, attachedShaders[i]);
	}
	htCheckGLError("Detach Shaders in useProgram");
	glAttachShader(m_programID, m_vertObj.getShaderID());
	glAttachShader(m_programID, m_fragObj.getShaderID());
	htCheckGLError("Attach Shaders in useProgram");
	glLinkProgram(m_programID);
	glGetProgramiv(m_programID, GL_LINK_STATUS, &programStatus);
	m_vertObj.clear();
	m_fragObj.clear();
	if(programStatus != GL_TRUE)
	{
		GLint logLen = 0;
		glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &logLen);
		if(logLen != 0)
		{
			char *buf = new char[logLen];
			if(buf != NULL)
			{
				glGetProgramInfoLog(m_programID, logLen, &logLen, buf);
				LOG_ERROR("Failed to link the program!\n%s", buf);
				delete [] buf;
			}
		}
		LOG_INFO("LINK %d Failed\n", m_programID);
		return false;
	}
	LOG_INFO("LINK %d OK\n", m_programID);
	htCheckGLError("Link Program");
	return true;
}

void ProgramObject::bind()
{
	glUseProgram(m_programID);
	//LOG_INFO("USE PROGRAM!!!! -->%d", m_programID);
}

void ProgramObject::sendUniformf(const char* name, float x)
{
	GETUNIFORM(uniform, m_programID, name);
	glUniform1f(uniform, x);
}

void ProgramObject::sendUniformf(const char* name, float x, float y)
{
	GETUNIFORM(uniform, m_programID, name);
	glUniform2f(uniform, x, y);
}

void ProgramObject::sendUniformf(const char* name, float x, float y, float z)
{
	GETUNIFORM(uniform, m_programID, name);
	glUniform3f(uniform, x, y, z);
}

void ProgramObject::sendUniformf(const char* name, float x, float y, float z, float w)
{
	GETUNIFORM(uniform, m_programID, name);
	glUniform4f(uniform, x, y, z, w);
}

void ProgramObject::sendUniformi(const char* name, int x)
{
	GETUNIFORM(uniform, m_programID, name);
	glUniform1i(uniform, x);
}

void ProgramObject::sendUniformi(const char* name, int x, int y)
{
	GETUNIFORM(uniform, m_programID, name);
	glUniform2i(uniform, x, y);
}

void ProgramObject::sendUniformi(const char* name, int x, int y, int z)
{
	GETUNIFORM(uniform, m_programID, name);
	glUniform3i(uniform, x, y, z);
}

void ProgramObject::sendUniformi(const char* name, int x, int y, int z, int w)
{
	GETUNIFORM(uniform, m_programID, name);
	glUniform4i(uniform, x, y, z, w);
}
