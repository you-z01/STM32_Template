/*
 * FreeRTOS V202212.01
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

#define configUSE_PREEMPTION		1


/***************************************************************
             FreeRTOS�빳�Ӻ����йص�����ѡ��                                            
**************************************************************/
/* ��1��ʹ�ÿ��й��ӣ�Idle Hook�����ڻص�����������0�����Կ��й���
 * 
 * ������������һ������������������û���ʵ�֣�
 * FreeRTOS�涨�˺��������ֺͲ�����void vApplicationIdleHook(void )��
 * ���������ÿ�������������ڶ��ᱻ����
 * �����Ѿ�ɾ����RTOS���񣬿�����������ͷŷ�������ǵĶ�ջ�ڴ档
 * ��˱��뱣֤����������Ա�CPUִ��
 * ʹ�ÿ��й��Ӻ�������CPU����ʡ��ģʽ�Ǻܳ�����
 * �����Ե��û������������������API����
 */
#define configUSE_IDLE_HOOK			0  //����ʱ ���Ӻ��� �ص�����

/* ��1��ʹ��ʱ��Ƭ���ӣ�Tick Hook������0������ʱ��Ƭ����
 * 
 * 
 * ʱ��Ƭ������һ������������������û���ʵ�֣�
 * FreeRTOS�涨�˺��������ֺͲ�����void vApplicationTickHook(void )
 * ʱ��Ƭ�жϿ��������Եĵ���
 * ��������ǳ���С�����ܴ���ʹ�ö�ջ��
 * ���ܵ����ԡ�FromISR" �� "FROM_ISR����β��API����
 */
 /*xTaskIncrementTick��������xPortSysTickHandler�жϺ����б����õġ���ˣ�vApplicationTickHook()����ִ�е�ʱ�����̲ܶ���*/
#define configUSE_TICK_HOOK			0

//ʹ���ڴ�����ʧ�ܹ��Ӻ���
#define configUSE_MALLOC_FAILED_HOOK			0 


#define configCPU_CLOCK_HZ			( ( unsigned long ) 72000000 )
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )
#define configMAX_PRIORITIES		( 32 ) //��ʹ�õ�������ȼ�
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 128 )   //��128�֣������ֽ� ��128*4���ֽڣ���ջ��С����Ϊ��λ
#define configTOTAL_HEAP_SIZE		( ( size_t ) ( 15 * 1024 ) )
#define configMAX_TASK_NAME_LEN		( 16 )
#define configUSE_TRACE_FACILITY	0
#define configUSE_16_BIT_TICKS		0 //ϵͳ���ļ����������������ͣ�1��ʾΪ16λ�޷������Σ�0��ʾΪ32λ�޷�������
#define configIDLE_SHOULD_YIELD		1 //1:�����������CPUʹ��Ȩ,������ͬ���ȼ����û�����  2:�������ȼ����������ȼ���ͬ������2����ʹ��1

#define configUSE_TIME_SLICING      1 //ʱ��Ƭ���ȣ������ȼ���ͬʱִ��


#define configUSE_QUEUE_SETS        0 //���� Ϊ1������Ϊ0�ر�



#define configUSE_TASK_NOTIFICATIONS 1 //��������֪ͨ���ܣ�Ĭ�Ͽ���


#define configUSE_MUTEXES           0  //�����ź�������


                                           
#define configUSE_RECURSIVE_MUTEXES			0   //ʹ�õݹ黥���ź��� 


#define configUSE_COUNTING_SEMAPHORES		0  //Ϊ1ʱʹ�ü����ź���

/* ���ÿ���ע����ź�������Ϣ���и��� */
#define configQUEUE_REGISTRY_SIZE			10                                 
                                                                       
#define configUSE_APPLICATION_TASK_TAG		 0         


/*****************************************************************
              FreeRTOS���ڴ������й�����ѡ��                                               
*****************************************************************/
//֧�ֶ�̬�ڴ�����
#define configSUPPORT_DYNAMIC_ALLOCATION     1    
//֧�־�̬�ڴ�
#define configSUPPORT_STATIC_ALLOCATION		 0					


/*
 * ����0ʱ���ö�ջ�����⹦�ܣ����ʹ�ô˹��� 
 * �û������ṩһ��ջ������Ӻ��������ʹ�õĻ�
 * ��ֵ����Ϊ1����2����Ϊ������ջ�����ⷽ�� */
#define configCHECK_FOR_STACK_OVERFLOW		 0   


/********************************************************************
          FreeRTOS������ʱ�������״̬�ռ��йص�����ѡ��   
**********************************************************************/
//��������ʱ��ͳ�ƹ���
#define configGENERATE_RUN_TIME_STATS	     0             
 //���ÿ��ӻ����ٵ���
