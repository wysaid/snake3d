﻿/*
 * WYGround.cpp
 *
 *  Created on: 2014-6-6
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
*/

#include "WYGround.h"

#define GROUND_TEXTURE_ID GL_TEXTURE1
#define GROUND_TEXTURE_INDEX (GROUND_TEXTURE_ID - GL_TEXTURE0)

static const char* const s_vshGroundNoTexture = SHADER_STRING
(
attribute vec4 v4Position;
uniform mat4 m4MVP;

void main()
{

	gl_Position = m4MVP * v4Position;
}
);

static const char* const s_vshGround = SHADER_STRING
(
attribute vec4 v4Position;
uniform mat4 m4MVP;
varying vec2 v2TexCoord;
uniform vec2 v2GroundSize;

void main()
{

	gl_Position = m4MVP * v4Position;
	v2TexCoord = (v4Position.xy + 1.0) / 2.0 * v2GroundSize;
}
);

static const char* const s_fshGroundNoTexture = SHADER_STRING_PRECISION_M
(
void main()
{
	gl_FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}
);

static const char* const s_fshGround = SHADER_STRING_PRECISION_M
(
uniform sampler2D groundTexture;
varying vec2 v2TexCoord;


void main()
{
	//gl_FragColor = texture2D(groundTexture, fract(v2TexCoord));
	gl_FragColor = texture2D(groundTexture, v2TexCoord);
}
);

static const char* const s_fshGroundMesh = SHADER_STRING_PRECISION_L
(
void main()
{
	gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}
);


const char* const WYGround::paramModelviewMatrixName = "m4MVP";
const char* const WYGround::paramVertexPositionName = "v4Position";
const char* const WYGround::paramGroundTextureName = "groundTexture";
const char* const WYGround::paramGroundSizeName = "v2GroundSize";

WYGround::WYGround() : m_groundVBO(0), m_groundIndexVBO(0), m_groundMeshIndexVBO(0), m_groundIndexSize(0), m_meshIndexSize(0), m_program(NULL), m_programMesh(NULL), m_groundTexture(0), m_groundSize()
{

}

WYGround::~WYGround()
{
	clearGround();
}

void WYGround::genCube(std::vector<HTAlgorithm::Vec3f>& vertexData, std::vector<unsigned short>& indexData, float x, float y, float width, float height)
{
	using HTAlgorithm::Vec3f;
	const float widthStep = 1.0f / width, heightStep = 1.0f / height;
	const Vec3f v(x * widthStep * 2.0f - 1.0f, y * heightStep * 2.0f - 1.0f, 0.0f);

	std::vector<HTAlgorithm::Vec3f>::size_type index = vertexData.size(), indexUp = index + 4;

	vertexData.push_back(v + Vec3f(-widthStep / 2.0f, heightStep / 2.0f, 0.0f));
	vertexData.push_back(v - Vec3f(widthStep / 2.0f, heightStep / 2.0f, 0.0f));
	vertexData.push_back(v + Vec3f(widthStep / 2.0f, -heightStep / 2.0f, 0.0f));
	vertexData.push_back(v + Vec3f(widthStep / 2.0f, heightStep / 2.0f, 0.0f));

	const float z = 1.0f / HT_MAX(width, height);

	vertexData.push_back(v + Vec3f(-widthStep / 2.0f, heightStep / 2.0f, z));
	vertexData.push_back(v - Vec3f(widthStep / 2.0f, heightStep / 2.0f, -z));
	vertexData.push_back(v + Vec3f(widthStep / 2.0f, -heightStep / 2.0f, z));
	vertexData.push_back(v + Vec3f(widthStep / 2.0f, heightStep / 2.0f, z));

	//////////////////////////////////////////////////////////////////////////

	const unsigned short dataIndex[] = 
	{
		//左面
		index, index + 1, indexUp,
		indexUp, index + 1, indexUp + 1,

		//前面
		index + 1, index + 2, indexUp + 1,
		indexUp + 1 , index + 2, indexUp + 2,

		//右面
		index + 2, index + 3, indexUp + 2,
		indexUp + 2, index + 3, indexUp + 3,

		//后面
		index + 3, index, indexUp + 3,
		indexUp + 3, index, indexUp,

		//上面
		indexUp, indexUp + 1, indexUp + 2,
		indexUp, indexUp + 2, indexUp + 3
	};

	const int sz = sizeof(dataIndex) / sizeof(*dataIndex);
	for(int i = 0; i != sz; ++i)
	{
		indexData.push_back(dataIndex[i]);
	}



}

