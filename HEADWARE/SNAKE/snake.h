#ifndef __SNAKE_H
#define __SNAKE_H	
#include "oled.h"
#include "joystick.h"


#define SnakeNode 4
#define SnakeSpeed 1
#define SnakeNodeSize 4

#define WALLSize 2
#define WALL_Left 16
#define WALL_Right 112
#define WALLHeight 64

#define FoodSize 4

#define TRUE 1
#define FALSE 0

enum SPEED
{
		low,mid,fast
};

extern unsigned char oled_cache[8][128];
extern enum DIRECTION mydirection;
extern int direction;

void GameInit(void);
void GameDraw(void);
void SnakeGame(void);
void DrawSnakeMap(void);
void DrawMySnake(void);
void SnakeMove(void);
void SnakeControl(void);
void SnakeControlSpeed(void);
void FoodCreat(void);
void FoodEat(void);
void PointToCache(unsigned char x_pos,unsigned char y_pos,unsigned char flag,unsigned char size);
#endif 
