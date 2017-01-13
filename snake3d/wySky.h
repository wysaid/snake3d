/*
 * WYSky.h
 *
 *  Created on: 2014-6-8
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 *        Blog: http://blog.wysaid.org
*/

#ifndef _SKY_H_
#define _SKY_H_

#include "wyShaderFunctions.h"

class WYSky
{
public:
	WYSky();
	~WYSky();

	bool initSky(const char* texName = NULL);

	void drawSky(const wy::Mat4& mvp);
	void drawSkyWithMesh(const wy::Mat4& mvp);

protected:
	static const char* const paramModelviewMatrixName;
	static const char* const paramVertexPositionName;
	static const char* const paramSkyTextureName;

	bool initSkyTexture(const char* texName);
	void clearSkyTexture();
	void clearSkyBuffers();
	bool initPrograms();

protected:
	GLuint m_skyVBO, m_skyIndexVBO;
	GLuint m_skyTexture;
	ProgramObject m_program, m_programMesh;
	GLuint m_vertAttribLocation;
	GLuint m_vertexIndexSize;
};



#endif