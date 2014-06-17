/*
 * WYSnake.h
 *
 *  Created on: 2014-6-17
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
*/

#ifndef _WYSNAKE_H_
#define _WYSNAKE_H_

#include "htShaderFunctions.h"

class WYSnake
{
public:
	WYSnake();
	~WYSnake();

	bool init(float x, float y, float len = 3.0f, float xNorm = 0.0f, float yNorm = 1.0f);

	void drawSnake(const HTAlgorithm::Mat4& mvp);

protected:


	bool initSnakeTexture(const char* texName);
	void clearSnakeTexture();
	bool initProgram();
	void clearProgram();

protected:
	GLuint m_snakeVBO, m_snakeIndexVBO;
	std::vector<HTAlgorithm::Vec3f> m_snakeSkeletonVertices;
	ProgramObject m_program;
};


#endif