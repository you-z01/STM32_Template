#ifndef __BlUETOOTH_CONTROL_H
#define __BlUETOOTH_CONTROL_H

#include "system.h"

void Bluetooth_control_task(void *pvParameters);
#define Bluetooth_control_TASK_PRIO 4  // 任务优先级
#define Bluetooth_control_STK_SIZE 256 // 任务堆栈大小

#endif
