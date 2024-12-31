#include "usart.h"		 
/*===================================================================================================*/
//重定向printf函数,需要选择use MicroLIB	  
//int fputc(int ch,FILE *p)  //函数默认的，在使用printf函数时自动调用
//{
//	USART_SendData(USART1,(u8)ch);	
//	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
//	return ch;
//}

//重定向printf函数,不需要选择use MicroLIB	  
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
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}

#endif


/*===================================================================================================*/


u8 Recvl_Buffer[RECEIVE_DATA_SIZE]; // 串口接收数据缓存
u8 Recvl_Data[3];					// 串口接收数据


/**************************************************************************
函数功能：数据处理函数
入口参数：bound:波特率
返回  值：无
**************************************************************************/
void USART1_Init(u32 bound)
{
 //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
 	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;//TX			   //串口输出PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* 初始化串口输入IO */
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//RX			 //串口输入PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //模拟输入
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* 初始化GPIO */
	
   //USART1 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	
	USART_Cmd(USART1, ENABLE);  //使能串口1
	//USART_ClearFlag(USART1, USART_FLAG_TC);	
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

}


/**************************************************************************
函数功能：USART1中断函数
入口参数：无
返回  值：无
**************************************************************************/
void USART1_IRQHandler(void)//串口2中断服务程序
{
	static u8 Count = 0;
	u8 Usart_Receive;
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)    // 判断是否接收到数据
	{
		Usart_Receive = USART_ReceiveData(USART1); // 读取数据
		Recvl_Buffer[Count] = Usart_Receive;	   // 串口数据填入缓冲区
		if (Usart_Receive == FRAME_HEADER || Count > 0) // 帧头校验
		{
			Count++;
		}
		else
		{
			Count = 0; // 如果不是帧头，重置计数器
		}

		if (Count == RECEIVE_DATA_SIZE) // 验证数据包的长度
		{
			Count = 0;											   // 为下一次接收数据做准备							   // 为串口数据重新填入数组做准备
			if (Recvl_Buffer[RECEIVE_DATA_SIZE - 1] == FRAME_TAIL) // 验证数据包的帧尾
			{
				Recvl_Data[0] = Recvl_Buffer[1]; // 接收数据
				Recvl_Data[1] = Recvl_Buffer[2];
				Recvl_Data[2] = Recvl_Buffer[3];
			}
		}
		
		USART_SendData(USART1, Usart_Receive); // 回传数据
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
			; // 等待发送完成
	}
	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
}  


/**************************************************************************
函数功能：串口1发送函数
入口参数：{u8} *data 待发送数据 {u8} len 数据长度
返回  值：无
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
