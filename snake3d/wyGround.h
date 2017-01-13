/*
 * WYGround.h
 *
 *  Created on: 2014-6-6
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
*/

#ifndef _GROUND_H_
#define _GROUND_H_

#include "wyShaderFunctions.h"

class WYGround
{
public:
	WYGround();
	~WYGround();

	bool initWithStage(const int *stage, int w, int h, const char* texName = NULL);
	void clearGround();

    void drawGround(const wy::Mat4& mvp);
    void drawGroundWithMesh(const wy::Mat4& mvp);

protected:
	static const char* const paramModelviewMatrixName;
	static const char* const paramVertexPositionName;
	static const char* const paramGroundTextureName;
//	static const char* const paramGroundSizeName;

	bool initGroundTexture(const char* texName);
	void clearGroundTexture();
	bool initPrograms();
	bool initProgramsNoTexture();

    static void genCube(std::vector<wy::Vec3f>& vertexData, std::vector<unsigned short>& indexData, float x, float y, float width, float height);

protected:
	GLuint m_groundVBO, m_groundIndexVBO, m_groundMeshIndexVBO;
    std::vector<wy::Vec3f> m_groundVertices;
//	wy::Vec2f m_groundSize;
	GLuint m_groundIndexSize, m_meshIndexSize;
	GLuint m_groundTexture;
	ProgramObject m_program, m_programMesh;
	GLuint m_vertAttribLocation;
};



#endif
