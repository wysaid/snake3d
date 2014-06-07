/*
 * ground.h
 *
 *  Created on: 2014-6-6
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
*/

#ifndef _GROUND_H_
#define _GROUND_H_

#include "scenewindow.h"

class Ground
{
public:
	Ground();
	~Ground();

	bool initWithStage(const int *stage, int w, int h);
	void clearGround();

	void drawGround(HTAlgorithm::Mat4& mvp);
	void drawGroundWithMesh(HTAlgorithm::Mat4& mvp);

protected:
	static const char* const paramModelviewMatrixName;
	static const char* const paramVertexPositionName;

protected:
	GLuint m_groundVBO, m_groundIndexVBO, m_groundMeshIndexVBO;
	std::vector<QVector3D> m_groundVertices;
	GLuint m_groundIndexSize, m_meshIndexSize;
	WYQOpenGLShaderProgram *m_program, *m_programMesh;
	GLuint m_vertAttribLocation;
};



#endif