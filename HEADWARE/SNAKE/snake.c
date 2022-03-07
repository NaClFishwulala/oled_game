#include "snake.h"
#include "stdlib.h"

typedef struct tagPOINT{
    char  x;//0~127
    char  y;//0~63
} POINT;//需要与oled_cache进行映射

struct MySnake
{
	int size;
	int dir;
	int speed;
	int status;
	POINT coord[5000];
}mysnake;

struct Food
{
	int x;
	int y;
	int flag;
}myfood;

/**
* @brief  游戏开始
* @param  None
* @retval 将围墙信息写入oled_cache,初始化蛇的信息
*/
void GameInit()
{
	OLED_Clear();
	//清空地图
	OLED_CacheClear();
	//画围墙
//	DrawSnakeMap();
	//初始化蛇
	mysnake.size = SnakeNode;
	mysnake.speed = SnakeSpeed;
	mysnake.dir = direction;
	mysnake.status = TRUE;
	
	for (int i = 0; i < mysnake.size; i++)
	{
		mysnake.coord[i].x = (mysnake.size-i)*SnakeNodeSize+10;
		mysnake.coord[i].y = 6;
	}
	
	//初始化食物
	FoodCreat();
}	

/**
* @brief  游戏图像绘制
* @param  None
* @retval 将oled_cache的数据写入屏幕显示
*/
void GameDraw()
{
	//画蛇
	for(int i=0;i<mysnake.size;i++)
		PointToCache(mysnake.coord[i].x, mysnake.coord[i].y,WRITE,SnakeNodeSize);
	//画食物
	if (myfood.flag)
		PointToCache(myfood.x, myfood.y,WRITE,FoodSize);

	OLED_DrawCache();
}

/**
* @brief  游戏围墙绘制
* @param  None
* @retval 将围墙信息写入oled_cache
*/
void DrawSnakeMap()
{
	for(int i=0;i<ROW;i++)
	{
		for(int j=0;j<Column;j++)
		{
			if(j==0 || j==1 || j== Column-2 || j == Column-1)
				oled_cache[i][j] = 0xff;
			else if(i == 0)
				oled_cache[i][j] = 0x03;
			else if(i == ROW-1)
				oled_cache[i][j] = 0xc0;
		}
	}
}

/**
* @brief  绘制蛇
* @param  None
* @retval 将蛇的数据写入oled_cache
*/
void DrawMySnake()
{
	for(int i=0;i<mysnake.size;i++)
		PointToCache(mysnake.coord[i].x, mysnake.coord[i].y,WRITE,SnakeNodeSize);
}



/**
* @brief  蛇移动
* @param  None
* @retval 根据蛇的移动改变蛇的坐标
*/
void SnakeMove()
{
	if (mysnake.status)
	{
		//先清除mysnake.coord[mysnake.size - 1].x&y 所对应的oled_cache缓存
		PointToCache(mysnake.coord[mysnake.size - 1].x, mysnake.coord[mysnake.size - 1].y,CLEAR,SnakeNodeSize);
		for (int i = mysnake.size - 1; i > 0; i--)
		{
			mysnake.coord[i].x = mysnake.coord[i - 1].x;
			mysnake.coord[i].y = mysnake.coord[i - 1].y;
		}
		mysnake.dir = direction;
		switch (mysnake.dir)
		{
		case right:
			mysnake.coord[0].x += SnakeSpeed;
			if (mysnake.coord[0].x >= Max_Column)
				mysnake.coord[0].x = 0;
			break;
		case left:
			mysnake.coord[0].x -= SnakeSpeed;
			if (mysnake.coord[0].x <= 0)
				mysnake.coord[0].x = Max_Column;
			break;
		case up:
			mysnake.coord[0].y -= SnakeSpeed;
			if (mysnake.coord[0].y <= 0)
				mysnake.coord[0].y = Max_Row;
			break;
		case down:
			mysnake.coord[0].y += SnakeSpeed;
			if (mysnake.coord[0].y >= Max_Row)
				mysnake.coord[0].y = 0;
			break;
		default:
			break;
		}
	}
}

/**
* @brief  产生食物
* @param  None
* @retval 随机生成食物的位坐标
*/
void FoodCreat()
{
	myfood.x = rand() % Max_Column;
	myfood.y = rand() % Max_Row;
	myfood.flag = TRUE;
}

/**
* @brief  接触食物
* @param  None
* @retval 触碰食物后蛇长度增加并重新生成新的食物
*/
void FoodEat()
{
	if (mysnake.coord[0].x + FoodSize >= myfood.x && mysnake.coord[0].x - FoodSize <= myfood.x
		&& mysnake.coord[0].y + FoodSize >= myfood.y && mysnake.coord[0].y - FoodSize <= myfood.y
		&& myfood.flag)
	{
		myfood.flag = FALSE;
		PointToCache(myfood.x, myfood.y,CLEAR,SnakeNodeSize);
		mysnake.size++;
	}

	if (!myfood.flag)
		FoodCreat();
}
