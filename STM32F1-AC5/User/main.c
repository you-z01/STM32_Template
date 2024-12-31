#include "system.h"


#define mode (0) // 0: ���  1��FreeRTOS
#define START_TASK_PRIO		1			//�������ȼ�
#define START_STK_SIZE 		128 	//�����ջ��С	
TaskHandle_t StartTask_Handler;	//������

void start_task(void *pvParameters);

	
extern u8 Recvl_Data[3];					// ���ڽ�������

int main()
{
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//����ϵͳ�ж����ȼ�����2


	USART1_Init(9600);
	LED_Init(); 				//LED�Ƴ�ʼ����0��  1��

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
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
#endif 

}


//��ʼ����������
void start_task(void *pvParameters)
{

	taskENTER_CRITICAL();           //�����ٽ���

	//����������������
	xTaskCreate(Bluetooth_control_task,	"Bluetooth_control_task",	Bluetooth_control_STK_SIZE,	NULL, 	Bluetooth_control_TASK_PRIO,	NULL);  

	vTaskDelete(StartTask_Handler); //ɾ����ʼ����
	taskEXIT_CRITICAL();            //�˳��ٽ���
} 


