#include "usart.h"

#if 0

uint8_t Recvl_data[128] = {0}; // ���ڽ��ջ���
u8 rx_count = 0;			  // �������ݸ�����������
/*===================================================================================================*/
// �ض���printf����,��Ҫѡ��use MicroLIB
// int fputc(int ch,FILE *p)  //����Ĭ�ϵģ���ʹ��printf����ʱ�Զ�����
//{
//	USART_SendData(USART1,(u8)ch);
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
//	return ch;
// }

// �ض���printf����,����Ҫѡ��use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
// ��׼����Ҫ��֧�ֺ���
struct __FILE
{
	int handle;
};

FILE __stdout;
// ����_sys_exit()�Ա���ʹ�ð�����ģʽ
void _sys_exit(int x)
{
	x = x;
}
// �ض���fputc����
int fputc(int ch, FILE *f)
{
	while ((USART1->SR & 0X40) == 0)
	{
	} // ѭ������,ֱ���������
	USART1->DR = (u8)ch;
	return ch;
}
#endif
/*===================================================================================================*/

/**************************************************************************************** 
 * @description: ����1��ʼ������
 * @param {u32} bound ������
 * @return {*}
 ****************************************************************************************/
void USART1_init(u32 bound)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; // A9��TX A10��RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			// ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			// �������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// ����50MHZ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			// ����
	GPIO_Init(GPIOA, &GPIO_InitStructure);					// ��ʼ��

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // ��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  // �����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; // �ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;			// ����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // �շ�ģʽ
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}


/**************************************************************************************** 
 * @description: �жϴ�����
 * @return {*}
 ****************************************************************************************/
int count = 0;
int USART1_IRQHandler(void)
{

	u8 Usart_Receive;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		Usart_Receive = USART_ReceiveData(USART1);
		//		if(Time_count<1000)
		//			return 0;	//ǰ�ڲ������ж� ����10��ǰ����������
		Recvl_data[rx_count] = Usart_Receive;
		count += 1;
		if (Usart_Receive == FRAME_Header || rx_count > 0) // ȷ����һ������Ϊ֡ͷ
		{
			rx_count++;
		}
		else
		{
			rx_count = 0;
		}
		if (Usart_Receive == FRAME_Tail) // ȷ�����һ������Ϊ֡β
		{
			rx_count = 0;
		}
		USART_SendData(USART1, Usart_Receive);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			; // �ȴ��������
	}

	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	return 0;
}

/**************************************************************************************** 
 * @description:  ����1���ͺ���
 * @param {u8} *data ����������
 * @param {u8} len ���ݳ���
 * @return {*}
 ****************************************************************************************/
void USART1_send(u8 *data, u8 len)
{
	u8 i;
	for (i = 0; i < len; i++)
	{
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
			;
		USART_SendData(USART1, data[i]);
	}
}

#endif
