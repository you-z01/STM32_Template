#include "usart.h"		 
/*===================================================================================================*/
//�ض���printf����,��Ҫѡ��use MicroLIB	  
//int fputc(int ch,FILE *p)  //����Ĭ�ϵģ���ʹ��printf����ʱ�Զ�����
//{
//	USART_SendData(USART1,(u8)ch);	
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
//	return ch;
//}

//�ض���printf����,����Ҫѡ��use MicroLIB	  
#if 1
#ifdef  __CC_ARM
#pragma import(__use_no_semihosting)
struct __FILE 
{ 
	int handle; 
}; 
#elif defined ( __GNUC__ ) || defined (__clang__)
__asm (".global __use_no_semihosting\n\t");   
#endif


FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}

#endif


/*===================================================================================================*/


u8 Recvl_Buffer[RECEIVE_DATA_SIZE]; // ���ڽ������ݻ���
u8 Recvl_Data[3];					// ���ڽ�������


/**************************************************************************
�������ܣ����ݴ�����
��ڲ�����bound:������
����  ֵ����
**************************************************************************/
void USART1_Init(u32 bound)
{
 //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
 	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //�������PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* ��ʼ����������IO */
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //��������PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //ģ������
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* ��ʼ��GPIO */
	
   //USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//����������
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART1, &USART_InitStructure); //��ʼ������1
	
	USART_Cmd(USART1, ENABLE);  //ʹ�ܴ���1
	//USART_ClearFlag(USART1, USART_FLAG_TC);	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//��������ж�

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

}


/**************************************************************************
�������ܣ�USART1�жϺ���
��ڲ�������
����  ֵ����
**************************************************************************/
void USART1_IRQHandler(void)//����2�жϷ������
{
	static u8 Count = 0;
	u8 Usart_Receive;
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)    // �ж��Ƿ���յ�����
	{
		Usart_Receive = USART_ReceiveData(USART1); // ��ȡ����
		Recvl_Buffer[Count] = Usart_Receive;	   // �����������뻺����
		if (Usart_Receive == FRAME_HEADER || Count > 0) // ֡ͷУ��
		{
			Count++;
		}
		else
		{
			Count = 0; // �������֡ͷ�����ü�����
		}

		if (Count == RECEIVE_DATA_SIZE) // ��֤���ݰ��ĳ���
		{
			Count = 0;											   // Ϊ��һ�ν���������׼��							   // Ϊ����������������������׼��
			if (Recvl_Buffer[RECEIVE_DATA_SIZE - 1] == FRAME_TAIL) // ��֤���ݰ���֡β
			{
				Recvl_Data[0] = Recvl_Buffer[1]; // ��������
				Recvl_Data[1] = Recvl_Buffer[2];
				Recvl_Data[2] = Recvl_Buffer[3];
			}
		}
		
		USART_SendData(USART1, Usart_Receive); // �ش�����
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			; // �ȴ��������
	}
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}  


/**************************************************************************
�������ܣ�����1���ͺ���
��ڲ�����{u8} *data ���������� {u8} len ���ݳ���
����  ֵ����
**************************************************************************/
void USART1_Send(u8 *data, u8 len)
{
	u8 i;
	for (i = 0; i < len; i++)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, data[i]);
	}
} 
