#ifndef __SNAKE_H
#define __SNAKE_H	
#include "oled.h"
#include "joystick.h"


#define SnakeNode 4
#define SnakeSpeed 1
#define SnakeNodeSize 2

#define FoodSize 2

#define TRUE 1
#define FALSE 0

extern unsigned char oled_cache[8][128];
extern enum DIRECTION mydirection;
extern int direction;

void GameInit(void);
void GameDraw(void);
void DrawSnakeMap(void);
void DrawMySnake(void);
void SnakeMove(void);
void SnakeControl(void);
void FoodCreat(void);
void FoodEat(void);
void PointToCache(unsigned char x_pos,unsigned char y_pos,unsigned char flag,unsigned char size);
#endif 
