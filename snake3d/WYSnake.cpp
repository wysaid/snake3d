/*
 * WYSnake.h
 *
 *  Created on: 2014-6-17
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 * Description: First time to use skeleton animation, and I'll use my own method.
*/

#include "WYSnake.h"

#define SNAKE_TEXTURE_ID GL_TEXTURE2
#define SNAKE_TEXTURE_INDEX (SNAKE_TEXTURE_ID - GL_TEXTURE0)

#define SNAKE_VERTEX_PER_UNIT 5
#define SNAKE_RADIUS 0.5f
#define SNAKE_RADIUS_VERTEX_SIZE 10
#define SNAKE_PERIMETER_VERTEX_SIZE (SNAKE_RADIUS_VERTEX_SIZE * 3)

#define SNAKE_DEFAULT_HEIGHT 0.5f
#define SNAKE_DEFAULT_ZNORM 0.0f

static const char* const s_vshSnake = SHADER_STRING
(
varying vec2 v2TexCoord;
attribute vec3 v3Position;
attribute vec3 v3Norm;
attribute vec2 v2Relative;
uniform mat4 m4MVP;

const float PI2 = 3.14159265 * 2.0;
 
void main()
{
	vec3 turn = normalize(v3Norm);
	float cosRad = -turn.y;
	float sinRad = sqrt(1.0 - cosRad * cosRad);
	if(v3Norm.x < 0.0)
		sinRad = -sinRad;

	float angle = PI2 * v2Relative.x;
	//Points in xOz平面
	vec3 pos = v3Position + mat3(cosRad, sinRad, 0.0,
		-sinRad, cosRad, 0.0,
		0.0, 0.0, 1.0) * vec3(cos(angle), 0.0, sin(angle)) * 0.3;

	gl_Position = m4MVP * vec4(pos, 1.0);

	v2TexCoord = vec2(v2Relative.y, angle / PI2);
}
);

static const char* const s_fshSnake = SHADER_STRING_PRECISION_M
(
uniform sampler2D snakeTexture;
varying vec2 v2TexCoord;


void main()
{
	gl_FragColor = texture2D(snakeTexture, v2TexCoord);
}
);

static const char* const s_vshSnakeNoTexture = SHADER_STRING
(
attribute vec3 v3Position;
attribute vec3 v3Norm;
attribute vec2 v2Relative;
varying vec3 v3Color;
uniform mat4 m4MVP;

const float PI2 = 3.14159265 * 2.0;

void main()
{
	vec3 turn = normalize(v3Norm);
	float cosRad = -turn.y;
	float sinRad = sqrt(1.0 - cosRad * cosRad);
	if(v3Norm.x < 0.0)
		sinRad = -sinRad;

	float angle = PI2 * v2Relative.x;
	//Points in xOz平面
	vec3 pos = mat3(cosRad, sinRad, 0.0,
		-sinRad, cosRad, 0.0,
		0.0, 0.0, 1.0) * vec3(cos(angle), 0.0, sin(angle));

	gl_Position = m4MVP * vec4(v3Position + pos * 0.5, 1.0);
	v3Color = abs(pos);
}
);

static const char* const s_fshSnakeNotexture = SHADER_STRING_PRECISION_M
(
varying vec3 v3Color;
void main()
{
	gl_FragColor = vec4(v3Color.x, 1.0, v3Color.z, 1.0);
}
);

const char* const WYSnake::paramModelviewMatrixName = "m4MVP";
const char* const WYSnake::paramVertexPositionName = "v3Position";
const char* const WYSnake::paramSnakeTextureName = "snakeTexture";
const char* const WYSnake::paramSnakeDirName = "v3Norm";
const char* const WYSnake::paramSnakeRelDataName = "v2Relative";

WYSnake::WYSnake() : m_snakeVBO(0), m_snakeIndexVBO(0), m_snakeDirVBO(0), m_snakeRelDataVBO(0), m_vertAttribLocation(0), m_snakeTexture(0), m_nextTurn(Turn_None), m_skeletonIndex(0)
{

}

WYSnake::~WYSnake()
{
	clearSnakeTexture();
	clearSnakeBuffers();
}

