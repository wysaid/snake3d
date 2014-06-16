/*
 * WYSky.cpp
 *
 *  Created on: 2014-6-8
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 *        Blog: http://blog.wysaid.org
*/

#include "WYSky.h"

#define SKY_TEXTURE_ID GL_TEXTURE2
#define SKY_TEXTURE_INDEX (SKY_TEXTURE_ID - GL_TEXTURE0)

#define SKY_RADIUS 10.0f
#define SKY_RADIUS_VERTEX_SIZE 10
#define SKY_PERIMETER_VERTEX_SIZE (SKY_RADIUS_VERTEX_SIZE * 3)

static const char* const s_vshSkyNoTexture = SHADER_STRING
(
attribute vec4 v4Position;
uniform mat4 m4MVP;
varying vec3 v3Color;

void main()
{
	v3Color = v4Position.xyz;
	gl_Position = m4MVP * v4Position;
}
);


static const char* const s_vshSky = SHADER_STRING
(
attribute vec4 v4Position;
uniform mat4 m4MVP;
varying vec2 v2TexCoord;

const float skyRadius = 10.0;

void main()
{

	gl_Position = m4MVP * v4Position;
	v2TexCoord = (v4Position.xy / skyRadius + 1.0) / 2.0;
}
);

static const char* const s_fshSkyNoTexture = SHADER_STRING_PRECISION_M
(
varying vec3 v3Color;
void main()
{
	gl_FragColor = vec4(1.0, v3Color.xy, 1.0);
}
);

static const char* const s_fshSky = SHADER_STRING_PRECISION_M
(
uniform sampler2D skyTexture;
varying vec2 v2TexCoord;


void main()
{
	gl_FragColor = texture2D(skyTexture, v2TexCoord);
}
);

const char* const WYSky::paramModelviewMatrixName = "m4MVP";
const char* const WYSky::paramVertexPositionName = "v4Position";
const char* const WYSky::paramSkyTextureName = "skyTexture";

WYSky::WYSky() : m_skyVBO(0), m_skyIndexVBO(0), m_skyTexture(0), m_program(NULL), m_programMesh(NULL)
{
	m_vertAttribLocation = 0;
}

WYSky::~WYSky()
{
	clearProgram();
	clearSkyTexture();
	clearSkyBuffers();
}

