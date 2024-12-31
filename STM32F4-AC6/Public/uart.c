#include "usart.h"

#if 0

uint8_t Recvl_data[128] = {0}; // 串口接收缓存
u8 rx_count = 0;			  // 接收数据个数计数变量
/*===================================================================================================*/
// 重定向printf函数,需要选择use MicroLIB
// int fputc(int ch,FILE *p)  //函数默认的，在使用printf函数时自动调用
//{
//	USART_SendData(USART1,(u8)ch);
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
//	return ch;
// }

// 重定向printf函数,不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
// 标准库需要的支持函数
struct __FILE
{
	int handle;
};

FILE __stdout;
// 定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
// 重定义fputc函数
int fputc(int ch, FILE *f)
{
	while ((USART1->SR & 0X40) == 0)
	{
	} // 循环发送,直到发送完毕
	USART1->DR = (u8)ch;
	return ch;
}
#endif
/*===================================================================================================*/

/**************************************************************************************** 
 * @description: 串口1初始化函数
 * @param {u32} bound 波特率
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

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; // A9—TX A10—RX
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			// 输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			// 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		// 高速50MHZ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			// 上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);					// 初始化

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; // 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  // 子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	USART_InitStructure.USART_BaudRate = bound;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; // 字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		// 一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;			// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; // 收发模式
	USART_Init(USART1, &USART_InitStructure);

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);
}


/**************************************************************************************** 
 * @description: 中断处理函数
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
		//			return 0;	//前期不进入中断 开机10秒前不处理数据
		Recvl_data[rx_count] = Usart_Receive;
		count += 1;
		if (Usart_Receive == FRAME_Header || rx_count > 0) // 确保第一个数据为帧头
		{
			rx_count++;
		}
		else
		{
			rx_count = 0;
		}
		if (Usart_Receive == FRAME_Tail) // 确保最后一个数据为帧尾
		{
			rx_count = 0;
		}
		USART_SendData(USART1, Usart_Receive);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			; // 等待发送完成
	}

	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	return 0;
}

/**************************************************************************************** 
 * @description:  串口1发送函数
 * @param {u8} *data 待发送数据
 * @param {u8} len 数据长度
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
