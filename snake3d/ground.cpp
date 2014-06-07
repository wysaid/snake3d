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
attribute vec4 vPosition;
uniform mat4 mvp;

void main()
{

	gl_Position = mvp * vPosition;
}
);

static const char* const s_fshGround = SHADER_STRING_PRECISION_M
(
void main()
{
	gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);
}
);


static const char* const s_fshGroundMesh = SHADER_STRING_PRECISION_L
(
void main()
{
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
);

const char* const Ground::paramModelviewMatrixName = "mvp";
const char* const Ground::paramVertexPositionName = "vPosition";

Ground::Ground() : m_groundVBO(0), m_groundIndexVBO(0), m_groundMeshIndexVBO(0), m_groundIndexSize(0), m_meshIndexSize(0)
{
	m_program = new WYQOpenGLShaderProgram;

	if(!(m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, s_vshGround) &&
		m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, s_fshGround) &&
		m_program->link()))
	{
		LOG_ERROR("Ground : Program link failed!\n");
	}

	m_programMesh = new WYQOpenGLShaderProgram;

	if(!(m_programMesh->addShaderFromSourceCode(QOpenGLShader::Vertex, s_vshGround) &&
		m_programMesh->addShaderFromSourceCode(QOpenGLShader::Fragment, s_fshGroundMesh) &&
		m_programMesh->link()))
	{
		LOG_ERROR("Ground : Program link failed!\n");
	}

	m_vertAttribLocation = 0;

	m_program->bindAttributeLocation(paramVertexPositionName, m_vertAttribLocation);
	m_programMesh->bindAttributeLocation(paramVertexPositionName, m_vertAttribLocation);

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
		float line = (w + 1) * i;
		float heightI = i * widthStep;

		for(int j = 0; j <= w; ++j)
		{
			const QVector3D v(j * widthStep * 2.0f - 1.0f, heightI * 2.0f - 1.0f, 0.0f);
			m_groundVertices[line + j] = v;
		}
	}

	glGenBuffers(1, &m_groundVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_groundVBO);
	glBufferData(GL_ARRAY_BUFFER, m_groundVertices.size() * sizeof(m_groundVertices[0]), m_groundVertices.data(), GL_DYNAMIC_DRAW);

	int index = 0;
	std::vector<unsigned short> meshIndexes;
	m_groundIndexSize = w * h * 2;
	meshIndexes.resize(m_groundIndexSize * 3);

	for(int i = 0; i < h; ++i)
	{
		const int pos1 = i * (w + 1);
		const int pos2 = (i + 1) * (w + 1);

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

	m_meshIndexSize = (w + 1) * (h + 1) * 2;
	meshIndexes.resize(2 * m_meshIndexSize);

	index = 0;

	for(int i = 0; i < h; ++i)
	{
		const int pos1 = i * (w + 1);
		const int pos2 = (i + 1) * (w + 1);

		for(int j = 0; j < w; ++j)
		{
			meshIndexes[index] = pos1 + j;
			meshIndexes[index + 1] = pos1 + j + 1;
			meshIndexes[index + 2] = pos1 + j;
			meshIndexes[index + 3] = pos2 + j;
			index += 4;
		}
		meshIndexes[index] = pos1 + w;
		meshIndexes[index + 1] = pos2 + w;
		index += 2;
	}

	const int pos = h * (w + 1);

	for(int i = 0; i < w; ++i)
	{
		meshIndexes[index] = pos + i;
		meshIndexes[index + 1] = pos + i + 1;
		index += 2;
	}

	glGenBuffers(1, &m_groundMeshIndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_groundMeshIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshIndexes.size() * sizeof(meshIndexes[0]), meshIndexes.data(), GL_STATIC_DRAW);

	return true;
}

void Ground::clearGround()
{
	glDeleteBuffers(1, &m_groundVBO);
	m_groundVBO = 0;

	glDeleteBuffers(1, &m_groundIndexVBO);
	m_groundIndexVBO = 0;

	glDeleteBuffers(1, &m_groundMeshIndexVBO);
	m_groundMeshIndexVBO = 0;

}

void Ground::drawGround(HTAlgorithm::Mat4& mvp)
{
	m_program->bind();
	m_program->setUniformValue(paramModelviewMatrixName, mvp);

	glBindBuffer(GL_ARRAY_BUFFER, m_groundVBO);
	glEnableVertexAttribArray(m_vertAttribLocation);
	glVertexAttribPointer(m_vertAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_groundIndexVBO);
	
	glDrawElements(GL_TRIANGLES, m_groundIndexSize * 3, GL_UNSIGNED_SHORT, 0);
	htCheckGLError("drawGround");
}

void Ground::drawGroundWithMesh(HTAlgorithm::Mat4& mvp)
{
	m_programMesh->bind();
	m_programMesh->setUniformValue(paramModelviewMatrixName, mvp);

	glBindBuffer(GL_ARRAY_BUFFER, m_groundVBO);
	glEnableVertexAttribArray(m_vertAttribLocation);
	glVertexAttribPointer(m_vertAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_groundMeshIndexVBO);

	glDrawElements(GL_LINES, m_meshIndexSize * 2, GL_UNSIGNED_SHORT, 0);
	htCheckGLError("drawGroundWithMesh");
}