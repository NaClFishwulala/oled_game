#include "snake.h"
#include "stdlib.h"

typedef struct tagPOINT{
    char  x;//0~127
    char  y;//0~63
} POINT;//��Ҫ��oled_cache����ӳ��

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
//	DrawSnakeMap();
	//��ʼ����
	mysnake.size = SnakeNode;
	mysnake.speed = SnakeSpeed;
	mysnake.dir = right;
	mysnake.status = TRUE;
	
	for (int i = 0; i < mysnake.size; i++)
	{
		mysnake.coord[i].x = (mysnake.size-i)*SnakeNodeSize+10;
		mysnake.coord[i].y = 6;
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
	if (mysnake.status)
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
			if (mysnake.coord[0].x >= Max_Column)
				mysnake.coord[0].x = 0;
			break;
		case left:
			mysnake.coord[0].x -= SnakeNodeSize;
			if (mysnake.coord[0].x <= 0)
				mysnake.coord[0].x = Max_Column;
			break;
		case up:
			mysnake.coord[0].y -= SnakeNodeSize;
			if (mysnake.coord[0].y <= 0)
				mysnake.coord[0].y = Max_Row;
			break;
		case down:
			mysnake.coord[0].y += SnakeNodeSize;
			if (mysnake.coord[0].y >= Max_Row)
				mysnake.coord[0].y = 0;
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
	myfood.x = rand() % Max_Column;
	myfood.y = rand() % Max_Row;
	myfood.flag = TRUE;
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