bool WYGround::initWithStage(const int *stage, int w, int h, const char* texName)
{
	m_groundSize[0] = w;
	m_groundSize[1] = h;

	clearGround();

	const float widthStep = 1.0f / w;
	const float heightStep = 1.0f / h;

	m_groundVertices.resize((w + 1) * (h + 1));
	int index = 0;
	for(int i = 0; i <= h; ++i)
	{
		const float heightI = i * heightStep;

		for(int j = 0; j <= w; ++j)
		{
			const HTAlgorithm::Vec3f v(j * widthStep * 2.0f - 1.0f, heightI * 2.0f - 1.0f, 0.0f);
			m_groundVertices[index++] = v;
		}
	}

	index = 0;
	std::vector<unsigned short> meshIndexes;
	m_groundIndexSize = w * h * 6;
	meshIndexes.resize(m_groundIndexSize);

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

	for(int i = 0; i < h; ++i)
	{
		for(int j = 0; j < w; ++j)
		{
			switch (stage[j + i * w])
			{
			case 1:
				genCube(m_groundVertices, meshIndexes, i, j, w, h);
				break;
			default:
				break;
			}
		}
	}

	m_groundIndexSize = meshIndexes.size();

	glGenBuffers(1, &m_groundVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_groundVBO);
	glBufferData(GL_ARRAY_BUFFER, m_groundVertices.size() * sizeof(m_groundVertices[0]), m_groundVertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_groundIndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_groundIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshIndexes.size() * sizeof(meshIndexes[0]), meshIndexes.data(), GL_STATIC_DRAW);

	m_meshIndexSize = (w + 1) * (h + 1) * 4;
	std::vector<unsigned short>	meshIndexes2(m_meshIndexSize);

	index = 0;

	for(int i = 0; i < h; ++i)
	{
		const int pos1 = i * (w + 1);
		const int pos2 = (i + 1) * (w + 1);

		for(int j = 0; j < w; ++j)
		{
			meshIndexes2[index] = pos1 + j;
			meshIndexes2[index + 1] = pos1 + j + 1;
			meshIndexes2[index + 2] = pos1 + j;
			meshIndexes2[index + 3] = pos2 + j;
			index += 4;
		}
		meshIndexes2[index] = pos1 + w;
		meshIndexes2[index + 1] = pos2 + w;
		index += 2;
	}

	const int pos = h * (w + 1);

	for(int i = 0; i < w; ++i)
	{
		meshIndexes2[index] = pos + i;
		meshIndexes2[index + 1] = pos + i + 1;
		index += 2;
	}

	glGenBuffers(1, &m_groundMeshIndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_groundMeshIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshIndexes2.size() * sizeof(meshIndexes2[0]), meshIndexes2.data(), GL_STATIC_DRAW);

	htCheckGLError("WYGround::initWithStage");

	if(texName == NULL || !initGroundTexture(texName))
	{
		return initProgramsNoTexture();
	}

	return  initPrograms();
}

void WYGround::clearGround()
{
	glDeleteBuffers(1, &m_groundVBO);
	m_groundVBO = 0;

	glDeleteBuffers(1, &m_groundIndexVBO);
	m_groundIndexVBO = 0;

	glDeleteBuffers(1, &m_groundMeshIndexVBO);
	m_groundMeshIndexVBO = 0;

}

void WYGround::drawGround(const HTAlgorithm::Mat4& mvp)
{
	m_program->bind();
	m_program->sendUniformMat4x4(paramModelviewMatrixName, 1, GL_FALSE, mvp[0]);

	if(m_groundTexture != 0)
	{
		
		glActiveTexture(GROUND_TEXTURE_ID);
		glBindTexture(GL_TEXTURE_2D, m_groundTexture);
		m_program->sendUniformi(paramGroundTextureName, GROUND_TEXTURE_INDEX);
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_groundVBO);
	glEnableVertexAttribArray(m_vertAttribLocation);
	glVertexAttribPointer(m_vertAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_groundIndexVBO);
	
	glDrawElements(GL_TRIANGLES, m_groundIndexSize, GL_UNSIGNED_SHORT, 0);
	htCheckGLError("drawGround");
}