bool WYSky::initSky(const char* texName)
{
	clearSkyBuffers();

	const float radiusStep = SKY_RADIUS / SKY_RADIUS_VERTEX_SIZE;
	const float radianStep = (M_PI * 2.0f) / SKY_PERIMETER_VERTEX_SIZE;

	std::vector<HTAlgorithm::Vec3f> skyVertices;
	skyVertices.resize(SKY_RADIUS_VERTEX_SIZE * (SKY_PERIMETER_VERTEX_SIZE + 1) + 1);
	int index = 0;
	for(int i = 0; i < SKY_RADIUS_VERTEX_SIZE; ++i)
	{
		const float z = i * radiusStep;
		const float dis = sqrtf(SKY_RADIUS*SKY_RADIUS - z*z);
		for(int j = 0; j <= SKY_PERIMETER_VERTEX_SIZE; ++j)
		{
			const float rad = radianStep * j;
			skyVertices[index++] = HTAlgorithm::Vec3f(cosf(rad) * dis, sinf(rad) * dis, z);
		}
	}
	skyVertices[index] = HTAlgorithm::Vec3f(0.0f, 0.0f, SKY_RADIUS);

	glGenBuffers(1, &m_skyVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
	glBufferData(GL_ARRAY_BUFFER, skyVertices.size() * sizeof(skyVertices[0]), skyVertices.data(), GL_STATIC_DRAW);

	index = 0;
	std::vector<unsigned short> meshIndexes;
	m_vertexIndexSize = SKY_RADIUS_VERTEX_SIZE * SKY_PERIMETER_VERTEX_SIZE * 6;
	meshIndexes.resize(m_vertexIndexSize);

	for(int i = 0; i < SKY_RADIUS_VERTEX_SIZE - 1; ++i)
	{
		const int pos1 = i * (SKY_PERIMETER_VERTEX_SIZE + 1);
		const int pos2 = (i + 1) * (SKY_PERIMETER_VERTEX_SIZE + 1);

		for(int j = 0; j < SKY_PERIMETER_VERTEX_SIZE; ++j)
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

	const int pos1 = (SKY_RADIUS_VERTEX_SIZE - 1) * (SKY_PERIMETER_VERTEX_SIZE + 1);
	const int pos2 = skyVertices.size() - 1;

	for(int j = 0; j < SKY_PERIMETER_VERTEX_SIZE; ++j)
	{
		meshIndexes[index] = pos1 + j;
		meshIndexes[index + 1] = pos1 + j + 1;
		meshIndexes[index + 2] = pos2;
		meshIndexes[index + 3] = pos2;
		meshIndexes[index + 4] = pos1 + j + 1;
		meshIndexes[index + 5] = pos2;
		index += 6;
	}

	glGenBuffers(1, &m_skyIndexVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_skyIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshIndexes.size() * sizeof(meshIndexes[0]), meshIndexes.data(), GL_STATIC_DRAW);

	htCheckGLError("WYSky::initSky");

	return initPrograms() && initSkyTexture(texName);
}

void WYSky::drawSky(const HTAlgorithm::Mat4& mvp)
{
	if(m_program == NULL || m_skyTexture == 0)
		return drawSkyWithMesh(mvp);

	m_program->bind();
	m_program->sendUniformMat4x4(paramModelviewMatrixName, 1, GL_FALSE, mvp[0]);

	glActiveTexture(SKY_TEXTURE_ID);
	glBindTexture(GL_TEXTURE_2D, m_skyTexture);
	m_program->sendUniformi(paramSkyTextureName, SKY_TEXTURE_INDEX);


	glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
	glEnableVertexAttribArray(m_vertAttribLocation);
	glVertexAttribPointer(m_vertAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_skyIndexVBO);

	glDrawElements(GL_TRIANGLES, m_vertexIndexSize, GL_UNSIGNED_SHORT, 0);
	htCheckGLError("drawGround");

}

void WYSky::drawSkyWithMesh(const HTAlgorithm::Mat4& mvp)
{
	m_programMesh->bind();
	m_program->sendUniformMat4x4(paramModelviewMatrixName, 1, GL_FALSE, mvp[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_skyVBO);
	glEnableVertexAttribArray(m_vertAttribLocation);
	glVertexAttribPointer(m_vertAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_skyIndexVBO);

	glDrawElements(GL_LINE_STRIP, m_vertexIndexSize, GL_UNSIGNED_SHORT, 0);
	htCheckGLError("drawGroundWithMesh");
}

bool WYSky::initSkyTexture(const char* texName)
{
	clearSkyTexture();
	QImage image = QImage(texName).convertToFormat(QImage::Format_RGBA8888);
	if(image.width() < 1)
	{
		LOG_ERROR("Failed to open file %s!\n", texName);
		return false;
	}

	m_skyTexture = htGenTextureWithBuffer(image.bits(), image.width(), image.height(), GL_RGBA, GL_UNSIGNED_BYTE);

	htCheckGLError("initSkyTexture");
	return m_skyTexture != 0;
}

void WYSky::clearSkyTexture()
{
	glDeleteTextures(1, &m_skyTexture);
	m_skyTexture = 0;
}

void WYSky::clearSkyBuffers()
{
	glDeleteBuffers(1, &m_skyVBO);
	glDeleteBuffers(1, &m_skyIndexVBO);
	m_skyVBO = m_skyIndexVBO = 0;
}

bool WYSky::initPrograms()
{
	clearProgram();

	m_program = new ProgramObject;

	if(!(m_program->initVertexShaderSourceFromString(s_vshSky) &&
		m_program->initFragmentShaderSourceFromString(s_fshSky) &&
		m_program->link()))
	{
		delete m_program;
		m_program = NULL;
		LOG_ERROR("WYSky : Program link failed!\n");
		return false;
	}

	m_programMesh = new ProgramObject;

	if(!(m_programMesh->initVertexShaderSourceFromString(s_vshSkyNoTexture) &&
		m_programMesh->initFragmentShaderSourceFromString(s_fshSkyNoTexture) &&
		m_programMesh->link()))
	{
		delete m_programMesh;
		m_programMesh = NULL;
		LOG_ERROR("WYSky : Program link failed!\n");
		return false;
	}

	m_vertAttribLocation = 0;
	m_program->bindAttributeLocation(paramVertexPositionName, m_vertAttribLocation);
	m_programMesh->bindAttributeLocation(paramVertexPositionName, m_vertAttribLocation);
	htCheckGLError("WYSky::initPrograms");
	return true;
}

void WYSky::clearProgram()
{
	delete m_programMesh;
	delete m_program;
	m_programMesh = m_program = NULL;
}