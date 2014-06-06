/*
 * stages.h
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

enum Directions { dirUP, dirDOWN, dirLEFT, dirRIGHT };

namespace Snake3D
{
	const int g_stage1[] = 
	{
		0,0,0, 0,0,0, 0,0,0, 0,
		0,0,0, 0,0,0, 0,0,0, 0,
		0,0,0, 0,0,0, 0,0,0, 0,

		0,0,0, 0,0,0, 0,0,0, 0,
		0,0,0, 0,0,0, 0,0,0, 0,
		0,0,0, 0,0,0, 0,0,0, 0,

		0,0,0, 0,0,0, 0,0,0, 0,
		0,0,0, 0,0,0, 0,0,0, 0,
		0,0,0, 0,0,0, 0,0,0, 0,

		0,0,0, 0,0,0, 0,0,0, 0,
	};

	const int g_stage1Width = 10;
	const int g_stage1Height = 10;


};