bool WYSnake::init(float x, float y, const char* texName, float len, float xNorm, float yNorm)
{
	clearSnakeBuffers();
	using namespace HTAlgorithm;
	Vec2f norm(xNorm, yNorm);
	Vec2f pos(x, y);
	norm.normalize();
	m_snakeSkeleton[0].clear();
	m_snakeSkeleton[1].clear();
	float snakeStride = 1.0f / SNAKE_VERTEX_PER_UNIT;
	
// 	const SnakeBody bd2(2.0, 2.0, 0.0, 0.0, SNAKE_DEFAULT_HEIGHT, SNAKE_DEFAULT_ZNORM);
// 	m_snakeSkeleton.push_back(bd2);
// 
// 	const SnakeBody bd1(1.0, 1.0, 0.0, 0.0, SNAKE_DEFAULT_HEIGHT, SNAKE_DEFAULT_ZNORM);
// 	m_snakeSkeleton.push_back(bd1);
	
	for(float f = 0.0f; f < len; f += snakeStride)
	{
		const Vec2f v(norm * f);
		const Vec2f coord(pos - v);
		const SnakeBody bd(coord[0], norm[0], coord[1], norm[1], SNAKE_DEFAULT_HEIGHT, SNAKE_DEFAULT_ZNORM);
		m_snakeSkeleton[0].push_back(bd);
		m_snakeSkeleton[1].push_back(bd);
	}
	
	initSnakeBuffers();
	return initPrograms() && initSnakeTexture(texName);
}

void WYSnake::drawSnake(const HTAlgorithm::Mat4& mvp)
{
	GLuint indexSize = genModelBySkeleton();
	m_program.bind();
	m_program.sendUniformMat4x4(paramModelviewMatrixName, 1, GL_FALSE, mvp[0]);
	glActiveTexture(SNAKE_TEXTURE_ID);
	glBindTexture(GL_TEXTURE_2D, m_snakeTexture);
	m_program.sendUniformi(paramSnakeTextureName, SNAKE_TEXTURE_INDEX);

	glBindBuffer(GL_ARRAY_BUFFER, m_snakeVBO);
	glEnableVertexAttribArray(m_vertAttribLocation);
	glVertexAttribPointer(m_vertAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_snakeDirVBO);
	glEnableVertexAttribArray(m_dirAttribLocation);
	glVertexAttribPointer(m_dirAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_snakeRelDataVBO);
	glEnableVertexAttribArray(m_relDataAttribLocation);
	glVertexAttribPointer(m_relDataAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_snakeIndexVBO);
	glDrawElements(GL_TRIANGLES, indexSize, GL_UNSIGNED_SHORT, 0);
	htCheckGLError("drawSnake");
}

void WYSnake::drawSnakeWithMesh(const HTAlgorithm::Mat4& mvp)
{
	GLuint indexSize = genModelBySkeleton();
	m_programMesh.bind();
	m_programMesh.sendUniformMat4x4(paramModelviewMatrixName, 1, GL_FALSE, mvp[0]);

	glBindBuffer(GL_ARRAY_BUFFER, m_snakeVBO);
	glEnableVertexAttribArray(m_vertAttribLocation);
	glVertexAttribPointer(m_vertAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_snakeDirVBO);
	glEnableVertexAttribArray(m_dirAttribLocation);
	glVertexAttribPointer(m_dirAttribLocation, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_snakeRelDataVBO);
	glEnableVertexAttribArray(m_relDataAttribLocation);
	glVertexAttribPointer(m_relDataAttribLocation, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_snakeIndexVBO);
	glDrawElements(GL_LINE_STRIP, indexSize, GL_UNSIGNED_SHORT, 0);
	htCheckGLError("drawSnakeWithMesh");
}

//Todo: 使用统一body，在shader中进行模型变换。
//body 都在xOy平面内，绕Z轴旋转
//计算所需数据： 

