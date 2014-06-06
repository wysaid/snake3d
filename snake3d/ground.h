/*
 * ground.h
 *
 *  Created on: 2014-6-6
 *      Author: Wang Yang
 *        Mail: admin@wysaid.org
*/

#ifndef _GROUND_H_
#define _GROUND_H_

#include "scenewindow.h"

class Ground
{
public:
	Ground();
	~Ground();

	bool initWithStage(const int *stage, int w, int h);
	void clearGround();

protected:
	GLuint m_groundVBO;
	
};



#endif