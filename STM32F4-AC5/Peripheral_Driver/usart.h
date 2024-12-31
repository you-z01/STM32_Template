#ifndef __usart_H
#define __usart_H

#define DATA_TASK_PRIO 2
#define DATA_STK_SIZE 512

#define FRAME_HEADER 0X7B   // 帧头{  发送数据时为:{data}
#define FRAME_TAIL 0X7D     // 帧尾}
#define RECEIVE_DATA_SIZE 5 // 缓冲区的大小 5

#include "system.h"

void USART1_init(u32 bound);
void USART1_send(u8 *data, u8 len);

#endif