#define configUSE_TRACE_FACILITY			 0    
/* ���configUSE_TRACE_FACILITYͬʱΪ1ʱ���������3������
 * prvWriteNameToBuffer()
 * vTaskList(),
 * vTaskGetRunTimeStats()
*/

                                                                        
/********************************************************************
                FreeRTOS��Э���йص�����ѡ��                                                
*********************************************************************/
//����Э�̣�����Э���Ժ��������ļ�croutine.c
#define configUSE_CO_ROUTINES 			        0                 
//Э�̵���Ч���ȼ���Ŀ
#define configMAX_CO_ROUTINE_PRIORITIES       ( 2 )                   


/***********************************************************************
                FreeRTOS�������ʱ���йص�����ѡ��      
**********************************************************************/
 //���������ʱ��
#define configUSE_TIMERS				        0                              
//�����ʱ�����ȼ�
#define configTIMER_TASK_PRIORITY		      (configMAX_PRIORITIES-1)        
//�����ʱ�����г���
#define configTIMER_QUEUE_LENGTH		        10                               
//�����ʱ�������ջ��С
#define configTIMER_TASK_STACK_DEPTH	      (configMINIMAL_STACK_SIZE*2)    




/*
 * ĳЩ����FreeRTOs��Ӳ�������ַ���ѡ����һ��Ҫִ�е�����:
 *
 * ͨ�÷������ض���Ӳ���ķ��������¼��"���ⷽ��")��
 * ͨ�÷���:
 * 1.configUSE_PORT_OPTIMISED_TASK_SELECTIONΪ������Ӳ����֧���������ⷽ����
 * 2.������������FreeRTOS֧�ֵ�Ӳ��
 * 3.��ȫ��cʵ��,Ч���Ե������ⷽ����
 * 4.��ǿ��Ҫ���������������ȼ���Ŀ

 * ���ⷽ��:
 * 1.���뽫configUSE_PORT_OPTIMISED_TASK_SELECTION����Ϊ1��
 * 2.����һ�������ض��ܹ��Ļ��ָ�һ�������Ƽ���ǰ����[CLZ]ָ���
 * 3.��ͨ�÷�������Ч
 * 4.һ��ǿ���޶����������ȼ���ĿΪ32
 * һ����Ӳ������ǰ����ָ������ʹ�õģ�MCUû����ЩӲ��ָ��Ļ��˺�Ӧ������Ϊ0 !
*/
#define configUSE_PORT_OPTIMISED_TASK SELECTION  1

/*
 * configUSE_TICKLESS_IDLE  

 * ��1:ʹ�ܵ͹���ticklessģʽ;��0:����ϵͳ����(tick���ж�һֱ����
 * ���迪���͹��ĵĻ����ܻᵼ�����س������⣬��Ϊ������˯����,�������°취���
 * ���ط���:
 * 1.���������������Ӻ�
 * 2.��ס��λ����,�������˲���ɿ���λ����
 * 

 * 1.ͨ������ñ��BO0T 0�Ӹߵ�ƽ(3.3v)
 * 2.�����ϵ�,����
 * 1.ʹ��FlyMcu����һ��оƬ,Ȼ���������STMISP ->���оƬ(z)
 * 
 */
#define configUSE_TICKLESS_IDLE   0





/*���ñ�Ҫ������*/
#define xPortPendSVHandler              PendSV_Handler 
#define vPortSVCHandler                 SVC_Handler
#define INCLUDE_xTaskGetSchedulerState  1



/************************************************************
            FreeRTOS��ѡ��������ѡ��                                                     
************************************************************/
#define INCLUDE_xTaskGetSchedulerState       1                       
#define INCLUDE_vTaskPrioritySet		     1
#define INCLUDE_uxTaskPriorityGet		     1
#define INCLUDE_vTaskDelete				     1
#define INCLUDE_vTaskCleanUpResources	     1
#define INCLUDE_vTaskSuspend			     1
#define INCLUDE_vTaskDelayUntil			     1
#define INCLUDE_vTaskDelay				     1
#define INCLUDE_eTaskGetState			     1
#define INCLUDE_xTimerPendFunctionCall	     0
//#define INCLUDE_xTaskGetCurrentTaskHandle       1
//#define INCLUDE_uxTaskGetStackHighWaterMark     0
//#define INCLUDE_xTaskGetIdleTaskHandle          0



/******************************************************************
            FreeRTOS���ж��йص�����ѡ��                                                 
******************************************************************/
#ifdef __NVIC_PRIO_BITS
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		4                  
#endif
//�ж�������ȼ�
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			15     

//ϵͳ�ɹ��������ж����ȼ���
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5 //5ָ �ж����ȼ�  0~5�����ܿأ��ж�5~15��freertos�ܿ�

#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )	/* 240 */

#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )


#define vPortSVCHandler SVC_Handler
#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler
#define F2T(X) ((unsigned int)((configTICK_RATE_HZ/(X))))
/* This is the value being used as per the ST library which permits 16
priority values, 0 to 15.  This must correspond to the
configKERNEL_INTERRUPT_PRIORITY setting.  Here 15 corresponds to the lowest
NVIC value of 255. */
#define configLIBRARY_KERNEL_INTERRUPT_PRIORITY	15

#endif /* FREERTOS_CONFIG_H */


