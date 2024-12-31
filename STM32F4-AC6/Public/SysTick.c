#include "SysTick.h"

static u8 fac_us = 0;  // us��ʱ������
static u16 fac_ms = 0; // ms��ʱ������

static u32 sysTickCnt = 0;

/****************************************************************************************
 * @description: getSysTickCnt()
 * @return {TickType_t} ���ȿ���֮ǰ ���� sysTickCnt
						���ȿ���֮ǰ ���� xTaskGetTickCount()
 ****************************************************************************************/
u32 getSysTickCnt(void)
{
	if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED) /*ϵͳ�Ѿ�����*/
		return xTaskGetTickCount();
	else
		return sysTickCnt;
}

/****************************************************************************************
 * @description: SysTick��ʼ����SYSTICK��ʱ�ӹ̶�ΪAHBʱ�ӵ�1/8
 * @param {u8} SYSCLK ϵͳʱ��Ƶ��
 * @return {*}
 ****************************************************************************************/
void SysTick_Init(u8 SYSCLK)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	fac_us = SYSCLK / 8;		 // SYSCLK��8��Ƶ ����1us����ļ�������
	fac_ms = (u16)fac_us * 1000; // ÿ��ms��Ҫ��systickʱ����
}

/****************************************************************************************
 * @description: us��ʱ
 * @param {u32} nus Ҫ��ʱ��us��
					ע��:nus��ֵ,��Ҫ����798915us(���ֵ��2^24/fac_us@fac_us=21)
 * @return {*}
 ****************************************************************************************/
void delay_us(u32 nus)
{
	u32 temp;
	SysTick->LOAD = nus * fac_us; // ʱ�����
	SysTick->VAL = 0x00;		  // ��ռ�����
	SysTick->CTRL |= 0x01;		  // ��ʼ����
	do
	{
		temp = SysTick->CTRL;
	} while ((temp & 0x01) && !(temp & (1 << 16))); // �ȴ�ʱ�䵽��
	SysTick->CTRL &= ~0x01; // �رռ�����
	SysTick->VAL = 0X00;	// ��ռ�����
}

/****************************************************************************************
 * @description: ms��ʱ
 * @param {u16} nms Ҫ��ʱ��ms��
					ע��:nms��ֵ,SysTick->LOADΪ24λ�Ĵ�����
					��Ҫ����0xffffff*8*1000/SYSCLKRecv1
					��168M������,nms<=798ms
 * @return {*}
 ****************************************************************************************/
void delay_nms(u16 nms)
{
	u32 temp;
	SysTick->LOAD = (u32)nms * fac_ms; // ʱ�����(SysTick->LOADΪ24bit)
	SysTick->VAL = 0x00;			   // ��ռ�����
	SysTick->CTRL |= 0x01;			   // ��ʼ����
	do
	{
		temp = SysTick->CTRL;
	} while ((temp & 0x01) && !(temp & (1 << 16))); // �ȴ�ʱ�䵽��
	SysTick->CTRL &= ~0x01; // �رռ�����
	SysTick->VAL = 0X00;	// ��ռ�����
}

/****************************************************************************************
 * @description: ms��ʱ
 * @param {u16} nms Ҫ��ʱ��ms�� 0~65535
 * @return {*}
 ****************************************************************************************/
void delay_ms(u16 nms)
{
	u8 repeat = nms / 540; // ������540,�ǿ��ǵ�ĳЩ�ͻ����ܳ�Ƶʹ��,
						   // ���糬Ƶ��248M��ʱ��,delay_nms���ֻ����ʱ541ms������
	u16 remain = nms % 540;
	while (repeat)
	{
		delay_nms(540);
		repeat--;
	}
	if (remain)
		delay_nms(remain);
}
