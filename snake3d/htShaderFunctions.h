/*
 * htShaderFunctions.h
 *
 *  Created on: 2013-12-6
 *      Author: Wang Yang
 */

#ifndef _HTSAHDERFUNCTIONS_H_
#define _HTSAHDERFUNCTIONS_H_

#include "htCommonDefine.h"
#include "htGLFunctions.h"

#include <fstream>
#include <vector>
#include <memory>

class ShaderObject
{
public:
	ShaderObject();
	~ShaderObject();
	bool init(GLenum shaderType);
	bool loadShaderSourceFromFile(const char* filename);
	bool loadShaderSourceFromString(const char* shaderString);
	inline GLuint getShaderID() { return m_shaderID; }
	void clear();
private:
	GLuint getShaderFileLength(std::fstream& file);
	void formatShaderSource(const char* src);
	GLenum m_shaderType;
	GLuint m_shaderID;
};

class ProgramObject
{
public:
	ProgramObject();
	~ProgramObject();
	
	bool initFragmentShaderSourceFromFile(const char* fragFileName);
	bool initVertexShaderSourceFromFile(const char* vertFileName);

	bool initFragmentShaderSourceFromString(const char* fragShader);
	bool initVertexShaderSourceFromString(const char* vertShader);

	bool link();
	void bind();

	// For usage convenience, do not use template here.
	void sendUniformf(const char* name, float);
	void sendUniformf(const char* name, float, float);
	void sendUniformf(const char* name, float, float, float);
	void sendUniformf(const char* name, float, float, float, float);

	void sendUniformi(const char* name, int);
	void sendUniformi(const char* name, int, int);
	void sendUniformi(const char* name, int, int, int);
	void sendUniformi(const char* name, int, int, int, int);

	GLuint programId() { return m_programID; }
	GLint attributeLocation(const char* name) { return glGetAttribLocation(m_programID, name); }
	GLint uniformLocation(const char* name) { return glGetUniformLocation(m_programID, name); } 
private:
	ShaderObject m_vertObj, m_fragObj;
	GLuint m_programID;
};


#endif
