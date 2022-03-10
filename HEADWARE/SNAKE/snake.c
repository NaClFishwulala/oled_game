#include "snake.h"
#include "stdlib.h"
#include "tim.h"

int time = 0;
int GameMode = snake_game;
typedef struct tagPOINT{
    int  x;//0~127
    int  y;//0~63
} POINT;//需要与oled_cache进行映射

struct MySnake
{
	int size;
	int dir;
	int speed;
	int status;
	POINT coord[2048];
}mysnake;

struct Food
{
	int x;
	int y;
	int flag;
}myfood;

void SnakeGame(void)
{
	if(mysnake.status == alive)
	{
		DrawSnakeMap();
		SnakeMove();
		GameDraw();
		FoodEat();
		SnakeControl();
		SnakeControlSpeed();
	}
	else if(mysnake.status == death)
	{
		GameMode = over;
		OLED_CacheClear();
		OLED_Clear();
		HAL_Delay(500);
		ShowScore();
		HAL_Delay(8000);
	}
}


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
	DrawSnakeMap();
	//初始化蛇
	mysnake.size = SnakeNode;
	mysnake.speed = SnakeSpeed;
	mysnake.dir = right;
	mysnake.status = alive;
	
	for (int i = 0; i < mysnake.size; i++)
	{
		mysnake.coord[i].x = (mysnake.size-i)*SnakeNodeSize+ WALL_Left + WALLSize;
		mysnake.coord[i].y = 2;
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
		for(int j=WALL_Left;j<WALL_Right;j++)
		{
			if(j==WALL_Left || j==WALL_Left+1 || j== WALL_Right-2 || j == WALL_Right-1)
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
	if (mysnake.status == alive)
	{
		//先清除mysnake.coord[mysnake.size - 1].x&y 所对应的oled_cache缓存
		PointToCache(mysnake.coord[mysnake.size - 1].x, mysnake.coord[mysnake.size - 1].y,CLEAR,SnakeNodeSize);		
		for (int i = mysnake.size - 1; i > 0; i--)
		{
			mysnake.coord[i].x = mysnake.coord[i - 1].x;
			mysnake.coord[i].y = mysnake.coord[i - 1].y;
		}
		switch (mysnake.dir)
		{
		case right:
			mysnake.coord[0].x += SnakeNodeSize;
			//撞墙死亡
			if (mysnake.coord[0].x >= WALL_Right - WALLSize)
			{
				mysnake.status = death;
				mysnake.coord[0].x = WALL_Left + WALLSize;
			}
			//撞身子死亡
			for(int i=1;i < mysnake.size;i++)
			{
				if(mysnake.coord[0].x == mysnake.coord[i].x - SnakeNodeSize && mysnake.coord[0].y == mysnake.coord[i].y)
					mysnake.status = death;
			}
			break;
		case left:
			mysnake.coord[0].x -= SnakeNodeSize;
			if (mysnake.coord[0].x < WALL_Left + WALLSize)
			{
				mysnake.status = death;
				mysnake.coord[0].x = WALL_Right - WALLSize;
			}
			for(int i=1;i < mysnake.size;i++)
			{
				if(mysnake.coord[0].x == mysnake.coord[i].x && mysnake.coord[0].y == mysnake.coord[i].y)
					mysnake.status = death;
			}
			break;
		case up:
			mysnake.coord[0].y -= SnakeNodeSize;
			if (mysnake.coord[0].y < WALLSize)
			{
				mysnake.status = death;
				mysnake.coord[0].y = WALLHeight-WALLSize;
			}
			for(int i=1;i < mysnake.size;i++)
			{
				if(mysnake.coord[0].x == mysnake.coord[i].x && mysnake.coord[0].y == mysnake.coord[i].y)
					mysnake.status = death;
			}
			break;
		case down:
			mysnake.coord[0].y += SnakeNodeSize;
			if (mysnake.coord[0].y >= WALLHeight- WALLSize)
			{
				mysnake.status = death;
				mysnake.coord[0].y = WALLSize;
			}
			for(int i=1;i < mysnake.size;i++)
			{
				if(mysnake.coord[0].x == mysnake.coord[i].x && mysnake.coord[0].y == mysnake.coord[i].y - SnakeNodeSize)
					mysnake.status = death;
			}
			break;
		default:
			break;
		}
	}
}

/**
* @brief  蛇方向控制
* @param  None
* @retval 根据joystick的方向来控制蛇的方向
*/
void SnakeControl(void)
{
	switch(direction)
	{
		case up:
			if(mysnake.dir != down)
				mysnake.dir = up;
			break;
		case down:
			if(mysnake.dir != up)
				mysnake.dir = down;
			break;
		case left:
			if(mysnake.dir != right)
				mysnake.dir = left;
			break;
		case right:
			if(mysnake.dir != left)
				mysnake.dir = right;
			break;
		default:
			break;
	}
}

/**
* @brief  产生食物
* @param  None
* @retval 随机生成食物的位坐标
*/
void FoodCreat()
{
	srand(time);
	while(!myfood.flag)
	{
		int flag = 1;
		myfood.x = WALL_Left + WALLSize + rand() % (WALL_Right-WALL_Left - WALLSize * 3);
		myfood.y = (WALLSize + rand()) % (WALLHeight - WALLSize * 2);
		//生成的食物不能在蛇身上
		//遍历蛇的每个节点，如果有和食物相等的部分要重新生成，如果全部不等，零flag为true
		for(int i=0;i<mysnake.size;i++)
		{
			if(mysnake.coord[i].x - SnakeNodeSize < myfood.x && myfood.x < mysnake.coord[i].x + SnakeNodeSize
			&& mysnake.coord[i].y - SnakeNodeSize < myfood.y && myfood.y < mysnake.coord[i].y + SnakeNodeSize)
				flag = 0;
		}
		if(flag == 1)
			myfood.flag = TRUE;
	}

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




void SnakeControlSpeed(void)
{
	if(mysnake.size <= SnakeNode + 5)
		mysnake.speed = low;
	else if(mysnake.size > SnakeNode + 5 && mysnake.size <= SnakeNode + 10)
		mysnake.speed = mid;
	else if(mysnake.size > SnakeNode + 10 && mysnake.size <= SnakeNode + 20)
		mysnake.speed = fast;
	else if(mysnake.size > SnakeNode + 20)
		mysnake.speed = flash;
	switch(mysnake.speed)
	{
		case low:
			HAL_Delay(150);
			break;
		case mid:
			HAL_Delay(100);
			break;
		case fast:
			HAL_Delay(50);
			break;
		case flash:
			HAL_Delay(20);
			break;
		default:
			break;
	}
}

void ShowScore(void)
{
	OLED_ShowNum(20,2,mysnake.size,2,16);
}

/**
* @brief  定时器中断函数
* @param  None
* @retval 
*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == htim3.Instance)
	{
		time = (time++) % 10000;
	}
}
