#ifndef _system_H
#define _system_H

#include "stm32f10x.h"

//============================头文件======================================
//系统头文件
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//FreeRTOS相关头文件 
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

//自定义相关头文件
#include "SysTick.h"
#include "usart.h"
#include "led.h"

//任务相关头文件
#include "Bluetooth_control.h"

//======================================================================



//位带操作,实现 51 类似的 GPIO 控制功能
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr&0xFFFFF)<<5)+(bitnum<<2))
#define MEM_ADDR(addr) *((volatile unsigned long *)(addr))
#define BIT_ADDR(addr, bitnum) MEM_ADDR(BITBAND(addr, bitnum))
//IO 口地址映射
#define GPIOA_ODR_Addr (GPIOA_BASE+12) //0x4001080C
#define GPIOB_ODR_Addr (GPIOB_BASE+12) //0x40010C0

#define GPIOC_ODR_Addr (GPIOC_BASE+12) //0x4001100C
#define GPIOD_ODR_Addr (GPIOD_BASE+12) //0x4001140C
#define GPIOE_ODR_Addr (GPIOE_BASE+12) //0x4001180C
#define GPIOF_ODR_Addr (GPIOF_BASE+12) //0x40011A0C
#define GPIOG_ODR_Addr (GPIOG_BASE+12) //0x40011E0C
#define GPIOA_IDR_Addr (GPIOA_BASE+8) //0x40010808
#define GPIOB_IDR_Addr (GPIOB_BASE+8) //0x40010C08
#define GPIOC_IDR_Addr (GPIOC_BASE+8) //0x40011008
#define GPIOD_IDR_Addr (GPIOD_BASE+8) //0x40011408
#define GPIOE_IDR_Addr (GPIOE_BASE+8) //0x40011808
#define GPIOF_IDR_Addr (GPIOF_BASE+8) //0x40011A08
#define GPIOG_IDR_Addr (GPIOG_BASE+8) //0x40011E08

//IO 口操作,只对单一的 IO 口!
//确保 n 的值小于 16!
#define PAout(n) BIT_ADDR(GPIOA_ODR_Addr,n) //输出
#define PAin(n) BIT_ADDR(GPIOA_IDR_Addr,n) //输入
#define PBout(n) BIT_ADDR(GPIOB_ODR_Addr,n) //输出
#define PBin(n) BIT_ADDR(GPIOB_IDR_Addr,n) //输入
#define PCout(n) BIT_ADDR(GPIOC_ODR_Addr,n) //输出
#define PCin(n) BIT_ADDR(GPIOC_IDR_Addr,n) //输入
#define PDout(n) BIT_ADDR(GPIOD_ODR_Addr,n) //输出
#define PDin(n) BIT_ADDR(GPIOD_IDR_Addr,n) //输入
#define PEout(n) BIT_ADDR(GPIOE_ODR_Addr,n) //输出
#define PEin(n) BIT_ADDR(GPIOE_IDR_Addr,n) //输入

#define PFout(n) BIT_ADDR(GPIOF_ODR_Addr,n) //输出
#define PFin(n) BIT_ADDR(GPIOF_IDR_Addr,n) //输入
#define PGout(n) BIT_ADDR(GPIOG_ODR_Addr,n) //输出
#define PGin(n) BIT_ADDR(GPIOG_IDR_Addr,n) //输入



#endif