void WYGround::drawGroundWithMesh(const HTAlgorithm::Mat4& mvp)
{
	m_programMesh->bind();
	m_program->sendUniformMat4x4(paramModelviewMatrixName, 1, GL_FALSE, mvp[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_groundVBO);
	glEnableVertexAttribArray(m_vertAttribLocation);
	glVertexAttribPointer(m_vertAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_groundMeshIndexVBO);

	glDrawElements(GL_LINES, m_meshIndexSize, GL_UNSIGNED_SHORT, 0);
	htCheckGLError("drawGroundWithMesh");
}

bool WYGround::initGroundTexture(const char* texName)
{
	clearGroundTexture();

    QImage image = QImage(texName).convertToFormat(QImage::Format_RGBA8888);
	if(image.width() < 1)
	{
		LOG_ERROR("Failed to open file %s!\n", texName);
		return false;
	}
	m_groundTexture = htGenTextureWithBuffer(image.bits(), image.width(), image.height(), GL_RGBA, GL_UNSIGNED_BYTE);

	htCheckGLError("initGroundTexture");
	return m_groundTexture != 0;
}

void WYGround::clearGroundTexture()
{
	glDeleteTextures(1, &m_groundTexture);
	m_groundTexture = 0;
}

bool WYGround::initPrograms()
{
	clearProgram();

	m_program = new ProgramObject;

	if(!(m_program->initVertexShaderSourceFromString(s_vshGround) &&
		m_program->initFragmentShaderSourceFromString(s_fshGround) &&
		m_program->link()))
	{
		delete m_program;
		m_program = NULL;
		LOG_ERROR("Ground : Program link failed!\n");
		return false;
	}

	m_program->bind();
	m_program->sendUniformf(paramGroundSizeName, m_groundSize[0], m_groundSize[1]);

	m_programMesh = new ProgramObject;

	if(!(m_programMesh->initVertexShaderSourceFromString(s_vshGroundNoTexture) &&
		m_programMesh->initFragmentShaderSourceFromString(s_fshGroundMesh) &&
		m_programMesh->link()))
	{
		delete m_programMesh;
		m_programMesh = NULL;
		LOG_ERROR("Ground : Program link failed!\n");
		return false;
	}

	m_vertAttribLocation = 0;
	m_program->bindAttributeLocation(paramVertexPositionName, m_vertAttribLocation);
	m_programMesh->bindAttributeLocation(paramVertexPositionName, m_vertAttribLocation);
	htCheckGLError("Ground::initPrograms");
	return true;
}

bool WYGround::initProgramsNoTexture()
{
	clearProgram();
	clearGroundTexture();

	m_program = new ProgramObject;

	if(!(m_program->initVertexShaderSourceFromString(s_vshGroundNoTexture) &&
		m_program->initFragmentShaderSourceFromString(s_fshGroundNoTexture) &&
		m_program->link()))
	{
		delete m_program;
		m_program = NULL;
		LOG_ERROR("Ground : Program link failed!\n");
		return false;
	}

	m_programMesh = new ProgramObject;

	if(!(m_programMesh->initVertexShaderSourceFromString(s_vshGroundNoTexture) &&
		m_programMesh->initFragmentShaderSourceFromString(s_fshGroundMesh) &&
		m_programMesh->link()))
	{
		delete m_programMesh;
		m_programMesh = NULL;
		LOG_ERROR("Ground : Program link failed!\n");
		return false;
	}

	m_vertAttribLocation = 0;

	m_program->bindAttributeLocation(paramVertexPositionName, m_vertAttribLocation);
	m_programMesh->bindAttributeLocation(paramVertexPositionName, m_vertAttribLocation);
	htCheckGLError("Ground::initProgramsNoTexture");
	return true;
}

void WYGround::clearProgram()
{
	delete m_programMesh;
	delete m_program;
	m_programMesh = m_program = NULL;
}
