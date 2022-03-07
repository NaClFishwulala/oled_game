#include "joystick.h"
enum DIRECTION mydirection;

int direction = right;

/**
* @brief  ADC中断回调函数，计算获得数组中的adc采样值
* @param  hadc: ADC handle
* @retval 获取joystick状态
*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc->Instance == ADC1)
	{
		unsigned int x = 0;
		unsigned int y = 0;

		for(int i = 0; i < 50;)
		{
			x += adc_buff[i++];
			y += adc_buff[i++];
		}
			
		x = x / 25 * 330 / 4096;
		y = y / 25 * 330 / 4096;
		
		#if 0
		printf("\n\r x:%d ADC1_Volt:%d.%d%d \n\r",x,x/100,(x/100)%10,x%10);
		printf("\n\r y:%d ADC2_Volt:%d.%d%d \n\r",y,y/100,(y/100)%10,y%10);
		#endif
		
		if(y >= 300)
			direction = up;
		else if(y <= 30)
			direction = down;
		else if(x >= 300)
			direction = left;
		else if(x <= 30)
			direction = right;
	}
}

/**
* @brief  
* @param  
* @retval 
*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == SW_Pin)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		printf("\r\n按键翻转\r\n");
	}
		
}

/**
* @brief  开启ADC采集
* @param  None
* @retval 触发ADC回调函数
*/
//void GetJoyStickStatus(void)
//{

//		
//		#if 0
//		printf("\n\r x:%d ADC1_Volt:%d.%d%d \n\r",x,x/100,(x/100)%10,x%10);
//		printf("\n\r y:%d ADC2_Volt:%d.%d%d \n\r",y,y/100,(y/100)%10,y%10);
//		#endif
//}
