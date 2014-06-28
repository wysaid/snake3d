/*
 * WYSnake.h
 *
 *  Created on: 2014-6-17
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
 * Description: First time to use skeleton animation, and I'll use my own method.
*/

#ifndef _WYSNAKE_H_
#define _WYSNAKE_H_

#include "htShaderFunctions.h"

//2014-6-28修正：将蛇神修正为结点管理方式。 每个结点不再自由运动，以避免出错。

//本游戏中的蛇实际上依旧在二维平面上跑(xOy平面)， 所以不需要太考虑z值
//关于贪吃蛇拐弯原理： 蛇身每个节点都有自己前进的方向，
//蛇身每个节点在向前运动时会与前一个节点之间坐标差与前进方向计算点积，若小于等于0，才转弯。（实现平滑移动)
struct SnakeBody
{
	SnakeBody() : pos(), dPos() {}
	SnakeBody(float fx, float fdx, float fy, float fdy, float fz = 0.0f, float fdz = 0.0f) : pos(fx, fy, fz), dPos(fdx, fdy, fdz) {}
	SnakeBody(HTAlgorithm::Vec3f fPos, HTAlgorithm::Vec3f fdPos) : pos(fPos), dPos(fdPos) {}
	HTAlgorithm::Vec3f pos;
	HTAlgorithm::Vec3f dPos; //dPos 必须标准化!
};

class WYSnake
{
public:
	WYSnake();
	~WYSnake();

	//x, y表示蛇头位置
	//len表示蛇身长度
	//xNorm与yNorm表示蛇身与蛇头初始状态下的朝向（初始状态下没有弯曲）
	bool init(float x, float y, const char* texName, float len = 3.0f, float xNorm = 0.0f, float yNorm = 1.0f);	

	void drawSnake(const HTAlgorithm::Mat4& mvp);
	void drawSnakeWithMesh(const HTAlgorithm::Mat4& mvp);

	void move(float motion);
	inline void turnLeft() { m_nextTurn = Turn_Left; }
	inline void turnRight() { m_nextTurn = Turn_Right; }
	inline void goAhead() { m_nextTurn = Turn_None; }

protected:
	static const char* const paramModelviewMatrixName;
	static const char* const paramVertexPositionName;
	static const char* const paramSnakeTextureName;
	static const char* const paramSnakeDirName;
	static const char* const paramSnakeRelDataName;

	bool initSnakeTexture(const char* texName);
	void clearSnakeTexture();
	bool initPrograms();
	void initSnakeBuffers();
	void clearSnakeBuffers();
	GLuint genModelBySkeleton(); //根据骨骼生成模型
//	GLuint genFacesBySkeleton();

	enum SnakeTurning { Turn_None, Turn_Left, Turn_Right };

protected:
	GLuint m_snakeVBO, m_snakeDirVBO, m_snakeRelDataVBO, m_snakeIndexVBO;
	GLuint m_snakeVertIndexSize;
	std::vector<SnakeBody> m_snakeSkeleton[2];
	ProgramObject m_program, m_programMesh;
	GLuint m_vertAttribLocation, m_dirAttribLocation, m_relDataAttribLocation;
	GLuint m_snakeTexture;
	SnakeTurning m_nextTurn;
	int m_skeletonIndex;
};


#endif