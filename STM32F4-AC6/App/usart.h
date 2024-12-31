#ifndef __usart_H
#define __usart_H

#include "system.h"


#define FRAME_HEADER 0X7B   // 帧头{  发送数据时为:{data}
#define FRAME_TAIL 0X7D     // 帧尾}
#define RECEIVE_DATA_SIZE 5 // 缓冲区的大小 5


void USART1_Init(u32 bound);
void USART1_Send(u8 *data, u8 len);

#endif
