#include "iic.h"

/****************************************************************************************
 * @description: IIC初始化
 * @return {*}
 ****************************************************************************************/
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(IIC_SCL_PORT_RCC | IIC_SDA_PORT_RCC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = IIC_SCL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(IIC_SCL_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
	GPIO_Init(IIC_SDA_PORT, &GPIO_InitStructure);

	IIC_SCL = 1;
	IIC_SDA = 1;
}

/****************************************************************************************
 * @description: SDA输出配置
 * @return {*}
 ****************************************************************************************/
void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(IIC_SDA_PORT, &GPIO_InitStructure);
}

/****************************************************************************************
 * @description: SDA输入配置
 * @return {*}
 ****************************************************************************************/
void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(IIC_SDA_PORT, &GPIO_InitStructure);
}

/****************************************************************************************
 * @description: 产生IIC起始信号
 * @return {*}
 ****************************************************************************************/
void IIC_Start(void)
{
	SDA_OUT(); // sda线输出
	IIC_SDA = 1;
	IIC_SCL = 1;
	delay_us(5);
	IIC_SDA = 0; // START:when CLK is high,DATA change form high to low
	delay_us(6);
	IIC_SCL = 0; // 钳住I2C总线，准备发送或接收数据
}

/****************************************************************************************
 * @description: 产生IIC停止信号
 * @return {*}
 ****************************************************************************************/
void IIC_Stop(void)
{
	SDA_OUT(); // sda线输出
	IIC_SCL = 0;
	IIC_SDA = 0; // STOP:when CLK is high DATA change form low to high
	IIC_SCL = 1;
	delay_us(6);
	IIC_SDA = 1; // 发送I2C总线结束信号
	delay_us(6);
}

/****************************************************************************************
 * @description: 等待应答信号到来
 * @return {u8} 1-接收应答失败	0-接收应答成功
 ****************************************************************************************/
u8 IIC_Wait_Ack(void)
{
	u8 tempTime = 0;

	IIC_SDA = 1;
	delay_us(1);
	SDA_IN(); // SDA设置为输入
	IIC_SCL = 1;
	delay_us(1);
	while (READ_SDA)
	{
		tempTime++;
		if (tempTime > 250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL = 0; // 时钟输出0
	return 0;
}

/****************************************************************************************
 * @description: 产生ACK应答
 * @return {*}
 ****************************************************************************************/
void IIC_Ack(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 0;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(5);
	IIC_SCL = 0;
}

/****************************************************************************************
 * @description: 产生NACK非应答
 * @return {*}
 ****************************************************************************************/
void IIC_NAck(void)
{
	IIC_SCL = 0;
	SDA_OUT();
	IIC_SDA = 1;
	delay_us(2);
	IIC_SCL = 1;
	delay_us(5);
	IIC_SCL = 0;
}

/****************************************************************************************
 * @description: IIC发送一个字节
 * @param {u8} txd 发送的字节
 * @return {*}
 ****************************************************************************************/
void IIC_Send_Byte(u8 txd)
{
	u8 t;
	SDA_OUT();
	IIC_SCL = 0; // 拉低时钟开始数据传输
	for (t = 0; t < 8; t++)
	{
		if ((txd & 0x80) > 0) // 0x80  1000 0000
			IIC_SDA = 1;
		else
			IIC_SDA = 0;
		txd <<= 1;
		delay_us(2); // 对TEA5767这三个延时都是必须的
		IIC_SCL = 1;
		delay_us(2);
		IIC_SCL = 0;
		delay_us(2);
	}
}

/****************************************************************************************
 * @description: IIC读一个字节
 * @param {u8} ack =1时，发送ACK，=0，发送nACK
 * @return {u8} receive 应答或非应答
 ****************************************************************************************/
u8 IIC_Read_Byte(u8 ack)
{
	u8 i, receive = 0;
	SDA_IN(); // SDA设置为输入
	for (i = 0; i < 8; i++)
	{
		IIC_SCL = 0;
		delay_us(2);
		IIC_SCL = 1;
		receive <<= 1;
		if (READ_SDA)
			receive++;
		delay_us(1);
	}
	if (!ack)
		IIC_NAck(); // 发送nACK
	else
		IIC_Ack(); // 发送ACK
	return receive;
}
