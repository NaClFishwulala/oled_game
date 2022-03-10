#include "snake.h"
#include "stdlib.h"
#include "tim.h"

int time = 0;
int GameMode = snake_game;
typedef struct tagPOINT{
    int  x;//0~127
    int  y;//0~63
} POINT;//��Ҫ��oled_cache����ӳ��

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
* @brief  ��Ϸ��ʼ
* @param  None
* @retval ��Χǽ��Ϣд��oled_cache,��ʼ���ߵ���Ϣ
*/
void GameInit()
{
	OLED_Clear();
	//��յ�ͼ
	OLED_CacheClear();
	//��Χǽ
	DrawSnakeMap();
	//��ʼ����
	mysnake.size = SnakeNode;
	mysnake.speed = SnakeSpeed;
	mysnake.dir = right;
	mysnake.status = alive;
	
	for (int i = 0; i < mysnake.size; i++)
	{
		mysnake.coord[i].x = (mysnake.size-i)*SnakeNodeSize+ WALL_Left + WALLSize;
		mysnake.coord[i].y = 2;
	}
	
	//��ʼ��ʳ��
	FoodCreat();
}	

/**
* @brief  ��Ϸͼ�����
* @param  None
* @retval ��oled_cache������д����Ļ��ʾ
*/
void GameDraw()
{
	//����
	for(int i=0;i<mysnake.size;i++)
		PointToCache(mysnake.coord[i].x, mysnake.coord[i].y,WRITE,SnakeNodeSize);
	//��ʳ��
	if (myfood.flag)
		PointToCache(myfood.x, myfood.y,WRITE,FoodSize);

	OLED_DrawCache();
}

/**
* @brief  ��ϷΧǽ����
* @param  None
* @retval ��Χǽ��Ϣд��oled_cache
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
* @brief  ������
* @param  None
* @retval ���ߵ�����д��oled_cache
*/
void DrawMySnake()
{
	for(int i=0;i<mysnake.size;i++)
		PointToCache(mysnake.coord[i].x, mysnake.coord[i].y,WRITE,SnakeNodeSize);
}



/**
* @brief  ���ƶ�
* @param  None
* @retval �����ߵ��ƶ��ı��ߵ�����
*/
void SnakeMove()
{
	if (mysnake.status == alive)
	{
		//�����mysnake.coord[mysnake.size - 1].x&y ����Ӧ��oled_cache����
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
			//ײǽ����
			if (mysnake.coord[0].x >= WALL_Right - WALLSize)
			{
				mysnake.status = death;
				mysnake.coord[0].x = WALL_Left + WALLSize;
			}
			//ײ��������
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
* @brief  �߷������
* @param  None
* @retval ����joystick�ķ����������ߵķ���
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
* @brief  ����ʳ��
* @param  None
* @retval �������ʳ���λ����
*/
void FoodCreat()
{
	srand(time);
	while(!myfood.flag)
	{
		int flag = 1;
		myfood.x = WALL_Left + WALLSize + rand() % (WALL_Right-WALL_Left - WALLSize * 3);
		myfood.y = (WALLSize + rand()) % (WALLHeight - WALLSize * 2);
		//���ɵ�ʳ�ﲻ����������
		//�����ߵ�ÿ���ڵ㣬����к�ʳ����ȵĲ���Ҫ�������ɣ����ȫ�����ȣ���flagΪtrue
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
* @brief  �Ӵ�ʳ��
* @param  None
* @retval ����ʳ����߳������Ӳ����������µ�ʳ��
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
* @brief  ��ʱ���жϺ���
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
