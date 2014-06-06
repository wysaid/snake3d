/*
 * ground.cpp
 *
 *  Created on: 2014-6-6
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
*/

#include "ground.h"

Ground::Ground() : m_groundVBO(0)
{

}

Ground::~Ground()
{
	clearGround();
}

bool Ground::initWithStage(const int *stage, int w, int h)
{

	return true;
}

void Ground::clearGround()
{
	glDeleteBuffers(1, &m_groundVBO);
	m_groundVBO = 0;
}