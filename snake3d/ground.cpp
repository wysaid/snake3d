/*
 * ground.cpp
 *
 *  Created on: 2014-6-6
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
*/

#include "ground.h"


static const char* const s_vshGround = SHADER_STRING
(
attribute vec2 vPosition;
uniform mat4 mvp;

void main()
{

	gl_Position = mvp * vec4(vPosition * 2.0 - 1.0, 0.0, 1.0);	
}
);

static const char* const s_fshGround = SHADER_STRING_PRECISION_M
(
void main()
{
	gl_FragColor = vec4(1.0);
}
);


static const char* const s_fshGroundMesh = SHADER_STRING_PRECISION_L
(
void main()
{
	gl_FragColor = vec4(1.0);
}
);

const char* const Ground::paramModelviewMatrixName = "mvp";
const char* const Ground::paramVertexPositionName = "vPosition";

Ground::Ground() : m_groundVBO(0), m_groundIndexVBO(0), m_meshIndexSize(0)
{
	m_program = new QOpenGLShaderProgram;

	if(!(m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, s_vshGround) &&
		m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, s_fshGround) &&
		m_program->link()))
	{
		LOG_ERROR("Ground : Program link failed!\n");
	}

	m_programMesh = new QOpenGLShaderProgram;

	if(!(m_programMesh->addShaderFromSourceCode(QOpenGLShader::Vertex, s_vshGround) &&
		m_programMesh->addShaderFromSourceCode(QOpenGLShader::Fragment, s_fshGroundMesh) &&
		m_programMesh->link()))
	{
		LOG_ERROR("Ground : Program link failed!\n");
	}

}

Ground::~Ground()
{
	clearGround();
}

bool Ground::initWithStage(const int *stage, int w, int h)
{
	clearGround();

	float widthStep = 1.0f / w;
	float heightStep = 1.0f / h;

	m_groundVertices.resize((w + 1) * (h + 1));

	for(int i = 0; i <= h; ++i)
	{
		float line = widthStep * i;

		for(int j = 0; j <= w; ++j)
		{
			m_groundVertices[line + j] = QVector2D(j * widthStep, i * heightStep);
		}
	}

	glGenBuffers(1, &m_groundVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_groundVBO);
	glBufferData(GL_ARRAY_BUFFER, m_groundVertices.size() * sizeof(m_groundVertices[0]), m_groundVertices.data(), GL_DYNAMIC_DRAW);

	int index = 0;
	std::vector<unsigned short> meshIndexes;
	m_meshIndexSize = w * h;
	meshIndexes.resize(m_meshIndexSize * 3);

	for(int i = 0; i < h; ++i)
	{
		int pos1 = i * w;
		int pos2 = (i + 1) * w;

		if(i%2)
		{
			for(int j = 0; j < w; ++j)
			{
				meshIndexes[index] = pos1 + j;
				meshIndexes[index + 1] = pos1 + j + 1;
				meshIndexes[index + 2] = pos2 + j;
				meshIndexes[index + 3] = pos2 + j;
				meshIndexes[index + 4] = pos1 + j + 1;
				meshIndexes[index + 5] = pos2 + j + 1;
				index += 6;
			}
		}
		else
		{
			for(int j = w - 1; j >= 0; --j)
			{
				meshIndexes[index] = pos1 + j + 1;
				meshIndexes[index + 1] = pos2 + j + 1;
				meshIndexes[index + 2] = pos2 + j;
				meshIndexes[index + 3] = pos1 + j;
				meshIndexes[index + 4] = pos1 + j + 1;
				meshIndexes[index + 5] = pos2 + j;		
				index += 6;
			}
		}
	}

	glGenBuffers(1, &m_groundIndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_groundIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshIndexes.size() * sizeof(meshIndexes[0]), meshIndexes.data(), GL_STATIC_DRAW);

	return true;
}

void Ground::clearGround()
{
	glDeleteBuffers(1, &m_groundVBO);
	m_groundVBO = 0;

	glDeleteBuffers(1, &m_groundIndexVBO);
	m_groundIndexVBO = 0;

}

void Ground::drawGround(QMatrix4x4& mvp, bool drawWidthMesh)
{
	QOpenGLShaderProgram* program = drawWidthMesh ? m_programMesh : m_program;
	program->bind();

	program->setUniformValue(paramModelviewMatrixName, mvp);


}