#ifndef __MOTOR_H
#define __MOTOR_H

#include "system.h"

void Emotor_GPIO_Init(void);                              // 编码电机方向脚初始化
void Emotor_PWM_TIM8_Init(u16 arr, u16 psc);              // 编码器电机PWM初始化
void Smotor_GPIO_Init(void);                              // 步进电机方向脚初始化
void Smotor_PWM_TIM1_Init(u16 arr, u16 psc);              // 步进电机PWM初始化
void TIM1_UP_TIM10_IRQHandler(void);                      // 步进电机中断函数
void TIM1_PWM_Output(int channel, FunctionalState state); // TIM1 PWM输出控制
void Sengine_PWM_TIM4_Init(u16 arr, u16 psc);
void Sengine_PWM_TIM5_Init(u16 arr, u16 psc);
#endif
