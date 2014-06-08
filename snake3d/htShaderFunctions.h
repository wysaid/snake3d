/*
 * htShaderFunctions.h
 *
 *  Created on: 2013-12-6
 *      Author: Wang Yang
 */

#ifndef _HTSAHDERFUNCTIONS_H_
#define _HTSAHDERFUNCTIONS_H_

#include "htGLFunctions.h"

class ShaderObject
{
public:
	ShaderObject();
	~ShaderObject();
	bool init(GLenum shaderType);
	bool loadShaderSourceFromString(const char* shaderString);
	inline GLuint getShaderID() { return m_shaderID; }
	void clear();
private:
	GLenum m_shaderType;
	GLuint m_shaderID;
};

class ProgramObject
{
public:
	ProgramObject();
	~ProgramObject();

	bool initFragmentShaderSourceFromString(const char* fragShader);
	bool initVertexShaderSourceFromString(const char* vertShader);

	bool link();
	void bind();

	// For usage convenience, do not use template here.
	void sendUniformf(const char* name, GLfloat);
	void sendUniformf(const char* name, GLfloat, GLfloat);
	void sendUniformf(const char* name, GLfloat, GLfloat, GLfloat);
	void sendUniformf(const char* name, GLfloat, GLfloat, GLfloat, GLfloat);

	void sendUniformi(const char* name, GLint);
	void sendUniformi(const char* name, GLint, GLint);
	void sendUniformi(const char* name, GLint, GLint, GLint);
	void sendUniformi(const char* name, GLint, GLint, GLint, GLint);

	void sendUniformMat2x2(const char* name, GLsizei count, GLboolean transpose, GLfloat* matrix);
	void sendUniformMat3x3(const char* name, GLsizei count, GLboolean transpose, GLfloat* matrix);
	void sendUniformMat4x4(const char* name, GLsizei count, GLboolean transpose, GLfloat* matrix);

	inline GLuint programId() { return m_programID; }
	inline GLint attributeLocation(const char* name) { return glGetAttribLocation(m_programID, name); }
	inline GLint uniformLocation(const char* name) { return glGetUniformLocation(m_programID, name); } 
	inline void bindAttributeLocation(const char* name, GLuint index) { return glBindAttribLocation(m_programID, index, name); }
private:
	ShaderObject m_vertObj, m_fragObj;
	GLuint m_programID;
};


#endif
