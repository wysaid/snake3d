/*
 * WYStages.h
 *
 *  Created on: 2014-6-4
 *      Author: Wang Yang
 *        Blog: http://blog.wysaid.org
 */

/*

地图定义：

0 代表空地
1 代表初始时的身体，请务必紧紧相连
2 代表初始时的头部，请务必连到身体上

其他： 各种障碍物


*/

#ifndef _STAGES_H_
#define _STAGES_H_

namespace Snake3D
{
	const char* const g_skyTextureName = ":/Snake3D/res/sky3.jpg";

	const int g_stage1[] = 
	{
		1,0,0, 0,0,0, 0,0,0, 0,
		0,0,0, 0,0,1, 0,0,1, 0,
		0,0,0, 0,1,0, 0,0,0, 0,

		0,0,1, 1,1,1, 0,0,0, 0,
		0,1,1, 0,1,1, 0,1,0, 0,
		0,0,0, 0,0,0, 0,0,0, 0,

		0,1,0, 1,0,0, 0,1,0, 0,
		0,1,1, 0,0,1, 0,0,0, 0,
		0,0,0, 0,0,0, 0,0,0, 0,

		0,0,0, 0,0,0, 0,0,1, 0,
	};

	const int g_stage1Width = 10;
	const int g_stage1Height = 10;
	const char* const g_stage1GroundTextureName = ":/Snake3D/res/ground1.jpg";


	const char* const g_stage1SnakeTextureName = ":/Snake3D/res/snake2.jpg";

};

#endif