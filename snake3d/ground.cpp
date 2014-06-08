/*
 * ground.cpp
 *
 *  Created on: 2014-6-6
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
*/

#include "ground.h"

#define GROUND_TEXTURE_ID GL_TEXTURE0
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
	v2TexCoord = (v4Position.xy + 1.0) / 2.0;
}
);

static const char* const s_fshGroundNoTexture = SHADER_STRING_PRECISION_M
(
void main()
{
	gl_FragColor = vec4(0.0, 1.0, 1.0, 1.0);
}
);

static const char* const s_fshGround = SHADER_STRING_PRECISION_M
(
uniform sampler2D groundTexture;
varying vec2 v2TexCoord;
void main()
{
	gl_FragColor = texture2D(groundTexture, v2TexCoord);//vec4(0.0, 1.0, 1.0, 1.0);
}
);

static const char* const s_fshGroundMesh = SHADER_STRING_PRECISION_L
(
void main()
{
	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
}
);

const char* const Ground::paramModelviewMatrixName = "m4MVP";
const char* const Ground::paramVertexPositionName = "v4Position";
const char* const Ground::paramGroundTextureName = "groundTexture";
const char* const Ground::paramGroundSizeName = "v2GroundSize";

Ground::Ground() : m_groundVBO(0), m_groundIndexVBO(0), m_groundMeshIndexVBO(0), m_groundIndexSize(0), m_meshIndexSize(0), m_program(NULL), m_programMesh(NULL), m_groundTexture(0)
{

}

Ground::~Ground()
{
	clearGround();
}

bool Ground::initWithStage(const int *stage, int w, int h, const char* texName)
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
			const HTAlgorithm::Vec3f v(j * widthStep * 2.0f - 1.0f, heightI * 2.0f - 1.0f, 0.0f);
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

	if(texName == NULL)
	{
		return initProgramsNoTexture();
	}

	return initGroundTexture(texName) && initPrograms();
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
	m_program->sendUniformMat4x4(paramModelviewMatrixName, 1, GL_FALSE, mvp[0]);

	if(m_groundTexture != 0)
	{
		
		glActiveTexture(GROUND_TEXTURE_ID);
		glBindTexture(GL_TEXTURE_2D, m_groundTexture);
		m_program->sendUniformi(paramGroundTextureName, GROUND_TEXTURE_INDEX);
		
// 		GLint u = m_program->uniformLocation(paramGroundTextureName);
// 		glUniform1i(m_program->uniformLocation(paramGroundTextureName), GROUND_TEXTURE_INDEX);
	}

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
	m_program->sendUniformMat4x4(paramModelviewMatrixName, 1, GL_FALSE, mvp[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_groundVBO);
	glEnableVertexAttribArray(m_vertAttribLocation);
	glVertexAttribPointer(m_vertAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_groundMeshIndexVBO);

	glDrawElements(GL_LINES, m_meshIndexSize * 2, GL_UNSIGNED_SHORT, 0);
	htCheckGLError("drawGroundWithMesh");
}

bool Ground::initGroundTexture(const char* texName)
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

void Ground::clearGroundTexture()
{
	glDeleteTextures(1, &m_groundTexture);
	m_groundTexture = 0;
}

bool Ground::initPrograms()
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

bool Ground::initProgramsNoTexture()
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

void Ground::clearProgram()
{
	delete m_programMesh;
	delete m_program;
	m_programMesh = m_program = NULL;
}