GLuint WYSnake::genModelBySkeleton()
{
	using namespace HTAlgorithm;

	const float lengthStep = 1.0f / SNAKE_VERTEX_PER_UNIT;
	const float radianStep = (M_PI * 2.0f) / (SNAKE_PERIMETER_VERTEX_SIZE - 1);

	std::vector<Vec3f> snakeVertices;
	std::vector<Vec3f> snakeDir;
	std::vector<Vec2f> snakeRelData;
	std::vector<SnakeBody>& snakeSkeleton = m_snakeSkeleton[m_skeletonIndex];
	int snakeDataSize = snakeSkeleton.size() * SNAKE_PERIMETER_VERTEX_SIZE + 1; //加蛇头

 	snakeVertices.resize(snakeDataSize);
	snakeDir.resize(snakeDataSize);
	snakeRelData.resize(snakeDataSize);

	const int sz = (int)snakeSkeleton.size() - 1;

	int index = 0;

	for(int i = 1; i < sz; ++i)
	{
		const SnakeBody body = snakeSkeleton[i];
		const Vec3f v3Pos(body.pos);
		const Vec3f v3Norm = snakeSkeleton[i - 1].pos - snakeSkeleton[i + 1].pos;
		for(int j = 0; j < SNAKE_PERIMETER_VERTEX_SIZE; ++j)
		{
			snakeVertices[index] = v3Pos;
			snakeDir[index] = v3Norm;
			snakeRelData[index] = Vec2f(float(j) / (SNAKE_PERIMETER_VERTEX_SIZE - 1), i / float(SNAKE_VERTEX_PER_UNIT));
			++index;
		}
	}

	snakeVertices[index] = snakeSkeleton[0].pos;
	snakeDir[index] = snakeSkeleton[0].pos - snakeSkeleton[1].pos;
	snakeRelData[index] = Vec2f(-1.0f / 4.0f, 0.0f);

	int snakeHeadIndex = index;
	
	std::vector<unsigned short> meshIndexes;
	GLuint indexSize = (sz - 2) * (SNAKE_PERIMETER_VERTEX_SIZE - 1) * 6 + SNAKE_PERIMETER_VERTEX_SIZE * 3;
	meshIndexes.resize(indexSize);
	
	index = 0;

	//蛇头
	{
		const int pos1 = snakeHeadIndex;

		for(int j = 0; j < SNAKE_PERIMETER_VERTEX_SIZE - 1; ++j)
		{
			meshIndexes[index] = pos1;
			meshIndexes[index + 1] = j;
			meshIndexes[index + 2] = j + 1;
			index += 3;
		}
		meshIndexes[index] = pos1;
		meshIndexes[index + 1] = SNAKE_PERIMETER_VERTEX_SIZE - 1;
		meshIndexes[index + 2] = 0;
		index += 3;
	}	

	for(int i = 0; i < sz - 2; ++i)
	{
		const int pos1 = i * SNAKE_PERIMETER_VERTEX_SIZE;
		const int pos2 = (i + 1) * SNAKE_PERIMETER_VERTEX_SIZE;

		for(int j = 0; j < SNAKE_PERIMETER_VERTEX_SIZE - 1; ++j)
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

	glBindBuffer(GL_ARRAY_BUFFER, m_snakeVBO);
	glBufferData(GL_ARRAY_BUFFER, snakeVertices.size() * sizeof(snakeVertices[0]), snakeVertices.data(), GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_snakeDirVBO);
	glBufferData(GL_ARRAY_BUFFER, snakeDir.size() * sizeof(snakeDir[0]), snakeDir.data(), GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, m_snakeRelDataVBO);
	glBufferData(GL_ARRAY_BUFFER, snakeRelData.size() * sizeof(snakeRelData[0]), snakeRelData.data(), GL_STREAM_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_snakeIndexVBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshIndexes.size() * sizeof(meshIndexes[0]), meshIndexes.data(), GL_STREAM_DRAW);

	htCheckGLError("WYSnake::genModelBySkeleton");
	return indexSize;
}

void WYSnake::move(float motion)
{
	using namespace HTAlgorithm;
	std::vector<SnakeBody>& snakeSkeletonRead = m_snakeSkeleton[m_skeletonIndex];
	(++m_skeletonIndex) %= 2;
	std::vector<SnakeBody>& snakeSkeletonWrite = m_snakeSkeleton[m_skeletonIndex];

	if(snakeSkeletonWrite.empty() || snakeSkeletonRead.size() != snakeSkeletonWrite.size())
	{
		LOG_ERROR("WYSnake::move Invalid Snake!\n");
		return;
	}

	std::vector<SnakeBody>::iterator iterWrite = snakeSkeletonWrite.begin();
	std::vector<SnakeBody>::iterator iterWriteEnd = snakeSkeletonWrite.end() - 1;

	std::vector<SnakeBody>::iterator iterRead = snakeSkeletonRead.begin();
	std::vector<SnakeBody>::iterator iterReadEnd = snakeSkeletonRead.end() - 1;

	iterWrite->pos = iterRead->pos + iterRead->dPos * motion;

	if(m_nextTurn != Turn_None)
	{
		Vec3f& v = iterRead->pos;
		Vec3f& dvR = iterRead->dPos;
		Vec3f& dvW = iterWrite->dPos;
		if((fabsf(dvR[0]) > 0.1 && fabsf(v[0] - int(v[0]) < 0.02)) ||
			(fabsf(dvR[1]) > 0.1 && fabsf(v[1] - int(v[1]) < 0.02)))
		{
// 			v[0] = int(v[0]);
// 			v[1] = int(v[1]);

			switch (m_nextTurn)
			{
			case WYSnake::Turn_Left:
				dvW = Vec3f(-dvR[1], dvR[0], 0.0f);
				break;
			case WYSnake::Turn_Right:
				dvW = Vec3f(dvR[1], -dvR[0], 0.0f);
				break;
			default:
				break;
			}
			m_nextTurn = Turn_None;
		}		
	}

	for(++iterRead, ++iterWrite; iterWrite < iterWriteEnd; ++iterRead, ++iterWrite)
	{
		const SnakeBody& prev = *(iterRead - 1);
		const SnakeBody& next = *(iterRead + 1);
		const SnakeBody& nowR = *iterRead;
		

		SnakeBody& nowW = *iterWrite;

		Vec3f nextPos = nowR.pos + nowR.dPos * motion;

		const Vec3f norm1 = (prev.pos + prev.dPos * motion) - nextPos;
		const Vec3f norm2 = (next.pos + next.dPos * motion) - nextPos;
		const Vec3f& v = nextPos;
		const Vec3f& dvR = nowR.dPos;

		if(norm1.dot(norm2) >= 0.0f &&
			(fabsf(dvR[0]) > 0.1 && fabsf(v[0] - int(v[0]) < 0.02)) ||
			(fabsf(dvR[1]) > 0.1 && fabsf(v[1] - int(v[1]) < 0.02)))
		{
			nowW.dPos = prev.dPos;
		}

		nowW.pos = nowR.pos + nowR.dPos * motion;
	}
	
	if(snakeSkeletonWrite.size() > 1)
	{
		iterWriteEnd->pos = iterReadEnd->pos + iterReadEnd->dPos * motion;

		Vec3f norm = (iterReadEnd-1)->pos - iterReadEnd->pos;
		if(norm.dot(iterReadEnd->dPos) < 0.0f)
		{
			iterWriteEnd->dPos = (iterReadEnd - 1)->dPos;
		}		
	}
	snakeSkeletonRead = snakeSkeletonWrite;
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
	m_vertAttribLocation = 0;
	m_dirAttribLocation = 1;
	m_relDataAttribLocation = 2;

	if(m_program.initVertexShaderSourceFromString(s_vshSnake) &&
		m_program.initFragmentShaderSourceFromString(s_fshSnake))
	{
		m_program.bindAttributeLocation(paramVertexPositionName, m_vertAttribLocation);
		m_program.bindAttributeLocation(paramSnakeDirName, m_dirAttribLocation);
		m_program.bindAttributeLocation(paramSnakeRelDataName, m_relDataAttribLocation);
	}

	if(!m_program.link())
	{
		LOG_ERROR("WYSnake : Program link failed!\n");
		return false;
	}

	if(m_programMesh.initVertexShaderSourceFromString(s_vshSnakeNoTexture) &&
		m_programMesh.initFragmentShaderSourceFromString(s_fshSnakeNotexture))
	{
		m_programMesh.bindAttributeLocation(paramVertexPositionName, m_vertAttribLocation);	
		m_programMesh.bindAttributeLocation(paramSnakeDirName, m_dirAttribLocation);
		m_programMesh.bindAttributeLocation(paramSnakeRelDataName, m_relDataAttribLocation);
	}

	if(!m_programMesh.link())
	{
		LOG_ERROR("WYSnake : Program link failed!\n");
		return false;
	}	

	htCheckGLError("WYSnake::initPrograms");
	return true;
}

void WYSnake::initSnakeBuffers()
{
	clearSnakeBuffers();
	glGenBuffers(1, &m_snakeVBO);
	glGenBuffers(1, &m_snakeIndexVBO);
	glGenBuffers(1, &m_snakeDirVBO);
	glGenBuffers(1, &m_snakeRelDataVBO);
}

void WYSnake::clearSnakeBuffers()
{
	glDeleteBuffers(1, &m_snakeVBO);
	glDeleteBuffers(1, &m_snakeIndexVBO);
	glDeleteBuffers(1, &m_snakeDirVBO);
	glDeleteBuffers(1, &m_snakeRelDataVBO);
	m_snakeVBO = m_snakeDirVBO = m_snakeRelDataVBO = m_snakeIndexVBO = 0;
}