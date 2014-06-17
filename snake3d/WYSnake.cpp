/*
 * WYSnake.h
 *
 *  Created on: 2014-6-17
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 * Description: First time to use skeleton animation, and I'll use my own method.
*/

#include "WYSnake.h"

#define SNAKE_VERTEX_PER_UNIT 1
#define SNAKE_RADIUS 0.5f
#define SNAKE_RADIUS_VERTEX_SIZE 3
#define SNAKE_PERIMETER_VERTEX_SIZE (SNAKE_RADIUS_VERTEX_SIZE * 3)

static const char* const s_vshSnake = SHADER_STRING
(
x
);

static const char* const s_fshSnake = SHADER_STRING_PRECISION_M
(

);

static const char* const s_vshSnakeNoTexture = SHADER_STRING
(
x
);

static const char* const s_fshSnakeNotexture = SHADER_STRING_PRECISION_M
(

);

const char* const WYSnake::paramModelviewMatrixName = "";
const char* const WYSnake::paramVertexPositionName = "";
const char* const WYSnake::paramSnakeTextureName = "";

WYSnake::WYSnake() : m_snakeVBO(0), m_snakeIndexVBO(0), m_vertAttribLocation(0), m_snakeTexture(0)
{

}

WYSnake::~WYSnake()
{
	clearSnakeTexture();
	clearSnakeBuffers();
}

bool WYSnake::init(float x, float y, float len, float xNorm, float yNorm)
{
	clearSnakeBuffers();
	using namespace HTAlgorithm;
	Vec2f norm(xNorm, yNorm);
	Vec2f pos(x, y);
	norm.normalize();

	float snakeStride = 1.0f / SNAKE_VERTEX_PER_UNIT;

	for(float f = 0.0f; f < len; f += snakeStride)
	{
		const Vec2f v(norm * f);
		const Vec2f coord(pos - v);
		const SnakeBody bd(coord[0], coord[1], norm[0], norm[1]);
		m_snakeSkeleton.push_back(bd);
	}
	initSnakeBuffers();
	return initPrograms();
}

void WYSnake::drawSnake(const HTAlgorithm::Mat4& mvp)
{

}

void WYSnake::genModelBySkeleton()
{
	const float lengthStep = 1.0f / SNAKE_VERTEX_PER_UNIT;
	const float radianStep = (M_PI * 2.0f) / SNAKE_PERIMETER_VERTEX_SIZE;

	std::vector<HTAlgorithm::Vec3f> snakeVertices;
	snakeVertices.resize(m_snakeSkeleton.size() * (SNAKE_PERIMETER_VERTEX_SIZE + 1));

	int sz = (int)m_snakeSkeleton.size() - 1;

	for(int i = 0; i < sz; ++i)
	{
		SnakeBody bd1 = m_snakeSkeleton[i];
		SnakeBody bd2 = m_snakeSkeleton[i + 1];
		
	}

}

void WYSnake::move(float motion)
{

}

void WYSnake::turnLeft()
{

}

void WYSnake::turnRight()
{

}

bool WYSnake::initSnakeTexture(const char* texName)
{
	clearSnakeTexture();

	QImage image = QImage(texName).convertToFormat(QImage::Format_RGBA8888);
	if(image.width() < 1)
	{
		LOG_ERROR("Failed to open file %s!\n", texName);
		return false;
	}
	m_snakeTexture = htGenTextureWithBuffer(image.bits(), image.width(), image.height(), GL_RGBA, GL_UNSIGNED_BYTE);

	htCheckGLError("initGroundTexture");
	return m_snakeTexture != 0;
}

void WYSnake::clearSnakeTexture()
{
	glDeleteTextures(1, &m_snakeTexture);
	m_snakeTexture = 0;
}

bool WYSnake::initPrograms()
{
	if(!(m_program.initVertexShaderSourceFromString(s_vshSnake) &&
		m_program.initFragmentShaderSourceFromString(s_fshSnake) &&
		m_program.link()))
	{
		LOG_ERROR("WYSnake : Program link failed!\n");
		return false;
	}

	if(!(m_programMesh.initVertexShaderSourceFromString(s_vshSnakeNoTexture) &&
		m_programMesh.initFragmentShaderSourceFromString(s_fshSnakeNotexture) &&
		m_programMesh.link()))
	{
		LOG_ERROR("WYSnake : Program link failed!\n");
		return false;
	}

	m_vertAttribLocation = 0;
	m_program.bindAttributeLocation(paramVertexPositionName, m_vertAttribLocation);
	m_programMesh.bindAttributeLocation(paramVertexPositionName, m_vertAttribLocation);
	htCheckGLError("WYSnake::initPrograms");
	return true;
}

void WYSnake::initSnakeBuffers()
{
	clearSnakeBuffers();
	glGenBuffers(1, &m_snakeVBO);
	glGenBuffers(1, &m_snakeIndexVBO);
}

void WYSnake::clearSnakeBuffers()
{
	glDeleteBuffers(1, &m_snakeVBO);
	glDeleteBuffers(1, &m_snakeIndexVBO);
	m_snakeVBO = m_snakeIndexVBO = 0;
}