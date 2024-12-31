#include "system.h"


#define mode (0) // 0: 裸机  1：FreeRTOS
#define START_TASK_PRIO		1			//任务优先级
#define START_STK_SIZE 		128 	//任务堆栈大小	
TaskHandle_t StartTask_Handler;	//任务句柄

void start_task(void *pvParameters);

	
extern u8 Recvl_Data[3];					// 串口接收数据

int main()
{
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//设置系统中断优先级分组2


	USART1_Init(9600);
	LED_Init(); 				//LED灯初始化，0亮  1灭

#if !mode
	
    while (1)
    {
				if(Recvl_Data[2] == 1)
				{
					led1 = 0;
				}
				else if(Recvl_Data[2] == 2)
				{
					led1 = 1;
				}
				LED_Test();
    }
#endif

	
#if mode
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
#endif 

}


//开始任务任务函数
void start_task(void *pvParameters)
{

	taskENTER_CRITICAL();           //进入临界区

	//处理蓝牙数据任务
	xTaskCreate(Bluetooth_control_task,	"Bluetooth_control_task",	Bluetooth_control_STK_SIZE,	NULL, 	Bluetooth_control_TASK_PRIO,	NULL);  

	vTaskDelete(StartTask_Handler); //删除开始任务
	taskEXIT_CRITICAL();            //退出临界区
} 


