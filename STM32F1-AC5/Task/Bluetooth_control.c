/**
 * @Author: z
 * @Date: 2024-11-14
 * @LastEditTime: 2024-11-14
 * @Description: 蓝牙控制任务
 * @FilePath: \AHrobot\Task\Bluetooth_control.c
 */
#include "Bluetooth_control.h"

// #define Bluetooth_control_TASK_PRIO 4  // 任务优先级
// #define Bluetooth_control_STK_SIZE 256 // 任务堆栈大小

extern u8 Recvl_Data[3]; // 串口接收数据

/****************************************************************************************
 * @description: 蓝牙控制任务
 * @param {void} *pvParameters 传递任务启动时需要的初始化参数或其他相关信息
 * @return {*}
 ****************************************************************************************/
void Bluetooth_control_task(void *pvParameters)
{
	u32 lastWakeTime = getSysTickCnt();
	while (1)
	{
		vTaskDelayUntil(&lastWakeTime, F2T(100)); // 此任务以100Hz的频率运行（10ms控制一次）

		switch (Recvl_Data[2])
		{
		case '1':
			led1 = 1;
			Recvl_Data[2] = '0';
			break;
		case '2':
			led1 = 0;
			Recvl_Data[2] = '0';
			break;

		default:
			break;
		}
	}
}
