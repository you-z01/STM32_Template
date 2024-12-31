
#include "led.h"

/****************************************************************************************
 * @description: LED初始化函数
 * @return {*}
 ****************************************************************************************/
void LED_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOC, GPIO_Pin_13); //初始化熄灭LED
}


/****************************************************************************************
 * @description: LED测试函数
 * @return {*}
 ****************************************************************************************/
void LED_Test()
{
	led1 = 0; //亮
	delay_ms(1000);
	led1 = 1; //灭
	delay_ms(1000);
}
